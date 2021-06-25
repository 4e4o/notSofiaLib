#include "DriverCommunicator.h"
#include "ViChannel.h"
#include "VoChannel.h"
#include "Chip.h"
#include "Motion.h"
#include "MotionLoop.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <bitset>
#include <stdexcept>

#include <nvp6134_ex/motion.h>

#define DRIVER_FILE "/dev/nc_vdec"

#define CHANNEL_LINEAR_INDEX(CHIP, CHANNEL) \
    ((CHIP * ChipSpecs::CS_VI_CHANNELS_COUNT) + CHANNEL)

#define CHANNEL_INDEX(IND, CHIP, CHANNEL) \
    int IND = CHANNEL_LINEAR_INDEX(CHIP, CHANNEL); \
    if ((IND < 0) || (IND >= (CHANNEL_LINEAR_INDEX(m_chipCount, ChipSpecs::CS_VI_CHANNELS_COUNT)))) \
        throw std::runtime_error("Incorrect channel index");

namespace nvp6134 {

// this is driver restriction
static const int MAX_CHIP_COUNT = 4;

using namespace std;

DriverCommunicator::DriverCommunicator(int chipCount)
    : m_chipCount(chipCount),
      m_driverFd(-1),
      m_motionLoop(new MotionLoop(this)) {
    if ((m_chipCount > MAX_CHIP_COUNT) || (m_chipCount < 0))
        throw std::runtime_error("Incorrect chip count");
}

DriverCommunicator::~DriverCommunicator() {
    stopMotionThread();
    closeDriver();
}

bool DriverCommunicator::configureImpl() {
    if (!openDriver())
        throw std::runtime_error("Can't open driver");

    detectVideoFmt();
    startMotionThread();
    return true;
}

DriverCommunicator::ViChannelFormat *DriverCommunicator::getVideoFmt(
    const ViChannel *ch) {
    CHANNEL_INDEX(ind, ch->parent()->id(), ch->id());
    return m_channelInputFormats[ind].get();
}

void DriverCommunicator::detectVideoFmt() {
    if (!getVideoFmt())
        return;

    for (int i = 0 ; i < m_chipCount ; i++) {
        for (int j = 0 ; j < ChipSpecs::CS_VI_CHANNELS_COUNT ; j++) {
            CHANNEL_INDEX(ind, i, j);
            ViChannelFormat *fmt = new ViChannelFormat();

            fmt->inputvideofmt = m_inputFormat->inputvideofmt[ind];
            fmt->getvideofmt = m_inputFormat->getvideofmt[ind];
            fmt->geteqstage = m_inputFormat->geteqstage[ind];
            ::memcpy(fmt->getacpdata, m_inputFormat->getacpdata, 8);
            m_channelInputFormats.emplace_back(fmt);
        }
    }
}

bool DriverCommunicator::setViChannelMode(const ViChannel *ch, bool pal,
        NVP6134_VI_MODE chmode) {
    CHANNEL_INDEX(ind, ch->parent()->id(), ch->id());

    nvp6134_chn_mode mode;

    mode.chmode = chmode;
    // pal тут имеет сакральное значние
    // см. комментарии в ViChannel::isNvpPal

    mode.vformat = pal;
    mode.ch = ind;

    if (::ioctl(m_driverFd, IOC_VDEC_SET_CHNMODE, &mode) == -1)
        return false;

    return true;
}

void DriverCommunicator::resetInputVideoFmt() {
    // Специально добавленная функция в драйвер чтоб сбрасывать внутренние переменные и
    // состояние драйвера, иначе детекция видео формата выдаёт ебанутые результаты,
    // приходилось делать rmmod/insmod nvp6134_ex.ko
    ::ioctl(m_driverFd, IOC_VDEC_VIDEO_FMT_RESET);
}

bool DriverCommunicator::getVideoFmt() {
    resetInputVideoFmt();

    m_inputFormat.reset(new nvp6134_input_videofmt{});

    if (::ioctl(m_driverFd, IOC_VDEC_GET_INPUT_VIDEO_FMT,
                m_inputFormat.get()) == -1)
        return false;

    return true;
}

bool DriverCommunicator::openDriver() {
    if (m_driverFd >= 0)
        return true;

    m_driverFd = ::open(DRIVER_FILE, O_RDWR);

    if (m_driverFd < 0) {
        cout << "open nvp6134 " << DRIVER_FILE << " " << "fail" << endl;
        return false;
    }

    return true;
}

bool DriverCommunicator::setViMotionEnabled(const ViChannel *c) {
    const unsigned long int req = c->motion()->enabled() ?
                                  IOC_VDEC_ENABLE_MOTION :
                                  IOC_VDEC_DISABLE_MOTION ;
    unsigned int ch = c->id();

    std::cout << "setViMotionEnabled: " << ch << " , " <<
              c->motion()->enabled() << std::endl;

    if (::ioctl(m_driverFd, req, &ch) == -1)
        return false;

    return true;
}

DriverCommunicator::TChipsChannels DriverCommunicator::getMotion() const {
    nvp6134_motion_data data;

    // юзаем hold mode
    // в котором регистры md будут 1 в случае движения пока мы не прочитаем их
    data.mode = 1;

    if (::ioctl(m_driverFd, IOC_VDEC_GET_MOTION_INFO, &data) == -1)
        return {};

    // std::cout << "getMotion: " << std::dec << (int) ids << std::endl;

    TChipsChannels result;
    std::bitset<ChipSpecs::CS_VI_CHANNELS_COUNT> channels;

    for (int chip = 0 ; chip < m_chipCount ; chip++) {
        channels = (data.motion >> (MAX_CHIP_COUNT * chip)) & 0xFF;

        // std::cout << "getMotion2: " << chip << " , " << channels.to_ulong() << std::endl;

        for (int c = 0 ; c < ChipSpecs::CS_VI_CHANNELS_COUNT ; c++) {
            if (channels.test(c))
                result.emplace_back(chip, c);
        }
    }

    return result;
}

bool DriverCommunicator::setViMotion(const ViChannel *c) {
    Motion *m = c->motion();
    TChipChannel id{c->parent()->id(), c->id()};

    // если выключаем то не надо другие найстроки крутить
    if (!m->enabled()) {
        m_motionLoop->remove(id);
        return setViMotionEnabled(c);
    }

    const bool enabled = setViMotionSensitivity(c) &&
                         setViMotionArea(c) &&
                         setViMotionVisualize(c) &&
                         setViMotionEnabled(c);

    if (enabled) {
        m_motionLoop->watch(id, [m]() { m->event()(m); });
    }

    return enabled;
}

bool DriverCommunicator::setViMotionSensitivity(const ViChannel *c) {
    nvp6134_motion_sens sens;

    sens.ch = c->id();
    sens.sens = static_cast<uint8_t>(c->motion()->sensitivity());

    if (::ioctl(m_driverFd, IOC_VDEC_SET_MOTION_SENS, &sens) == -1)
        return false;

    return true;
}

bool DriverCommunicator::setViMotionArea(const ViChannel *c) {
    nvp6134_motion_area area{};
    area.ch = c->id();

    std::bitset<8> byte;
    int bitIndex = 7;
    int blockIndex = 0;

    std::cout << "setViMotionArea: " << std::dec << (int) area.ch << std::endl;

    for (const auto &row : c->motion()->area()) {
        for (bool column : row) {
            byte.set(bitIndex--, column);

            if (bitIndex < 0) {
                area.blockset[blockIndex++] = byte.to_ulong();

                std::cout << std::hex << (int) area.blockset[blockIndex - 1] << " ";

                if ((blockIndex % 2) == 0) {
                    std::cout << std::endl;
                }

                bitIndex = 7;
                byte.reset();
            }
        }
    }

    std::cout << std::dec;

    if (::ioctl(m_driverFd, IOC_VDEC_SET_MOTION_AREA, &area) == -1)
        return false;

    return true;
}

bool DriverCommunicator::setViMotionVisualize(const ViChannel *c) {
    unsigned int onoff = c->motion()->visualize();

    if (::ioctl(m_driverFd, IOC_VDEC_SET_MOTION_DISPLAY, &onoff) == -1)
        return false;

    return true;
}

void DriverCommunicator::startMotionThread() {
    m_motionThread.reset(new std::thread([this]() {
        m_motionLoop->run();
    }));
}

void DriverCommunicator::stopMotionThread() {
    m_motionLoop->stop();
    m_motionThread->join();
}

bool DriverCommunicator::setVoChannelMode(const VoChannel *vo,
        unsigned char chid, NVP6134_OUTMODE_SEL mode) {
    nvp6134_opt_mode optmode;

    optmode.chipsel = vo->parent()->id();
    optmode.portsel = vo->id() + 1;
    optmode.portmode = mode;
    // TODO тут чид зависит от NVP6134_OUTMODE_SEL
    // наверно надо тут его выбирать а не передавать в setVoChannelMode
    optmode.chid = chid;

    return ::ioctl(m_driverFd, IOC_VDEC_SET_OUTPORTMODE, &optmode) != -1;
}

bool DriverCommunicator::closeDriver() {
    if (m_driverFd < 0)
        return false;

    return close(m_driverFd) == 0;
}

}

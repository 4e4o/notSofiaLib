#include "DriverCommunicator.h"
#include "ViChannel.h"
#include "VoChannel.h"
#include "Chip.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>

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
      m_driverFd(-1) {
    if ((m_chipCount > MAX_CHIP_COUNT) || (m_chipCount < 0))
        throw std::runtime_error("Incorrect chip count");

    if (!openDriver())
        throw std::runtime_error("Can't open driver");

    detectVideoFmt();
}

DriverCommunicator::~DriverCommunicator() {
    closeDriver();
}

DriverCommunicator::ViChannelFormat* DriverCommunicator::getVideoFmt(const ViChannel* ch) {
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

bool DriverCommunicator::setViChannelMode(const ViChannel* ch, bool pal, NVP6134_VI_MODE chmode) {
    CHANNEL_INDEX(ind, ch->parent()->id(), ch->id());

    nvp6134_chn_mode mode;

    mode.chmode = chmode;
    mode.vformat = pal;
    mode.ch = ind;

    if (::ioctl(m_driverFd, IOC_VDEC_SET_CHNMODE, &mode) == -1)
        return false;

    return true;
}

bool DriverCommunicator::getVideoFmt() {
    m_inputFormat.reset(new nvp6134_input_videofmt());
    ::memset(m_inputFormat.get(), 0, sizeof(nvp6134_input_videofmt));

    if (::ioctl(m_driverFd, IOC_VDEC_GET_INPUT_VIDEO_FMT, m_inputFormat.get()) == -1)
        return false;

    for(int i = 0; i < m_chipCount * ChipSpecs::CS_VI_CHANNELS_COUNT; i++) {
        cout << "[DriverCommunicator::getVideoFmt] " << i << " fmt = " <<
                m_inputFormat->getvideofmt[i] << endl;\
    }

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

bool DriverCommunicator::setVoChannelMode(const VoChannel* vo, unsigned char chid, NVP6134_OUTMODE_SEL mode) {
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

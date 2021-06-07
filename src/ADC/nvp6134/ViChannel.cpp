#include "ViChannel.h"
#include "DriverCommunicator.h"
#include "Chip.h"
#include "Utils/VideoFormat.h"
#include "VoChannel.h"

#include <stdexcept>
#include <iostream>

#define CASE_SIZE(C, X, Y) \
    case C: { \
    TSize s; \
    s.width = X; \
    s.height = Y; \
    return s; }

namespace nvp6134 {

using namespace std;

ViChannel::ViChannel(Chip* p, int id)
    : ChipChild(p), IdHolder(id),
      m_videoFormat(TVideoFormat::DF_NOT_DETECTED),
      m_mode(NVP6134_VI_BUTT),
      m_outChannel(nullptr) {
    init();
}

ViChannel::~ViChannel() {
}

void ViChannel::init() {
    DriverCommunicator::ViChannelFormat* fmt = parent()->driver()->getVideoFmt(this);

    if (fmt == nullptr)
        throw std::runtime_error("Driver does not provide vi format");

    // TODO verify that getvideofmt exist in TDetectedFormat
    m_videoFormat = static_cast<TVideoFormat>(fmt->getvideofmt);
}

// По даташиту nvp6134 поддерживает только 4:2:2 yuv
ViChannel::OutPixelFormat ViChannel::pixelFormat() const {
    return OutPixelFormat::YUV_422;
}

ViChannel::TVideoFormat ViChannel::videoFormat() const {
    return m_videoFormat;
}

void ViChannel::setOutChannel(VoChannel* vo) {
    m_outChannel = vo;
}

TSize ViChannel::captureSize() const {
    if (m_outChannel == nullptr)
        throw std::runtime_error("Out channel not set");

    TSize s = modeToSize();

    // вот не понятно почему это делается
    // софия так же делает
    // без этого картинка двоится по горизонтали
    // а по вертикали снизу пустое место
    if (inXFormatMode())
        s.width = s.width / 2;

    return s;
}

// Возвращает просто размер, без каких-либо поправок на режимы
TSize ViChannel::videoFormatSize() const {
    switch(m_videoFormat) {
    case DF_CVBS_PAL:
        CASE_SIZE(DF_CVBS_NTSC, 704, CVBS_HEIGHT(pal()));
    case DF_1080P_PAL:
        CASE_SIZE(DF_1080P_NTSC, 1920, 1080);
        // TODO другие форматы
    case DF_NOT_DETECTED:
        throw std::runtime_error("Format not detected");
    default:
        throw std::runtime_error("Unimplemented format size");
    }
}

// Возвращает размер картинки канала
// с поправкой на режим канала
TSize ViChannel::imageSize() const {
    TSize s = videoFormatSize();

    if (inXFormatMode())
        s.width = s.width / 2;

    return s;
}

bool ViChannel::inXFormatMode() const {
    // для vo режима NVP6134_OUTMODE_2MUX_MIX:
    // https://github.com/4e4o/nvp6134_ex_170306/blob/master/video.c#L1419
    // и для vi режимов ahd_1080_30, ahd_1080_25
    // https://github.com/4e4o/nvp6134_ex_170306/blob/master/video.c#L1432
    // ширина урезается / 2
    // для vo режима NVP6134_OUTMODE_4MUX_MIX:
    // тоже самое делается https://github.com/4e4o/nvp6134_ex_170306/blob/master/video.c#L1387
    // т.е. по nvp6134 даташиту это ставится x-format channel mode
    // который делит пополам ширину картинки
    // см. даташит стр. 21 2.10.2
    if (m_outChannel->isMixMode()) {
        switch(m_videoFormat) {
        // TODO тут другие форматы еще соотвествуют ahd_1080_30, ahd_1080_25
        // надо их добавить сюда, какие именно я хз
        case DF_1080P_NTSC:
        case DF_1080P_PAL:
            return true;
        default:
            return false;
        }
    }

    return false;
}

TSize ViChannel::modeToSize() const {
    if (m_mode == NVP6134_VI_BUTT)
        throw "mode is undefined";

    switch(m_mode) {
    CASE_SIZE(NVP6134_VI_720H, 720, CVBS_HEIGHT(pal()));
    CASE_SIZE(NVP6134_VI_960H, 960, CVBS_HEIGHT(pal()));
    CASE_SIZE(NVP6134_VI_1280H, 1280, CVBS_HEIGHT(pal()));
    CASE_SIZE(NVP6134_VI_1440H, 1440, CVBS_HEIGHT(pal()));
    CASE_SIZE(NVP6134_VI_1920H, 1920, CVBS_HEIGHT(pal()));
    CASE_SIZE(NVP6134_VI_960H2EX, 3840, CVBS_HEIGHT(pal()));
    CASE_SIZE(NVP6134_VI_720P_2530, 1280, 720);
    CASE_SIZE(NVP6134_VI_EXC_720P, 1280, 720);
    CASE_SIZE(NVP6134_VI_EXT_720PA, 1280, 720);
    CASE_SIZE(NVP6134_VI_EXT_720PB, 1280, 720);
    CASE_SIZE(NVP6134_VI_HDEX, 2560, 720);
    CASE_SIZE(NVP6134_VI_EXC_HDEX, 2560, 720);
    CASE_SIZE(NVP6134_VI_EXT_HDAEX, 2560, 720);
    CASE_SIZE(NVP6134_VI_EXT_HDBEX, 2560, 720);
    CASE_SIZE(NVP6134_VI_720P_5060, 1280, 720);
    CASE_SIZE(NVP6134_VI_EXC_720PRT, 1280, 720);
    CASE_SIZE(NVP6134_VI_EXT_720PRT, 1280, 720);
    CASE_SIZE(NVP6134_VI_1080P_2530, 1920, 1080);
    CASE_SIZE(NVP6134_VI_EXC_1080P, 1920, 1080);
    CASE_SIZE(NVP6134_VI_EXT_1080P, 1920, 1080);
    CASE_SIZE(NVP6134_VI_1080P_NRT, 1920, 1080);
    CASE_SIZE(NVP6134_VI_1080P_NOVIDEO, 1920, 1080);
    CASE_SIZE(NVP6134_VI_3M_NRT, 2048, 1536);
    CASE_SIZE(NVP6134_VI_3M, 2048, 1536);
    CASE_SIZE(NVP6134_VI_EXT_3M_NRT, 2048, 1536);
    CASE_SIZE(NVP6134_VI_4M_NRT, 2560, 1440);
    CASE_SIZE(NVP6134_VI_4M, 2560, 1440);
    CASE_SIZE(NVP6134_VI_EXC_4M_NRT, 2688, 1520);
    CASE_SIZE(NVP6134_VI_EXC_4M, 2688, 1520);
    CASE_SIZE(NVP6134_VI_EXT_4M_NRT, 2688, 1520);
    CASE_SIZE(NVP6134_VI_5M_NRT, 2592, 1944);
    CASE_SIZE(NVP6134_VI_5M, 2592, 1944);
    CASE_SIZE(NVP6134_VI_EXT_5M_NRT, 2592, 1944);
    CASE_SIZE(NVP6134_VI_5M_20P, 2592, 1944);
    default:
        throw std::runtime_error("Unimplemented vi mode");
    }
}

bool ViChannel::pal() const{
    return (m_videoFormat == DF_CVBS_PAL) ||
            (m_videoFormat == DF_720P_PAL) ||
            (m_videoFormat == DF_720P_RT_PAL) ||
            (m_videoFormat == DF_1080P_PAL);
}

void ViChannel::setMode(NVP6134_VI_MODE m) {
    m_mode = m;
    parent()->driver()->setViChannelMode(this, pal(), m);
}

bool ViChannel::formatDetected() const {
    return m_videoFormat != DF_NOT_DETECTED;
}

}

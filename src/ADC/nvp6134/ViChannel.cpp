#include "ViChannel.h"
#include "DriverCommunicator.h"
#include "Chip.h"
#include "VoChannel.h"

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 255
#include "Misc/MagicEnum.hpp"

#include <stdexcept>
#include <iostream>

#define CVBS_HEIGHT(PAL) (PAL ? 576 : 480)

#define CASE_SIZE(C, X, Y) \
    case C: { \
        TSize s; \
        s.width = X; \
        s.height = Y; \
        return s; }

namespace nvp6134 {

using namespace std;

ViChannel::ViChannel(Chip *p, int id)
    : ChipChild(p), IdHolder(id),
      m_videoFormat(TVideoFormat::DF_NOT_DETECTED),
      m_mode(NVP6134_VI_BUTT),
      m_outChannel(nullptr) {
    init();
}

ViChannel::~ViChannel() {
}

void ViChannel::init() {
    using TFormat = DriverCommunicator::ViChannelFormat;
    TFormat *fmt = parent()->driver()->getVideoFmt(this);

    if (fmt == nullptr)
        throw std::runtime_error("Driver does not provide vi format");

    auto optionalFmt = magic_enum::enum_cast<TVideoFormat>(fmt->getvideofmt);

    if (!optionalFmt.has_value())
        throw std::runtime_error("getvideofmt maps to unknown TVideoFormat");

    m_videoFormat = optionalFmt.value();
    std::cout << "nvp6134::ViChannel::init " << id() << ", format: " <<
              magic_enum::enum_name(m_videoFormat) << ", fps: " << fps() << std::endl;
}

// По даташиту nvp6134 поддерживает только 4:2:2 yuv
ViChannel::OutPixelFormat ViChannel::pixelFormat() const {
    return OutPixelFormat::YUV_422;
}

ViChannel::TVideoFormat ViChannel::videoFormat() const {
    return m_videoFormat;
}

void ViChannel::setOutChannel(VoChannel *vo) {
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
    switch (m_videoFormat) {
    case TVideoFormat::DF_CVBS_PAL:
        CASE_SIZE(TVideoFormat::DF_CVBS_NTSC, 704, CVBS_HEIGHT(isCvbsPal()));
    case TVideoFormat::DF_1080P_PAL:
    case TVideoFormat::DF_FHD_EXC_30P:
    case TVideoFormat::DF_FHD_EXC_25P:
    case TVideoFormat::DF_FHD_EXT_30P:
    case TVideoFormat::DF_FHD_EXT_25P:
        CASE_SIZE(TVideoFormat::DF_1080P_NTSC, 1920, 1080);
    // TODO другие форматы
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

float ViChannel::fps() const {
    if (m_videoFormat == TVideoFormat::DF_NOT_DETECTED)
        return 0;

    // Тут определяем fps по имени енум элемента TVideoFormat
    // если кончается на pal = 25, ntsc = 30, ЧислоБуква = Число fps
    std::string name(magic_enum::enum_name(m_videoFormat));
    size_t pos = name.find_last_of("_");

    if (pos == std::string::npos)
        throw std::runtime_error("Invalid video format name, fix it!");

    name = name.substr(pos + 1);
    float fps = 0;

    if (name == "NTSC")
        fps = 30.0f;
    else if (name == "PAL")
        fps = 25.0f;
    else {
        if (std::isalpha(name.back()))
            name = name.substr(0, name.size() - 1);

        std::replace(name.begin(), name.end(), 'd', '.');
        fps = std::stof(name);
    }

    return fps;
}

bool ViChannel::isCvbsPal() const {
    if (!cvbs())
        return false;

    return (m_videoFormat == TVideoFormat::DF_CVBS_PAL);
}

bool ViChannel::isNvpPal() const {
    if (m_videoFormat == TVideoFormat::DF_NOT_DETECTED)
        return false;

    // Значит тут у нас имеется хрень:
    // Китайский драйвер писал инвалид-третъеклашка с оторванными руками из жопы
    // Этот метод специально сделан для bool DriverCommunicator::setViChannelMode()
    // Он отличается от isCvbsPal() тем что предназначем для драйвера, в котором он имеет многозначное
    // применение. С одной стороны для cvbs он показывает действительно pal это или ntsc формат,
    // но так же он имеет смысл для других форматов (вот что это блять?). В драйвере для разных
    // форматов выставляется разные настройки фпс и еще всякие хрени непонятние в зависимости от
    // этого самого isNvpPal(). Я так понял что китаец думал что pal/ntsc относится к fps метрике
    // и наговнокодил там, возможно я ошибаюсь, но какая логика там я так и не понял.
    // Вобщем тут возвращаем isCvbsPal() для cvbs,
    // для других форматов если fps > 25 - return false(ntsc), остальные fps - return true (pal).

    if (cvbs())
        return isCvbsPal();

    if (fps() > 25.0f)
        return false;

    return true;
}

ViChannel::ScanMode ViChannel::scanMode() const {
    // TODO is this correct ?
    if (cvbs())
        return ScanMode::INTERLACED;

    return ScanMode::PROGRESSIVE;
}

bool ViChannel::inXFormatMode() const {
    // для vo режима NVP6134_OUTMODE_2MUX_MIX:
    // https://github.com/4e4o/nvp6134_ex/blob/master/video.c#L1419
    // и для vi режимов ahd_1080_30, ahd_1080_25
    // https://github.com/4e4o/nvp6134_ex/blob/master/video.c#L1432
    // ширина урезается / 2
    // для vo режима NVP6134_OUTMODE_4MUX_MIX:
    // тоже самое делается https://github.com/4e4o/nvp6134_ex/blob/master/video.c#L1387
    // т.е. по nvp6134 даташиту это ставится x-format channel mode
    // который делит пополам ширину картинки
    // см. даташит стр. 21 2.10.2
    if (m_outChannel->isMixMode()) {
        switch (m_videoFormat) {
        // TODO тут другие форматы еще соотвествуют ahd_1080_30, ahd_1080_25
        // надо их добавить сюда, какие именно я хз
        case TVideoFormat::DF_1080P_NTSC:
        case TVideoFormat::DF_1080P_PAL:
        case TVideoFormat::DF_FHD_EXC_30P:
        case TVideoFormat::DF_FHD_EXC_25P:
        case TVideoFormat::DF_FHD_EXT_30P:
        case TVideoFormat::DF_FHD_EXT_25P:
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

    switch (m_mode) {
        CASE_SIZE(NVP6134_VI_720H, 720, CVBS_HEIGHT(isCvbsPal()));
        CASE_SIZE(NVP6134_VI_960H, 960, CVBS_HEIGHT(isCvbsPal()));
        CASE_SIZE(NVP6134_VI_1280H, 1280, CVBS_HEIGHT(isCvbsPal()));
        CASE_SIZE(NVP6134_VI_1440H, 1440, CVBS_HEIGHT(isCvbsPal()));
        CASE_SIZE(NVP6134_VI_1920H, 1920, CVBS_HEIGHT(isCvbsPal()));
        CASE_SIZE(NVP6134_VI_960H2EX, 3840, CVBS_HEIGHT(isCvbsPal()));
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

bool ViChannel::cvbs() const {
    return (m_videoFormat == TVideoFormat::DF_CVBS_PAL) ||
           (m_videoFormat == TVideoFormat::DF_CVBS_NTSC);
}

void ViChannel::setMode(NVP6134_VI_MODE m) {
    m_mode = m;
    parent()->driver()->setViChannelMode(this, isNvpPal(), m);
}

bool ViChannel::formatDetected() const {
    return m_videoFormat != TVideoFormat::DF_NOT_DETECTED;
}

}

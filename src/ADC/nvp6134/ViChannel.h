#ifndef NVP_6134_VI_CHANNEL_H
#define NVP_6134_VI_CHANNEL_H

#include <nvp6134_ex/video.h>

#include "ChipChild.h"
#include "Misc/IdHolder.h"
#include "Misc/Size.h"

namespace nvp6134 {

class VoChannel;
class Motion;

class ViChannel : public ChipChild, public IdHolder {
  public:
    // значения взяты из nvp6134_ex/video.c, см. тело функции nvp6134_vfmt_convert
    // https://github.com/4e4o/nvp6134_ex/blob/master/video.c#L332
    enum class TVideoFormat {
        DF_NOT_DETECTED = 0x00,

        DF_CVBS_NTSC = 0x01,
        DF_CVBS_PAL = 0x02,

        DF_720P_NTSC = 0x04,
        DF_720P_PAL = 0x08,
        DF_720P_RT_NTSC = 0x51,
        DF_720P_RT_PAL = 0x52,

        DF_1080P_NTSC = 0x40,
        DF_1080P_PAL = 0x80,

        DF_HD_EXC_30P = 0x11,
        DF_HD_EXC_25P = 0x12,
        DF_HD_EXT_30A = 0x31,
        DF_HD_EXT_24A = 0x32,
        DF_HD_EXT_30B = 0x34,
        DF_HD_EXT_25B = 0x38,

        DF_HD_EXC_60P = 0x51,
        DF_HD_EXC_50P = 0x52,
        DF_HD_EXT_60 = 0x54,
        DF_HD_EXT_50 = 0x58,

        DF_FHD_EXC_30P = 0x71,
        DF_FHD_EXC_25P = 0x72,
        DF_FHD_EXT_30P = 0x74,
        DF_FHD_EXT_25P = 0x78,

        DF_QHD_AHD_30P = 0x81,
        DF_QHD_AHD_25P = 0x82,
        DF_QHD_AHD_NRT_15P = 0x83,

        DF_5M_AHD_12d5P = 0xA0,
        DF_5M_20P = 0xA1,
        DF_5M_EXT_12d5P = 0xA2,

        DF_3M_AHD_NRT_18P = 0x90,
        DF_3M_AHD_RT_30P = 0x91,
        DF_3M_AHD_RT_25P = 0x92,
        DF_3M_EXT_NRT_18P = 0x93
    };

    enum class OutPixelFormat {
        YUV_422
    };

    enum class ScanMode {
        PROGRESSIVE,
        INTERLACED
    };

    ViChannel(Chip *, int id);
    ~ViChannel();

    bool formatDetected() const;
    OutPixelFormat pixelFormat() const;
    TVideoFormat videoFormat() const;
    TSize captureSize() const;
    TSize imageSize() const;
    float fps() const;
    ScanMode scanMode() const;
    Motion *motion() const;

    void setMode(NVP6134_VI_MODE m);
    void setOutChannel(VoChannel *);

  private:
    void init();
    TSize modeToSize() const;
    TSize videoFormatSize() const;
    bool inXFormatMode() const;
    bool isCvbsPal() const;
    bool isNvpPal() const;
    bool cvbs() const;

    TVideoFormat m_videoFormat;
    NVP6134_VI_MODE m_mode;
    VoChannel *m_outChannel;
    std::unique_ptr<Motion> m_motion;
};

}

#endif // NVP_6134_VI_CHANNEL_H

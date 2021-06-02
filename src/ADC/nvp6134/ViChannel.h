#ifndef NVP_6134_VI_CHANNEL_H
#define NVP_6134_VI_CHANNEL_H

#include <nvp6134_ex_170306/video.h>

#include "ChipChild.h"
#include "Utils/IdHolder.h"

namespace nvp6134 {

class ViChannel : public ChipChild, public IdHolder {
public:
    // значения взяты из nvp6134_ex_170306/video.c, см. тело функции nvp6134_vfmt_convert
    enum TVideoFormat {
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

    ViChannel(Chip*, int id);
    ~ViChannel();

    bool pal() const;

    void setMode(NVP6134_VI_MODE m);

    TVideoFormat videoFormat() const;

private:
    void init();

    TVideoFormat m_videoFormat;
};

}

#endif // NVP_6134_VI_CHANNEL_H

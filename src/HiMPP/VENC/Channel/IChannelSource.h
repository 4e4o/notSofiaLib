#ifndef MPP_VENC_H264_I_CHANNEL_SOURCE_H
#define MPP_VENC_H264_I_CHANNEL_SOURCE_H

#include <hi_comm_venc.h>

namespace hisilicon::mpp::venc {

class IChannelSource {
public:
    IChannelSource() { }
    virtual ~IChannelSource() { }

    virtual SIZE_S imgSize() const = 0;
    virtual bool pal() const = 0;
    virtual PIXEL_FORMAT_E pixelFormat() const = 0;
};

}

#endif // MPP_VENC_H264_I_CHANNEL_SOURCE_H

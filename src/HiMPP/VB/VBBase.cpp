#include "VBBase.h"
#include "VBufferizable.h"
#include "HiMPP/Sys/Sys.h"

#include <cmath>

namespace hisilicon::mpp {

// размер буфер страйда должен быть кратен sysAlignWidth
// HiMPP Media Processing Software Development Reference.pdf
// page 90

HI_U32 VBBase::picVbBlkSize(IBufferizable *b) {
    const HI_U32 sysAlignWidth = parent()->sys()->sysWidthAlign();
    const SIZE_S imgSize = b->bufferImageSize();
    const PIXEL_FORMAT_E pixFmt = b->bufferPixelFormat();

    if ((PIXEL_FORMAT_YUV_SEMIPLANAR_422 != pixFmt) &&
            (PIXEL_FORMAT_YUV_SEMIPLANAR_420 != pixFmt)) {
        throw std::runtime_error("Unsupported pixel format");
    }

    // https://stackoverflow.com/questions/8561185/yuv-422-yuv-420-yuv-444
    // https://www.fourcc.org/yuv.php
    // https://wiki.videolan.org/YUV/
    // 16 bits for 422, 12 bits for 420
    const float bytesPerPixel = (PIXEL_FORMAT_YUV_SEMIPLANAR_422 == pixFmt) ? 2 : 1.5;
    const HI_U32 bufSize = std::ceil(imgSize.u32Width * imgSize.u32Height * bytesPerPixel);

    // потому что степень для CEILING_2_POWER должна быть кратна 2
    if (sysAlignWidth > 1)
        return CEILING_2_POWER(bufSize, sysAlignWidth);

    return bufSize;
}

}

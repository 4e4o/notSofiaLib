#include "Channel.h"

#include "Hisilicon/MPP/VI/Source/ChannelInfo.h"
#include "Hisilicon/MPP/VI/Device.h"

namespace hisilicon {
namespace mpp {
namespace vi {
namespace hi3520dv200 {

Channel::Channel(mpp::MPP *p, Device *d, int id)
    : mpp::vi::Channel(p, d, id) {
}

Channel::~Channel() {
}

VI_CHN_BIND_ATTR_S Channel::createBindAttrs() const {
    VI_CHN_BIND_ATTR_S attrs;
    attrs.ViDev = device()->id();

    if (attrs.ViDev == 0)
        attrs.ViWay = id() / 2;
    else
        attrs.ViWay = (id() - 4) / 2;

    return attrs;
}

TSize Channel::createDestSize() const {
    // HiMPP Media Processing Software Development Reference.pdf
    // page 136/135
    // for hi3520d:
    /** For primary attributes, the width and height
        in stCapRect are static attributes, and others
        are dynamic attributes. For secondary
        attributes, it is meaningless to set stCapRect
        and control the frame rate. Other attributes
        are dynamic attributes.

        stCapRect is based on the source picture. The
        width of stDestSize can be the same as or half
        of the width of stCapRect. The height of
        stDestSize must be half of the height of
        stCapRect in single-field capture mode. In
        dual-field capture mode, the heights must be
        the same.

        In interlaced capture mode, s32Y and
        u32Height must be 4-pixel aligned.
   */

    // тут смотрим на формат результирующего изображения
    // если width влазеет в cap::width / 2 то юзаем cap::width / 2
    // если не влазеет то юзаем cap::width

    auto i = info();

    const TSize capSize = i->capSize();
    const HI_U32 halfWidth = capSize.width / 2;
    const TSize imgSize = i->imgSize();
    TSize destSize = imgSize;

    if (imgSize.width <= halfWidth)
        destSize.width = halfWidth;

    return destSize;
}

}
}
}
}

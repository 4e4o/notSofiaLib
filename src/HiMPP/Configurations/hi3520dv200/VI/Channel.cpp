#include "Channel.h"

#include "HiMPP/Misc/Utils.h"

namespace hisilicon::mpp::vi::hi3520dv200 {

SIZE_S *Channel::createDestSize() const {
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

    const RECT_S captureRect = capRect();
    const HI_U32 halfCapWidth = captureRect.u32Width / 2;
    SIZE_S dest = Utils::rectToSize(captureRect);

    if (imgSize().u32Width <= halfCapWidth)
        dest.u32Width = halfCapWidth;

    return new SIZE_S{std::move(dest)};
}

}

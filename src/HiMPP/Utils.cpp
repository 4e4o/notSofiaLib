#include "Utils.h"

namespace hisilicon::mpp {

SIZE_S Utils::toMppSize(const TSize &s) {
    SIZE_S r{};
    r.u32Width = s.width;
    r.u32Height = s.height;
    return r;
}

RECT_S Utils::toMppRect(const TSize &s) {
    RECT_S r{};
    r.u32Width = s.width;
    r.u32Height = s.height;
    return r;
}

SIZE_S Utils::rectToSize(const RECT_S&r) {
    SIZE_S result{};
    result.u32Width = r.u32Width;
    result.u32Height = r.u32Height;
    return result;
}

}

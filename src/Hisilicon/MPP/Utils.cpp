#include "Utils.h"

namespace hisilicon {
namespace mpp {

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

}
}

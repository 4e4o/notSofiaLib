#ifndef MPP_UTILS_H
#define MPP_UTILS_H

#include <hi_common.h>

#include "Misc/Size.h"

namespace hisilicon::mpp {

class Utils {
public:
    static SIZE_S toMppSize(const TSize&);
    static RECT_S toMppRect(const TSize&);
    static SIZE_S rectToSize(const RECT_S&r);
};

}

#endif // MPP_UTILS_H

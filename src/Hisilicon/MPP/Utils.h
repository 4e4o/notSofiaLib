#ifndef MPP_UTILS_H
#define MPP_UTILS_H

#include <hi_common.h>

#include "Utils/Size.h"

namespace hisilicon {
namespace mpp {

class Utils {
public:
    static SIZE_S toMppSize(const TSize&);
    static RECT_S toMppRect(const TSize&);
};

}
}

#endif // MPP_UTILS_H

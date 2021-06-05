#ifndef MPP_CHILD_H
#define MPP_CHILD_H

#include "Utils/Child.h"
#include "MPP.h"

namespace hisilicon {
namespace mpp {

class MPPChild : public Child<MPP> {
public:
    MPPChild(MPP* p) : Child<MPP>(p) { }
};

}
}

#endif // MPP_CHILD_H

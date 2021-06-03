#include "MPP.h"
#include "ElementsFactory.h"

namespace hisilicon {
namespace mpp {
namespace hi3520dv200 {

MPP::MPP(ViInfoProvider *inf) :
    mpp::MPP(inf) {
    setFactory(new ElementsFactory());
}

}
}
}

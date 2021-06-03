#ifndef MPP_HI3520DV200_ELEMENTS_FACTORY_H
#define MPP_HI3520DV200_ELEMENTS_FACTORY_H

#include "Hisilicon/MPP/ElementsFactory.h"

namespace hisilicon {
namespace mpp {
namespace hi3520dv200 {

class MPP;
class ViDevice;
class ViChannel;

class ElementsFactory : public mpp::ElementsFactory {
public:
    ElementsFactory();
    ~ElementsFactory();

    mpp::ViSubsystem* vi(mpp::MPP*);
    mpp::ViDevice* viDevice(mpp::MPP*, int id);
    mpp::ViChannel* viChannel(mpp::MPP*, mpp::ViDevice*, int id);
};

}
}
}

#endif // MPP_HI3520DV200_ELEMENTS_FACTORY_H

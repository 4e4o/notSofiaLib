#ifndef MPP_HI3520DV200_ELEMENTS_FACTORY_H
#define MPP_HI3520DV200_ELEMENTS_FACTORY_H

#include "Hisilicon/MPP/ElementsFactory.h"

namespace hisilicon {
namespace mpp {
namespace hi3520dv200 {

class ElementsFactory : public mpp::ElementsFactory {
public:
    mpp::vi::Subsystem* vi(mpp::MPP*);
    mpp::vi::Device* viDevice(mpp::vi::Subsystem*, int id);
    mpp::vi::Channel* viChannel(mpp::vi::Device*, mpp::vi::ChannelInfo*, int id);
};

}
}
}

#endif // MPP_HI3520DV200_ELEMENTS_FACTORY_H

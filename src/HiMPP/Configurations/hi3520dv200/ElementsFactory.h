#ifndef MPP_HI3520DV200_ELEMENTS_FACTORY_H
#define MPP_HI3520DV200_ELEMENTS_FACTORY_H

#include "HiMPP/ElementsFactory.h"

namespace hisilicon::mpp::hi3520dv200 {

class ElementsFactory : public mpp::ElementsFactory {
public:
    mpp::vi::Subsystem* vi(mpp::MPP*) override;
    mpp::vi::Device* viDevice(mpp::vi::Subsystem*, int id) override;
    mpp::vi::Channel* viChannel(mpp::vi::Device*, mpp::vi::ChannelInfo*, int id) override;
};

}

#endif // MPP_HI3520DV200_ELEMENTS_FACTORY_H

#include "Subsystem.h"
#include "HiMPP/VI/Device.h"
#include "Channel.h"
#include "Device.h"

namespace hisilicon::mpp::vi::hi3520dv200 {

Subsystem::Subsystem(mpp::MPP *mpp)
    : mpp::vi::Subsystem(mpp),
      m_mode(HI3520DV200_MODE::MODE_UNKNOWN) {
    mpp->registerType<vi::Device, vi::hi3520dv200::Device, vi::Subsystem *, int>();
    mpp->registerType<vi::Channel, vi::hi3520dv200::Channel, vi::Device *, const vi::ChannelInfo *, int>();
}

void Subsystem::setHiMode(const Subsystem::HI3520DV200_MODE &mode) {
    m_mode = mode;
    initMode();
}

const Subsystem::HI3520DV200_MODE &Subsystem::hiMode() const {
    return m_mode;
}

void Subsystem::initMode() {
    switch (m_mode) {
    case HI3520DV200_MODE::MODE_4CH_72OP: {
        // id каналов фиксированы и зависят от хики чипа И режима его работы!
        // в нашем случае 4channel 720p режим для hi3520d v200
        // Hi3520D／Hi3515A／Hi3515C H.264 CODEC Processor Data Sheet.pdf
        // page 616
        vi::Device *dev0 = addDevice(0);
        vi::Device *dev1 = addDevice(1);

        dev0->addChannel(0, 0, 0);
        dev0->addChannel(2, 0, 1);

        dev1->addChannel(4, 0, 2);
        dev1->addChannel(6, 0, 3);
        break;
    }
    default:
        throw std::runtime_error("[vi::hi3520dv200::Subsystem] Unsupported HI3520DV200_MODE");
    }
}

bool Subsystem::configureImpl() {
    if (m_mode == HI3520DV200_MODE::MODE_UNKNOWN)
        throw std::runtime_error("[vi::hi3520dv200::Subsystem] You must set hi mode");

    return  mpp::vi::Subsystem::configureImpl();
}

}

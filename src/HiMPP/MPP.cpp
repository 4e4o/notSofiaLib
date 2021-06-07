#include "MPP.h"
#include "VI/Subsystem.h"
#include "VPSS/Subsystem.h"
#include "Sys/Sys.h"
#include "VideoBuffer.h"

#include <stdexcept>

namespace hisilicon::mpp {

MPP::MPP()
    : m_vi(nullptr),
      m_vpss(nullptr) {
    registerDefaultTypes();
}

void MPP::registerDefaultTypes() {
    registerType([] (MPP *p) -> VideoBuffer* {
        return new VideoBuffer(p);
    });
    registerType([] (MPP *p) -> Sys* {
        return new Sys(p);
    });
}

bool MPP::configureImpl() {
    // Здесь порядок важен
    addItem(create<VideoBuffer>(this));
    addItem(create<Sys>(this));
    addSubsystems();
    return Configurator::configureImpl();
}

void MPP::addSubsystems() {
}

vi::Subsystem* MPP::addViSubsystem() {
    if (m_vi != nullptr)
        throw std::runtime_error("vi subsystem already added");

    addItem(m_vi = create<vi::Subsystem>(this));
    return m_vi;
}

vpss::Subsystem* MPP::addVpssSubsystem() {
    if (m_vpss != nullptr)
        throw std::runtime_error("vpss subsystem already added");

    addItem(m_vpss = create<vpss::Subsystem>(this));
    return m_vpss;
}

vpss::Subsystem* MPP::vpss() const {
    return m_vpss;
}

Sys* MPP::sys() const {
    return static_cast<Sys*>(item(1));
}

vi::Subsystem* MPP::vi() const {
    return m_vi;
}

}

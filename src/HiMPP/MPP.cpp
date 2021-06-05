#include "MPP.h"
#include "VI/Subsystem.h"
#include "VPSS/Subsystem.h"
#include "Sys/Sys.h"
#include "VideoBuffer.h"
#include "ElementsFactory.h"

#include <stdexcept>

namespace hisilicon {
namespace mpp {

MPP::MPP(ElementsFactory* f)
    : m_factory(f),
      m_vi(NULL),
      m_vpss(NULL) {
    // Здесь порядок важен
    addItem(m_factory->videoBuffer(this));
    addItem(new Sys(this));
}

vi::Subsystem* MPP::addViSubsystem() {
    if (m_vi != NULL)
        throw std::runtime_error("vi subsystem already added");

    addItem(m_vi = m_factory->vi(this));
    return m_vi;
}

vpss::Subsystem* MPP::addVpssSubsystem() {
    if (m_vpss != NULL)
        throw std::runtime_error("vpss subsystem already added");

    addItem(m_vpss = m_factory->vpss(this));
    return m_vpss;
}

vpss::Subsystem* MPP::vpss() const {
    return m_vpss;
}

ElementsFactory* MPP::factory() const {
    return m_factory.get();
}

Sys* MPP::sys() const {
    return static_cast<Sys*>(item(1));
}

vi::Subsystem* MPP::vi() const {
    return m_vi;
}

}
}

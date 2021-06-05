#include "MPP.h"
#include "VI/Subsystem.h"
#include "Sys/Sys.h"
#include "VideoBuffer.h"
#include "ElementsFactory.h"

#include <stdexcept>

namespace hisilicon {
namespace mpp {

MPP::MPP(ElementsFactory* f)
    : m_factory(f),
      m_vi(NULL) {
    // Здесь порядок важен
    addItem(m_factory->videoBuffer(this));
    addItem(new Sys(this));
}

vi::Subsystem* MPP::addViSubsystem() {
    if (m_vi != NULL)
        throw std::runtime_error("vi subsystem already added");

    if (itemsCount() != 2)
        throw std::runtime_error("vi subsystem must be added first");

    addItem(m_vi = m_factory->vi(this));
    return m_vi;
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

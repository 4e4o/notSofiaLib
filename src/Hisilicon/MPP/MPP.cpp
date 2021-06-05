#include "MPP.h"
#include "VI/Subsystem.h"
#include "Sys/Sys.h"
#include "VideoBuffer.h"
#include "ElementsFactory.h"

namespace hisilicon {
namespace mpp {

MPP::MPP(ElementsFactory* f)
    : m_factory(f) {
    // Здесь порядок важен
    addItem(m_factory->videoBuffer(this));
    addItem(new Sys(this));
    addItem(m_factory->vi(this));
}

ElementsFactory* MPP::factory() const {
    return m_factory.get();
}

Sys* MPP::sys() const {
    return static_cast<Sys*>(item(1));
}

vi::Subsystem* MPP::vi() const {
    return static_cast<vi::Subsystem*>(item(2));
}

}
}

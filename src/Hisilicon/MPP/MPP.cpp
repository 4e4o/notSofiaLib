#include "MPP.h"
#include "VI/Source/InfoProvider.h"
#include "VI/Subsystem.h"
#include "Sys/Sys.h"
#include "VideoBuffer.h"
#include "ElementsFactory.h"

#include <stdexcept>

namespace hisilicon {
namespace mpp {

MPP::MPP(ElementsFactory* f)
    : m_factory(f) {
    addItem(m_factory->viInfoProvider());
    addItem(m_factory->videoBuffer(this));
    addItem(new Sys(this));
    addItem(m_factory->vi(this));
}

MPP::~MPP() {
}

ElementsFactory* MPP::factory() const {
    return m_factory.get();
}

vi::InfoProvider* MPP::viSourceInfo() const {
    return static_cast<vi::InfoProvider*>(item(0));
}

Sys* MPP::sys() const {
    return static_cast<Sys*>(item(2));
}

vi::Subsystem* MPP::vi() const {
    return static_cast<vi::Subsystem*>(item(3));
}

}
}

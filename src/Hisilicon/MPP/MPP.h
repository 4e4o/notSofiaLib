#ifndef MPP_H
#define MPP_H

#include <memory>

#include "Utils/Configurator/Configurator.h"

namespace hisilicon {
namespace mpp {

namespace vi{
class Subsystem;
}

class Sys;
class ElementsFactory;

class MPP : public Configurator {
public:
    MPP(ElementsFactory*);

    ElementsFactory* factory() const;

    Sys* sys() const;
    vi::Subsystem* vi() const;

private:
    std::unique_ptr<ElementsFactory> m_factory;
};

}
}

#endif // MPP_H

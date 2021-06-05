#ifndef MPP_H
#define MPP_H

#include <memory>

#include "Utils/Configurator/Configurator.h"

namespace hisilicon {
namespace mpp {

namespace vi {
class Subsystem;
}

namespace vpss {
class Subsystem;
}

class Sys;
class ElementsFactory;

class MPP : public Configurator {
public:
    MPP(ElementsFactory*);

    ElementsFactory* factory() const;

    vi::Subsystem* addViSubsystem();
    vpss::Subsystem* addVpssSubsystem();

    Sys* sys() const;
    vi::Subsystem* vi() const;
    vpss::Subsystem* vpss() const;

private:
    std::unique_ptr<ElementsFactory> m_factory;
    vi::Subsystem* m_vi;
    vpss::Subsystem* m_vpss;
};

}
}

#endif // MPP_H

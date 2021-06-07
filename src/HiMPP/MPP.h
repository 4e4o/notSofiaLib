#ifndef MPP_H
#define MPP_H

#include <memory>

#include "Utils/Configurator/Configurator.h"
#include "Utils/Configurator/ConfigurableFactory.h"

namespace hisilicon::mpp {

namespace vi {
class Subsystem;
}

namespace vpss {
class Subsystem;
}

class Sys;

class MPP : public Configurator, public ConfigurableFactory {
public:
    MPP();

    vi::Subsystem* addViSubsystem();
    vpss::Subsystem* addVpssSubsystem();

    Sys* sys() const;
    vi::Subsystem* vi() const;
    vpss::Subsystem* vpss() const;

private:
    bool configureImpl() override final;
    virtual void addSubsystems();
    void registerDefaultTypes();

    vi::Subsystem* m_vi;
    vpss::Subsystem* m_vpss;
};

}

#endif // MPP_H

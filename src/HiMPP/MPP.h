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

namespace venc {
class Subsystem;
}

class Sys;

class MPP : public Configurator, public ConfigurableFactory {
public:
    MPP();

    vi::Subsystem* addViSubsystem();
    vpss::Subsystem* addVpssSubsystem();
    venc::Subsystem* addVencSubsystem();

    Sys* sys() const;
    vi::Subsystem* vi() const;
    vpss::Subsystem* vpss() const;
    venc::Subsystem* venc() const;

    int veduCount() const;
    void setVeduCount(int veduCount);

private:
    bool configureImpl() override final;
    virtual void addSubsystems();
    void registerDefaultTypes();

    template<class T>
    T* addSubsystem(T*& store) {
        if (store != nullptr)
            throw std::runtime_error("subsystem already added");

        addItem(store = create<T>(this));
        return store;
    }

    vi::Subsystem* m_vi;
    vpss::Subsystem* m_vpss;
    venc::Subsystem* m_venc;
    // VEDU in HiMPP Media Processing Software Development Reference.pdf
    int m_veduCount;
};

}

#endif // MPP_H

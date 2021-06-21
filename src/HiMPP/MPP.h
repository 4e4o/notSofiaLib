#ifndef MPP_H
#define MPP_H

#include <memory>

#include "Misc/Configurator/Configurator.h"
#include "Misc/Configurator/ConfigurableFactory.h"
#include "ASubsystem/ReadLoop/LoopManager.h"

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
class VideoBuffer;

class MPP : public Configurator, public ConfigurableFactory,
    public LoopManager {
  public:
    MPP();

    vi::Subsystem *addViSubsystem();
    vpss::Subsystem *addVpssSubsystem();
    venc::Subsystem *addVencSubsystem();

    Sys *sys() const;
    vi::Subsystem *vi() const;
    vpss::Subsystem *vpss() const;
    venc::Subsystem *venc() const;

    int veduCount() const;
    void setVeduCount(int veduCount);

  protected:
    bool configureImpl() override;

  private:
    void registerDefaultTypes();

    template<class T, bool back = true>
    T * addSubsystem(T *&store) {
        if (store != nullptr)
            throw std::runtime_error("subsystem already added");

        store = create<T>(this);

        if constexpr (back)
            addItemBack(store);
        else
            addItemFront(store);

        return store;
    }

    Sys *m_sys;
    VideoBuffer *m_vb;
    vi::Subsystem *m_vi;
    vpss::Subsystem *m_vpss;
    venc::Subsystem *m_venc;
    // VEDU in HiMPP Media Processing Software Development Reference.pdf
    int m_veduCount;
};

}

#endif // MPP_H

#ifndef VPSS_BINDER_H
#define VPSS_BINDER_H

#include <memory>

#include <hi_common.h>

#include "Misc/Configurator/Configurable.h"

namespace hisilicon::mpp::vpss {

class BindSource;
class BindReceiver;

// HiMPP Media Processing Software Development Reference.pdf
// page 42

class Binder : public Configurable {
  public:
    Binder(BindSource *, BindReceiver *);
    ~Binder();

  private:
    bool configureImpl() override final;
    bool startImpl() override final;

    BindSource *m_source;
    BindReceiver *m_receiver;
    std::unique_ptr<MPP_CHN_S> m_in;
    std::unique_ptr<MPP_CHN_S> m_out;
};

}

#endif // VPSS_BINDER_H

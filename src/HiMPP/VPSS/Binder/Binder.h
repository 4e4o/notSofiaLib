#ifndef VPSS_BINDER_H
#define VPSS_BINDER_H

#include <memory>

#include <hi_common.h>

#include "Misc/Configurator/Configurable.h"

namespace hisilicon::mpp::vpss {

class BindItem;

// HiMPP Media Processing Software Development Reference.pdf
// page 42

class Binder : public Configurable {
  public:
    Binder(BindItem *, BindItem *);
    ~Binder();

  private:
    bool configureImpl() override final;

    BindItem *m_source;
    BindItem *m_receiver;
    std::unique_ptr<MPP_CHN_S> m_in;
    std::unique_ptr<MPP_CHN_S> m_out;
};

}

#endif // VPSS_BINDER_H

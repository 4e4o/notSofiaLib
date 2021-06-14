#ifndef MPP_VPSS_CONFIGURATOR_BINDER_H
#define MPP_VPSS_CONFIGURATOR_BINDER_H

#include "Misc/Configurator/Configurator.h"

namespace hisilicon::mpp::vpss {

class BindItem;

class ConfiguratorBinder : public Configurator {
  public:
    void bind(BindItem *, BindItem *);
};

}

#endif // MPP_VPSS_CONFIGURATOR_BINDER_H

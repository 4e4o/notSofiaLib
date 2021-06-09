#ifndef MPP_VPSS_CONFIGURATOR_BINDER_H
#define MPP_VPSS_CONFIGURATOR_BINDER_H

#include "Misc/Configurator/Configurator.h"

namespace hisilicon::mpp::vpss {

class BindSource;
class BindReceiver;

class ConfiguratorBinder : public Configurator {
protected:
    void bind(vpss::BindSource*, vpss::BindReceiver*);
};

}

#endif // MPP_VPSS_CONFIGURATOR_BINDER_H

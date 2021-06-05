#ifndef VPSS_BINDER_H
#define VPSS_BINDER_H

#include <memory>

#include <hi_common.h>

#include "Utils/Configurator/Configurable.h"

namespace hisilicon {
namespace mpp {
namespace vpss {

class BindSource;
class BindReceiver;

// HiMPP Media Processing Software Development Reference.pdf
// page 42

class Binder : public Configurable {
public:
    Binder(BindSource*, BindReceiver*);
    ~Binder();

private:
    bool configureImpl();
    bool startImpl();

    BindSource* m_source;
    BindReceiver* m_receiver;

    std::unique_ptr<MPP_CHN_S> m_in;
    std::unique_ptr<MPP_CHN_S> m_out;
};

}
}
}

#endif // VPSS_BINDER_H

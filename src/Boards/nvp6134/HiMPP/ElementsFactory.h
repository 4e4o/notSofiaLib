#ifndef BOARD_NVP6134_ELEMENTS_FACTORY_H
#define BOARD_NVP6134_ELEMENTS_FACTORY_H

#include "ViInfoProvider.h"

namespace boards::nvp6134::mpp {

template<class BaseFactory>
class ElementsFactory : public BaseFactory, public Holder<Board*> {
public:
    using Holder<Board*>::Holder;

    hisilicon::mpp::vi::InfoProvider* viInfoProvider() {
        return new vi::InfoProvider(value());
    }
};

}

#endif // BOARD_NVP6134_ELEMENTS_FACTORY_H

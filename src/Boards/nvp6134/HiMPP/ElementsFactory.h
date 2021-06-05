#ifndef BOARD_NVP6134_ELEMENTS_FACTORY_H
#define BOARD_NVP6134_ELEMENTS_FACTORY_H

#include "ViInfoProvider.h"

namespace boards {
namespace nvp6134 {
namespace mpp {

template<class BaseFactory>
class ElementsFactory : public BaseFactory, public Holder<Board*> {
public:
    ElementsFactory(Board* b)
        : Holder<Board*>(b) {
    }

    hisilicon::mpp::vi::InfoProvider* viInfoProvider() {
        return new vi::InfoProvider(value());
    }
};

}
}
}

#endif // BOARD_NVP6134_ELEMENTS_FACTORY_H

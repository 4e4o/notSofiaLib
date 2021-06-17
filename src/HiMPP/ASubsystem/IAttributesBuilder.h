#ifndef MPP_I_ATTRIBUTES_BUILDER_H
#define MPP_I_ATTRIBUTES_BUILDER_H

#include "Misc/PropertyContainer.h"

#include <hi_common.h>

#define MPP_ATTRIBUTE(KEY, TYPE) struct KEY : Key<TYPE> {}

namespace hisilicon::mpp {

template<class TAttrStruct, typename... ConstrArgs>
class IAttributesBuilder : public PropertyContainer {
  public:
    virtual ~IAttributesBuilder() { }

    TAttrStruct *build(ConstrArgs... args) {
        TAttrStruct *result = buildImpl(std::forward<ConstrArgs>(args)...);
        // После построения аттрибутов начинаем слушать изменения
        setNotifyOnChange(true);
        return result;
    }

  protected:
    static HI_BOOL toHIBool(bool v) {
        return v ? HI_BOOL::HI_TRUE : HI_BOOL::HI_FALSE;
    }

  private:
    virtual TAttrStruct *buildImpl(ConstrArgs...) = 0;
};

}

#endif // MPP_I_ATTRIBUTES_BUILDER_H

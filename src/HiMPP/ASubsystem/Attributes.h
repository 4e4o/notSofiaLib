#ifndef MPP_I_ATTRIBUTES_H
#define MPP_I_ATTRIBUTES_H

#include "Misc/PropertyContainer.h"

#include <hi_common.h>

#include <memory>

namespace hisilicon::mpp {

template<class Parent, class TAttrStruct>
class Attributes : public PropertyContainer {
  public:
    Attributes() : m_parent(nullptr) { }
    virtual ~Attributes() { }

    template<bool Required = true>
    TAttrStruct * createAttributes() {
        m_attributes.reset(buildAttributesImpl());

        if constexpr (Required) {
            if (m_attributes.get() == nullptr)
                throw std::runtime_error("[IAttributesBuilder::createAttributes] m_attributes is nullptr");
        }

        // После построения аттрибутов начинаем слушать изменения
        setNotifyOnChange(true);

        return m_attributes.get();
    }

    template<bool Required = true>
    void setAttributes() {
        createAttributes<Required>();

        if (m_attributes.get() != nullptr) {
            if (setAttributesImpl(m_attributes.get()) != HI_SUCCESS)
                throw std::runtime_error("[IAttributesBuilder::setAttributes] setAttributesImpl failed");
        }
    }

    template<class AttrHolder>
    void setParent(AttrHolder *p) {
        m_parent = static_cast<Parent *>(p);
        onAttached();
    }

  protected:
    virtual void onAttached() { }

    Parent *parent() const {
        if (m_parent == nullptr)
            throw std::runtime_error("parent is nullptr");

        return m_parent;
    }

    static HI_BOOL toHIBool(bool v) {
        return v ? HI_BOOL::HI_TRUE : HI_BOOL::HI_FALSE;
    }

  private:
    virtual TAttrStruct *buildAttributesImpl() = 0;
    virtual HI_S32 setAttributesImpl(TAttrStruct *) {
        return HI_FAILURE;
    }

    std::unique_ptr<TAttrStruct> m_attributes;
    Parent *m_parent;
};

}

#endif // MPP_I_ATTRIBUTES_H

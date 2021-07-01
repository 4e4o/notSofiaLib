#ifndef MPP_I_ATTRIBUTES_HOLDER_H
#define MPP_I_ATTRIBUTES_HOLDER_H

#include <tuple>

namespace hisilicon::mpp {

template<bool CreateFirstDefault, typename... Attributes>
class AttributesHolder {
  public:
    typedef std::tuple<Attributes...> TAttributes;
    typedef typename std::tuple_element<0, TAttributes>::type TIn0;
    typedef typename std::remove_pointer<TIn0>::type TFirstAttribute;

    AttributesHolder() {
        if constexpr (CreateFirstDefault) {
            setAttributes(new TFirstAttribute());
        }
    }

    virtual ~AttributesHolder() {
        std::apply([](const auto & ... items) {
            (delete items, ...);
        },  m_attributes);
    }

    template<class Attribute>
    void setAttributes(Attribute *a) {
        delete std::get<Attribute *>(m_attributes);
        std::get<Attribute *>(m_attributes) = a;
        a->setParent(this);
    }

    template<class Attribute = TFirstAttribute>
    Attribute * attributes() const {
        return std::get<Attribute *>(m_attributes);
    }

  private:
    TAttributes m_attributes;
};

}

#endif // MPP_I_ATTRIBUTES_HOLDER_H

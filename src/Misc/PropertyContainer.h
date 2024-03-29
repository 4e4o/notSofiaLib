#ifndef PROPERTY_CONTAINER_H
#define PROPERTY_CONTAINER_H

#include <string>
#include <any>
#include <map>
#include <functional>

#include "Misc/ClassName.h"

#define PROPERTY(KEY, TYPE) struct KEY : Key<TYPE> {}

class PropertyContainer {
  public:
    typedef std::function<void()> TPropertyChanged;

    PropertyContainer()
        : m_notifyChanged(false) {}

    template<typename ValueType>
    struct Key {
        typedef ValueType TValueType;
    };

    template<class KeyClass>
    void set(const typename KeyClass::TValueType &value) {
        constexpr auto className = getFullTypeName<KeyClass>();

        if (m_properties.contains(className))
            m_properties.erase(className);

        m_properties.emplace(className, value);

        if (m_notifyChanged && (m_changedEvent != nullptr))
            m_changedEvent();
    }

    template<class KeyClass>
    auto contains() const {
        constexpr auto className = getFullTypeName<KeyClass>();
        return m_properties.contains(className);
    }

    template<class KeyClass>
    auto get(const typename KeyClass::TValueType &def = {}) const {
        typedef typename KeyClass::TValueType TValueType;
        constexpr auto className = getFullTypeName<KeyClass>();

        if (!m_properties.contains(className))
            return def;

        return std::any_cast<const TValueType &>(m_properties.at(className));
    }

    template<class Callable>
    void setChangedEvent(Callable &&c) {
        m_changedEvent = std::move(c);
    }

  protected:
    void setNotifyOnChange(bool e) {
        m_notifyChanged = e;
    }

  private:
    std::map<std::string, std::any> m_properties;
    TPropertyChanged m_changedEvent;
    bool m_notifyChanged;
};

#endif // PROPERTY_CONTAINER_H

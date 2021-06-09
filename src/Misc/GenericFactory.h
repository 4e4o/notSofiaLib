#ifndef GENERIC_FACTORY_H
#define GENERIC_FACTORY_H

#include <map>
#include <string>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <concepts>
#include <any>

#include "Misc/ClassName.h"
#include "Misc/LambdaTraits.h"

template<class Base>
class GenericFactory {
public:
    template<LambdaPtrResultDerived<Base> Callable>
    void registerType(Callable&& c) {
        typedef typename MakeCreator<typename LambdaTraits<Callable>::args_tuple>::TCreator TCreator;
        TCreator creator{std::move(c)};
        using RetType = typename std::remove_pointer<typename LambdaTraits<Callable>::result_type>::type;
        constexpr auto className = getFullTypeName<RetType>();
        m_types.emplace(className, std::move(creator));
    }

    template<class T, typename... CreatorArgs>
    requires std::derived_from<T, Base>
    T* create(CreatorArgs... args) {
        constexpr auto className = getFullTypeName<T>();

        if (m_types.find(className) == m_types.end())
            throw std::runtime_error("Types does not registered");

        typedef typename MakeCreator<CreatorArgs...>::TCreator TCreator;
        std::any& c = m_types[className];
        TCreator& creator = std::any_cast<TCreator&>(c);
        return static_cast<T*>(creator(args...));
    }

private:
    template<typename... Args>
    struct MakeCreator {
        typedef std::function<Base*(Args...)> TCreator;
    };

    template<typename... Ts>
    struct MakeCreator<std::tuple<Ts...>> {
        typedef std::function<Base*(Ts...)> TCreator;
    };

    std::map<std::string, std::any> m_types;
};

#endif // GENERIC_FACTORY_H

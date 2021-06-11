#ifndef UTILS_H
#define UTILS_H

#include <type_traits>

class Utils {
  public:
    template<class C>
    static void clearPtrContainer(C &c, bool fromEnd = false) {
        if (fromEnd) {
            for (auto it = c.rbegin() ; it != c.rend() ; it++)
                delete (*it);
        } else {
            for (auto it = c.begin() ; it != c.end() ; it++)
                delete (*it);
        }

        if constexpr (!std::is_const<C>::value) {
            c.clear();
        }
    }
};

#endif // UTILS_H

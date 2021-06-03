#ifndef UTILS_H
#define UTILS_H

#include <type_traits>

class Utils {
public:
    template<class C>
    static void clearPtrContainer(C& c) {
        for (auto it = c.begin() ; it != c.end() ; it++)
            delete (*it);

        if (!std::is_const<C>::value) {
            c.clear();
        }
    }
};

#endif // UTILS_H

#ifndef UTILS_CHILD_H
#define UTILS_CHILD_H

#include "Holder.h"

template<typename Parent>
class Child : public Holder<Parent*> {
public:
    Child(Parent* p) : Holder<Parent*>(p) { }

    Parent* parent() const {
        return Holder<Parent*>::value();
    }
};

#endif // UTILS_CHILD_H

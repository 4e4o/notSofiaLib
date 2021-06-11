#ifndef CHILD_H
#define CHILD_H

#include "Holder.h"

template<typename Parent>
class Child : public Holder<Parent *> {
  public:
    using Holder<Parent *>::Holder;

    Parent *parent() const {
        return Holder<Parent *>::value();
    }
};

#endif // CHILD_H

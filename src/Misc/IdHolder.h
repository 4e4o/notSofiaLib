#ifndef ID_HOLDER_H
#define ID_HOLDER_H

#include "Holder.h"

class IdHolder : public Holder<int> {
public:
    using Holder<int>::Holder;

    int id() const {
        return Holder<int>::value();
    }
};

#endif // ID_HOLDER_H

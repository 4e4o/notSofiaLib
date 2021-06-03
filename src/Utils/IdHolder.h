#ifndef ID_HOLDER_H
#define ID_HOLDER_H

#include "Holder.h"

class IdHolder : public Holder<int> {
public:
    IdHolder(int id) : Holder<int>(id) { }

    int id() const {
        return Holder<int>::value();
    }
};

#endif // ID_HOLDER_H

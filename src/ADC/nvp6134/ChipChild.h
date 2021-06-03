#ifndef CHIP_CHILD_H
#define CHIP_CHILD_H

#include "Utils/Child.h"
#include "Chip.h"

namespace nvp6134 {

class ChipChild : public Child<Chip> {
public:
    ChipChild(Chip* c) : Child<Chip>(c) { }
};

}

#endif // CHIP_CHILD_H

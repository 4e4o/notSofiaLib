#ifndef CHIP_CHILD_H
#define CHIP_CHILD_H

namespace nvp6134 {

class Chip;

class ChipChild {
public:
    ChipChild(Chip* parent);
    ~ChipChild();

    Chip* parent() const;

private:
    Chip *m_parent;
};

}

#endif // CHIP_CHILD_H

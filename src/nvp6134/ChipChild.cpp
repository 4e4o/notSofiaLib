#include "ChipChild.h"

namespace nvp6134 {

ChipChild::ChipChild(Chip *parent)
    : m_parent(parent) {
}

ChipChild::~ChipChild() {
}

Chip *ChipChild::parent() const {
    return m_parent;
}

}

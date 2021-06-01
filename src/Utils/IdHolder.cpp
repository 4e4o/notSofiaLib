#include "IdHolder.h"

IdHolder::IdHolder(int id)
    : m_id(id) {
}

int IdHolder::id() const {
    return m_id;
}

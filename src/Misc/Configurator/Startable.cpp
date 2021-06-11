#include "Startable.h"

Startable::Startable()
    : m_started(false) {
}

Startable::~Startable() {
}

bool Startable::startImpl() {
    return true;
}

bool Startable::start() {
    if (m_started)
        return true;

    m_started = startImpl();
    return m_started;
}

bool Startable::started() const {
    return m_started;
}

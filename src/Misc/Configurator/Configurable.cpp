#include "Configurable.h"

Configurable::Configurable()
    : m_configured(false) {
}

Configurable::~Configurable() {
}

bool Configurable::configureImpl() {
    return true;
}

bool Configurable::configure() {
    if (m_configured)
        return true;

    m_configured = configureImpl();
    return m_configured;
}

bool Configurable::configured() const {
    return m_configured;
}

#include "Configurator.h"
#include "Utils/Utils.h"

Configurator::~Configurator() {
    clear();
}

bool Configurator::configureImpl() {
    bool res = false;

    for (int i = 0 ; i < (int) m_items.size() ; i++)
        res &= m_items[i]->configure();

    return res;
}

Configurable *Configurator::item(int ind) const {
    return m_items[ind];
}

void Configurator::addItem(Configurable *it) {
    m_items.push_back(it);
}

int Configurator::itemsCount() const {
    return m_items.size();
}

void Configurator::clear() {
    Utils::clearPtrContainer(m_items);
}


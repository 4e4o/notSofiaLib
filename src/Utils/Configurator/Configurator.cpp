#include "Configurator.h"
#include "Utils/Utils.h"

Configurator::~Configurator() {
    clear();
}

bool Configurator::configureImpl() {
    for (int i = 0 ; i < (int) m_items.size() ; i++) {
        if (!m_items[i]->configure())
            return false;
    }

    return true;
}

bool Configurator::startImpl() {
    if (!configured())
        return false;

    for (int i = 0 ; i < (int) m_items.size() ; i++) {
        if (!m_items[i]->start())
            return false;
    }

    return true;
}

Configurable* Configurator::item(int ind) const {
    return m_items[ind];
}

void Configurator::addItem(Configurable *it) {
    m_items.push_back(it);
}

int Configurator::itemsCount() const {
    return m_items.size();
}

void Configurator::clear() {
    // удаляем с конца
    // потому что добавлялись элементы в конец
    // это важно для MPP например
    Utils::clearPtrContainer(m_items, true);
}


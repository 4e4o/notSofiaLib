#include "Configurator.h"
#include "Utils/Utils.h"

Configurator::~Configurator() {
    clear();
}

bool Configurator::configureImpl() {
    for (auto& item : m_items) {
        if (!item->configure())
            return false;
    }

    return true;
}

bool Configurator::startImpl() {
    if (!configured())
        return false;

    for (auto& item : m_items) {
        if (!item->start())
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


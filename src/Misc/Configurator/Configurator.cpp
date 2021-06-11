#include "Configurator.h"
#include "Misc/Utils.h"

Configurator::~Configurator() {
    clear();
}

bool Configurator::configureImpl() {
    for (auto &item : m_items) {
        if (!item->configure())
            return false;
    }

    return true;
}

void Configurator::addItemBack(Configurable *it) {
    m_items.push_back(it);
}

void Configurator::addItemFront(Configurable *it) {
    m_items.push_front(it);
}

void Configurator::clear() {
    // удаляем с конца
    // потому что добавлялись элементы в конец
    // это важно для MPP например
    Utils::clearPtrContainer(m_items, true);
}


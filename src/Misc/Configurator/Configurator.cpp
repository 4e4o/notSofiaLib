#include "Configurator.h"
#include "Misc/Utils.h"

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
    auto it1 = std::next(m_items.begin(), ind);
    return *it1;
}

void Configurator::addItemBack(Configurable *it) {
    m_items.push_back(it);
}

void Configurator::addItemFront(Configurable* it) {
    m_items.push_front(it);
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


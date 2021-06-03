#include "AConfigurator.h"
#include "Utils/Utils.h"

AConfigurator::~AConfigurator() {
    clear();
}

bool AConfigurator::configure() {
    bool res = false;

    for (int i = 0 ; i < (int) m_items.size() ; i++)
        res &= m_items[i]->configure();

    return res;
}

IConfigurable *AConfigurator::item(int ind) const {
    return m_items[ind];
}

void AConfigurator::addItem(IConfigurable *it) {
    m_items.push_back(it);
}

int AConfigurator::itemsCount() const {
    return m_items.size();
}

void AConfigurator::clear() {
    Utils::clearPtrContainer(m_items);
}


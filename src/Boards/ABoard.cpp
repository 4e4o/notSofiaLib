#include "ABoard.h"
#include "Utils/Utils.h"

namespace boards {

ABoard::~ABoard() {
    Utils::clearPtrContainer(m_items);
}

bool ABoard::configure() {
    bool res = false;

    for (int i = 0 ; i < (int) m_items.size() ; i++)
        res &= m_items[i]->configure();

    return res;
}

IConfigurable *ABoard::item(int ind) const {
    return m_items[ind];
}

void ABoard::addItem(IConfigurable *it) {
    m_items.push_back(it);
}

int ABoard::itemsCount() const {
    return m_items.size();
}


}

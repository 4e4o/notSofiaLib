#ifndef A_BOARD_H
#define A_BOARD_H

#include <vector>

#include "Utils/IConfigurable.h"

namespace boards {

class ABoard : public IConfigurable {
public:
    ~ABoard();

    bool configure();
    IConfigurable* item(int = 0) const;

protected:
    void addItem(IConfigurable*);
    int itemsCount() const;

private:
    std::vector<IConfigurable*> m_items;
};

}

#endif // A_BOARD_H

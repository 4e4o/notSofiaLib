#ifndef A_CONFIGURATOR_H
#define A_CONFIGURATOR_H

#include <vector>

#include "IConfigurable.h"

class AConfigurator : public IConfigurable {
public:
    ~AConfigurator();

    bool configure();
    IConfigurable* item(int = 0) const;

protected:
    void addItem(IConfigurable*);
    int itemsCount() const;
    void clear();

private:
    std::vector<IConfigurable*> m_items;
};

#endif // A_CONFIGURATOR_H

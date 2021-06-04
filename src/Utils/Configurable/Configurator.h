#ifndef A_CONFIGURATOR_H
#define A_CONFIGURATOR_H

#include <vector>

#include "Configurable.h"

class Configurator : public Configurable {
public:
    ~Configurator();

    Configurable* item(int = 0) const;

protected:
    bool configureImpl();

    void addItem(Configurable*);
    int itemsCount() const;
    void clear();

private:
    std::vector<Configurable*> m_items;
};

#endif // A_CONFIGURATOR_H

#ifndef A_CONFIGURATOR_H
#define A_CONFIGURATOR_H

#include <list>

#include "Configurable.h"

class Configurator : public Configurable {
public:
    ~Configurator();

    // TODO remove it
    Configurable* item(int = 0) const;

protected:
    bool configureImpl() override;
    bool startImpl() override;

    void addItem(Configurable*);
    void addItemFront(Configurable*);
    int itemsCount() const;
    void clear();

private:
    std::list<Configurable*> m_items;
};

#endif // A_CONFIGURATOR_H

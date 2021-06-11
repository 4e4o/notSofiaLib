#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <list>

#include "Configurable.h"

class Configurator : public Configurable {
  public:
    ~Configurator();

  protected:
    bool configureImpl() override;
    bool startImpl() override;

    void addItemFront(Configurable *);
    void addItemBack(Configurable *);

    void clear();

  private:
    std::list<Configurable *> m_items;
};

#endif // CONFIGURATOR_H

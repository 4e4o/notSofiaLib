#ifndef APPLICATION_H
#define APPLICATION_H

#include "Misc/AApplication.h"

class Application : public AApplication {
  public:
    Application(int count = 0, char **argv = nullptr);
};

#endif // APPLICATION_H

#ifndef AAPPLICATION_H
#define AAPPLICATION_H

#include <memory>

#include "Boards/ABoard.h"
#include "Misc/GenericFactory.h"

#define AAPP (AApplication::app())

// Абстрактное приложение

class AApplication : public GenericFactory<boards::ABoard> {
  public:
    AApplication();
    virtual ~AApplication();

    virtual int run();
    void stop();

    static AApplication *app();

  private:
    void setExit();

    std::unique_ptr<boards::ABoard> m_board;
    static AApplication *g_app;
};

#endif // AAPPLICATION_H

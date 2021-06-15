#ifndef AAPPLICATION_H
#define AAPPLICATION_H

#include <memory>
#include <span>

#include "Boards/ABoard.h"
#include "Misc/GenericFactory.h"

#define AAPP (AApplication::app())

// Абстрактное приложение

class AApplication : public GenericFactory<boards::ABoard> {
  public:
    typedef std::span<char> TArg;
    typedef std::vector<TArg> TArgs;

    AApplication(int count = 0, char **argv = nullptr);
    virtual ~AApplication();

    virtual int run();
    void stop();

    static AApplication *app();

    boards::ABoard *board() const;

    const TArgs &args() const;

  protected:
    virtual void beforeBoardRun();

  private:
    void setExit();

    std::unique_ptr<boards::ABoard> m_board;
    TArgs m_args;

    static AApplication *g_app;
};

#endif // AAPPLICATION_H

#ifndef AAPPLICATION_H
#define AAPPLICATION_H

#include <memory>
#include <string_view>

#include "Boards/ABoard.h"
#include "Misc/GenericFactory.h"

#define AAPP (AApplication::app())

// Абстрактное приложение

class AApplication : public GenericFactory<boards::ABoard> {
  public:
    typedef std::string_view TArg;
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

    TArgs m_args;
    bool m_stopped;
    std::unique_ptr<boards::ABoard> m_board;

    static AApplication *g_app;
};

#endif // AAPPLICATION_H

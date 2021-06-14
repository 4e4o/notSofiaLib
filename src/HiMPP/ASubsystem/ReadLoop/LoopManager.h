#ifndef MPP_LOOP_MANAGER_H
#define MPP_LOOP_MANAGER_H

#include <thread>
#include <vector>

namespace hisilicon::mpp {

class ReadLoop;

class LoopManager {
  public:
    LoopManager();
    ~LoopManager();

    void setLoopsCount(int loopsCount);
    ReadLoop *assignLoop();

    void run();
    void stop();
    void join();

  protected:
    void createLoops();

  private:
    std::vector<ReadLoop *> m_readLoops;
    std::vector<std::thread *> m_threads;
    int m_assignLoopIndex;
};

}

#endif // MPP_LOOP_MANAGER_H

#ifndef NVP_6134_MOTION_LOOP_H
#define NVP_6134_MOTION_LOOP_H

#include <map>
#include <mutex>

#include "Misc/EventLoop/EventLoop.h"
#include "Misc/Holder.h"

class Timer;

namespace nvp6134 {

class DriverCommunicator;

class MotionLoop : public EventLoop,
    public Holder<DriverCommunicator *> {
  public:
    typedef std::pair<int, int> TChipChannel;
    typedef std::function<void()> TMotionEvent;

    MotionLoop(DriverCommunicator *);
    ~MotionLoop();

    void watch(TChipChannel, TMotionEvent);
    void remove(TChipChannel);

  private:
    void onTick();

    typedef std::map<TChipChannel, TMotionEvent> TChannels;

    TChannels m_channels;
    std::mutex m_channelsMutex;
    std::unique_ptr<Timer> m_readTimer;
};

}

#endif // NVP_6134_MOTION_LOOP_H

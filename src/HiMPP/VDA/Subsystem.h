#ifndef MPP_VDA_SUBSYSTEM_H
#define MPP_VDA_SUBSYSTEM_H

#include "HiMPP/ASubsystem/ASubsystem.h"
#include "HiMPP/VPSS/Binder/ConfiguratorBinder.h"

namespace hisilicon::mpp::vda {

class Channel;
class MotionChannel;

class Subsystem : public ASubsystem<vpss::ConfiguratorBinder, Channel> {
  public:
    Subsystem(MPP *);
    ~Subsystem();

    MotionChannel *addMotionChannel(int id);
    const std::vector<Channel *> &channels() const;

  protected:
    void registerDefaultTypes();
};

}

#endif // MPP_VDA_SUBSYSTEM_H

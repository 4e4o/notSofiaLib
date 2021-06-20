#ifndef MPP_VDA_MOTION_CHANNEL_H
#define MPP_VDA_MOTION_CHANNEL_H

#include "HiMPP/VDA/Channel/Channel.h"

namespace hisilicon::mpp::vda {

class MotionReader;
class MotionAttributes;

class MotionChannel : public Channel {
  public:
    MotionChannel(Subsystem *, int id);
    ~MotionChannel();

    MotionReader *motionReader() const;
    MotionAttributes *attributes() const;
};

}

#endif // MPP_VDA_MOTION_CHANNEL_H

#ifndef MPP_VDA_MOTION_ATTRIBUTES_H
#define MPP_VDA_MOTION_ATTRIBUTES_H

#include "HiMPP/VDA/Channel/Attributes.h"

namespace hisilicon::mpp::vda {

class MotionAttributes : public ChannelAttributes {
  public:
    MotionAttributes();

    MPP_ATTRIBUTE(Bpp, float);
  private:
    void buildForMode(VDA_CHN_ATTR_S *, IVideoFormatSource *source) override final;
};

}

#endif // MPP_VDA_MOTION_ATTRIBUTES_H

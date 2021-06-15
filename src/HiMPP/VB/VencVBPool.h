#ifndef MPP_VENC_VB_POOL_H
#define MPP_VENC_VB_POOL_H

#include "HiMPP/VB/VBPool.h"

namespace hisilicon::mpp::venc {

class VBPool : public mpp::VBPool {
  public:
    VBPool(MPP *);

  private:
    void init();
    HI_U32 maxBlkSize(int &channelsCount);
};

}

#endif // MPP_VENC_VB_POOL_H

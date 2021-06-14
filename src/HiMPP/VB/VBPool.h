#ifndef MPP_VB_POOL_H
#define MPP_VB_POOL_H

#include <hi_comm_vb.h>

#include "VBBase.h"

namespace hisilicon::mpp {

class VBPool : public VBBase {
  public:
    VBPool(MPP *);

    ~VBPool();

    void initForVenc();

    void setBlockSize(const HI_U32 &blockSize);
    void setBlockCount(const HI_U32 &blockCount);
    void setName(const std::string &name);

    VB_POOL id() const;

  private:
    bool configureImpl() override final;
    HI_U32 maxVencBlkSize(int &);

    HI_U32 m_blockSize;
    HI_U32 m_blockCount;
    std::string m_name;
    VB_POOL m_id;
};

}

#endif // MPP_VB_POOL_H

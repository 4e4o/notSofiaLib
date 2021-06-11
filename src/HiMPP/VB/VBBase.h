#ifndef MPP_VB_BASE_H
#define MPP_VB_BASE_H

#include <hi_type.h>

#include "HiMPP/MPPChild.h"
#include "Misc/Configurator/Configurable.h"

namespace hisilicon::mpp {

class IVBufferizable;

class VBBase : public MPPChild, public Configurable {
  public:
    using MPPChild::MPPChild;

  protected:
    HI_U32 picVbBlkSize(IVBufferizable *b);

    // дополнительное кол-во блоков для выделения в пуле
    // помогает найти нестабильные конфиги пула
    // если в cat /proc/umap/vb MinFree будет равен нулю, а не EXTRA_BLOCKS_COUNT
    // - значит конфиг не стабилен и требуется больше блоков
    static constexpr int EXTRA_BLOCKS_COUNT = 1;
};

}

#endif // MPP_VB_BASE_H

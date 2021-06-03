#ifndef MPP_H
#define MPP_H

#include <memory>

#include <hi_type.h>

#include "Utils/IConfigurable.h"

namespace hisilicon {
namespace mpp {

class ViInfoProvider;
class VideoBuffer;

class MPP : public IConfigurable {
public:
    MPP(ViInfoProvider*);
    ~MPP();

    ViInfoProvider* viSourceInfo() const;

    void setSysWidthAlign(HI_U32);
    HI_U32 sysWidthAlign() const;

    bool configure();

private:
    void init();

    std::unique_ptr<ViInfoProvider> m_sourceViInfo;
    HI_U32 m_sysWidthAlign;
    std::unique_ptr<VideoBuffer> m_videoBuffer;
};

}
}

#endif // MPP_H

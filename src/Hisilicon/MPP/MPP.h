#ifndef MPP_H
#define MPP_H

#include <memory>

#include <hi_type.h>

#include "Utils/IConfigurable.h"

namespace hisilicon {
namespace mpp {

class ViInfoProvider;
class VideoBuffer;
class ViSubsystem;
class ElementsFactory;

class MPP : public IConfigurable {
public:
    MPP(ViInfoProvider*);
    ~MPP();

    ViInfoProvider* viSourceInfo() const;

    void setSysWidthAlign(HI_U32);
    HI_U32 sysWidthAlign() const;

    bool configure();

    ElementsFactory* factory() const;
    void setFactory(ElementsFactory* factory);

    ViSubsystem* vi() const;

private:
    void init();

    std::unique_ptr<ViInfoProvider> m_sourceViInfo;
    HI_U32 m_sysWidthAlign;
    std::unique_ptr<VideoBuffer> m_videoBuffer;
    std::unique_ptr<ViSubsystem> m_vi;
    std::unique_ptr<ElementsFactory> m_factory;
};

}
}

#endif // MPP_H

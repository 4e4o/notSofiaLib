#ifndef MPP_H
#define MPP_H

#include <memory>

#include <hi_type.h>

#include "Utils/Configurable/Configurable.h"

namespace hisilicon {
namespace mpp {

namespace vi{
class InfoProvider;
class Subsystem;
}

class VideoBuffer;
class ElementsFactory;

class MPP : public Configurable {
public:
    MPP(ElementsFactory*);
    ~MPP();

    vi::InfoProvider* viSourceInfo() const;

    void setSysWidthAlign(HI_U32);
    HI_U32 sysWidthAlign() const;

    ElementsFactory* factory() const;

    vi::Subsystem* vi() const;

private:
    bool configureImpl();
    void init();

    std::unique_ptr<vi::InfoProvider> m_sourceViInfo;
    HI_U32 m_sysWidthAlign;
    std::unique_ptr<VideoBuffer> m_videoBuffer;
    std::unique_ptr<vi::Subsystem> m_vi;
    std::unique_ptr<ElementsFactory> m_factory;
};

}
}

#endif // MPP_H

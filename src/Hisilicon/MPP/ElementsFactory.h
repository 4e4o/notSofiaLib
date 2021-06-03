#ifndef MPP_ELEMENTS_FACTORY_H
#define MPP_ELEMENTS_FACTORY_H

namespace hisilicon {
namespace mpp {

class MPP;
class VideoBuffer;
class ViSubsystem;
class ViDevice;
class ViChannel;

class ElementsFactory {
public:
    ElementsFactory();
    ~ElementsFactory();

    virtual VideoBuffer* videoBuffer(MPP*);
    virtual ViSubsystem* vi(MPP*);
    virtual ViDevice* viDevice(MPP*, int id);
    virtual ViChannel* viChannel(MPP*, ViDevice*, int id);


};

}
}

#endif // MPP_ELEMENTS_FACTORY_H

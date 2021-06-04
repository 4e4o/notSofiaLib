#ifndef MPP_ELEMENTS_FACTORY_H
#define MPP_ELEMENTS_FACTORY_H

namespace hisilicon {
namespace mpp {

namespace vi {
class Subsystem;
class Device;
class Channel;
}

class MPP;
class VideoBuffer;

class ElementsFactory {
public:
    ElementsFactory();
    ~ElementsFactory();

    virtual VideoBuffer* videoBuffer(MPP*);
    virtual vi::Subsystem* vi(MPP*);
    virtual vi::Device* viDevice(MPP*, int id);
    virtual vi::Channel* viChannel(MPP*, vi::Device*, int id);


};

}
}

#endif // MPP_ELEMENTS_FACTORY_H

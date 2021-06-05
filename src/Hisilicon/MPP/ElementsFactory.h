#ifndef MPP_ELEMENTS_FACTORY_H
#define MPP_ELEMENTS_FACTORY_H

namespace hisilicon {
namespace mpp {

namespace vi {
class Subsystem;
class Device;
class Channel;
class InfoProvider;
class ChannelInfo;
}

class MPP;
class VideoBuffer;

class ElementsFactory {
public:
    ElementsFactory();
    ~ElementsFactory();

    virtual VideoBuffer* videoBuffer(MPP*);
    virtual vi::InfoProvider* viInfoProvider() = 0;
    virtual vi::Subsystem* vi(MPP*);
    virtual vi::Device* viDevice(vi::Subsystem*, int id);
    virtual vi::Channel* viChannel(vi::Device*, mpp::vi::ChannelInfo*, int id);


};

}
}

#endif // MPP_ELEMENTS_FACTORY_H

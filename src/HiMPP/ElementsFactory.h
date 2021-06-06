#ifndef MPP_ELEMENTS_FACTORY_H
#define MPP_ELEMENTS_FACTORY_H

namespace hisilicon::mpp {

namespace vi {
class Subsystem;
class Device;
class Channel;
class InfoProvider;
class ChannelInfo;
}

namespace vpss {
class Group;
class Channel;
class Subsystem;
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

    virtual vpss::Subsystem* vpss(MPP*);
    virtual vpss::Group* vpssGroup(vpss::Subsystem*, int id);
    virtual vpss::Channel* vpssChannel(vpss::Group*, int id);
};

}

#endif // MPP_ELEMENTS_FACTORY_H

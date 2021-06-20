#ifndef MPP_VI_INFO_PROVIDER_H
#define MPP_VI_INFO_PROVIDER_H

#include <map>
#include <memory>

#include "Misc/Configurator/Configurable.h"

namespace hisilicon::mpp::vi {

class ChannelInfo;

// Задача класса -
// Передавать инфу каналов из источника в vi

class InfoProvider : public Configurable {
  public:
    typedef std::pair<int, int> TChannelId;

    InfoProvider();
    ~InfoProvider();

    const ChannelInfo *getInfo(const TChannelId &) const;

  protected:
    void addChannel(const TChannelId &inputId, const ChannelInfo *);
    virtual TChannelId inputIdToVi(const TChannelId &) = 0;

  private:
    typedef std::map<TChannelId, const ChannelInfo *> TChannels;
    TChannels m_channels;
};

}

#endif // MPP_VI_INFO_PROVIDER_H

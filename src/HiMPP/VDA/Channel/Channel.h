#ifndef MPP_VDA_CHANNEL_H
#define MPP_VDA_CHANNEL_H

#include <memory>

#include "HiMPP/ASubsystem/ASubsystemLeaf.h"
#include "HiMPP/VPSS/Binder/BindItem.h"

namespace hisilicon::mpp {
class IVideoFormatSource;
}

namespace hisilicon::mpp::vda {

class Subsystem;
class Channel;
class ChannelAttributes;
class VdaDataReader;

class Channel : public ASubsystemLeaf<Subsystem>, public vpss::VdaBindItem {
  public:
    Channel(ChannelAttributes *, VdaDataReader *, Subsystem *, int id);
    ~Channel();

    template<class TReader = VdaDataReader>
    TReader * vdaReader() const {
        return static_cast<TReader *>(m_vdaReader.get());
    }

    template<class TAttributes = ChannelAttributes>
    TAttributes * attributes() const {
        return static_cast<TAttributes *>(m_attrBuilder.get());
    }

    const Subsystem *subsystem() const;

  private:
    bool configureImpl() override final;
    void setBindedItem(BindItem *bi, bool source) override final;

    std::unique_ptr<ChannelAttributes> m_attrBuilder;
    std::unique_ptr<VdaDataReader> m_vdaReader;
    IVideoFormatSource *m_source;
};

}

#endif // MPP_VDA_CHANNEL_H

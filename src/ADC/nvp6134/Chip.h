#ifndef NVP_6134_CHIP_H
#define NVP_6134_CHIP_H

#include <memory>

#include <nvp6134_ex_170306/video.h>

#include "Utils/IdHolder.h"
#include "Utils/IConfigurable.h"
#include "ChipSpecs.h"

#include <array>

namespace nvp6134 {

class ViChannel;
class VoChannel;
class DriverCommunicator;

// Тут прикольная анимация как работает bt656 от adc к hisilicon'у
// и вообще принципы описаны
// https://www.programmersought.com/article/35434438458/

// single nvp6134 chip

class Chip : public IdHolder, public IConfigurable {
public:
    typedef std::array<std::unique_ptr<ViChannel>, ChipSpecs::CS_VI_CHANNELS_COUNT> TViChannels;
    typedef std::array<std::unique_ptr<VoChannel>, ChipSpecs::CS_VO_CHANNELS_COUNT> TVoChannels;

    // id - device index from nvp driver [0, 3]
    Chip(DriverCommunicator*, int id);
    ~Chip();

    DriverCommunicator* driver() const;

    TViChannels& viChannels();
    TVoChannels& voChannels();

    bool configure();

    virtual NVP6134_VI_MODE getViChannelMode(ViChannel*);
    virtual NVP6134_OUTMODE_SEL getVoChannelMode(VoChannel*);

private:
    void init();

    DriverCommunicator *m_driver;
    TViChannels m_viChannels;
    TVoChannels m_voChannels;
};

}

#endif // NVP_6134_CHIP_H

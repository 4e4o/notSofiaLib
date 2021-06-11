#include "VoChannel.h"
#include "DriverCommunicator.h"
#include "Chip.h"
#include "ViChannel.h"

#include <stdexcept>
#include <iostream>

namespace nvp6134 {

VoChannel::VoChannel(Chip *p, int id)
    : ChipChild(p), IdHolder(id),
      m_mode(NVP6134_OUTMODE_BUTT) {
}

VoChannel::~VoChannel() {
}

bool VoChannel::is1MuxMode() const {
    switch (m_mode) {
    case NVP6134_OUTMODE_1MUX_SD:
    case NVP6134_OUTMODE_1MUX_HD:
    case NVP6134_OUTMODE_1MUX_HD5060:
    case NVP6134_OUTMODE_1MUX_FHD:
    case NVP6134_OUTMODE_1MUX_HD_X:
    case NVP6134_OUTMODE_1MUX_FHD_X:
    case NVP6134_OUTMODE_1MUX_BT1120S:
    case NVP6134_OUTMODE_1MUX_3M_RT:
    case NVP6134_OUTMODE_1MUX_4M_NRT:
        return true;
    default:
        return false;
    }
}

bool VoChannel::is2MuxMode() const {
    switch (m_mode) {
    case NVP6134_OUTMODE_2MUX_SD:
    case NVP6134_OUTMODE_2MUX_HD_X:
    case NVP6134_OUTMODE_2MUX_HD:
    case NVP6134_OUTMODE_2MUX_FHD_X:
    case NVP6134_OUTMODE_2MUX_FHD:
    case NVP6134_OUTMODE_2MUX_MIX:
        return true;
    default:
        return false;
    }
}

bool VoChannel::isMixMode() const {
    switch (m_mode) {
    case NVP6134_OUTMODE_4MUX_MIX:
    case NVP6134_OUTMODE_2MUX_MIX:
        return true;
    default:
        return false;
    }
}

bool VoChannel::is4MuxMode() const {
    switch (m_mode) {
    case NVP6134_OUTMODE_4MUX_SD:
    case NVP6134_OUTMODE_4MUX_HD_X:
    case NVP6134_OUTMODE_4MUX_HD:
    case NVP6134_OUTMODE_4MUX_FHD_X:
    case NVP6134_OUTMODE_4MUX_MIX:
        return true;
    default:
        return false;
    }
}

bool VoChannel::setMode(unsigned char chid, NVP6134_OUTMODE_SEL mode) {
    m_mode = mode;

    if (is1MuxMode()) {
        parent()->viChannels()[chid]->setOutChannel(this);
    } else if (is2MuxMode()) {
        // for 0 vo - 0, 1 channels
        // for 1 vo - 2, 3 channels
        // indexes is from nvp driver
        auto &ch = parent()->viChannels();

        if (id() == 0) {
            for (int i = 0 ; i < 2 ; i++)
                ch[i]->setOutChannel(this);
        } else {
            for (int i = 2 ; i < 4 ; i++)
                ch[i]->setOutChannel(this);
        }
    } else if (is4MuxMode()) {
        auto &ch = parent()->viChannels();

        for (int i = 0 ; i < 4 ; i++)
            ch[i]->setOutChannel(this);
    } else {
        throw std::runtime_error("Unknown mode");
    }

    return parent()->driver()->setVoChannelMode(this, chid, mode);
}

}

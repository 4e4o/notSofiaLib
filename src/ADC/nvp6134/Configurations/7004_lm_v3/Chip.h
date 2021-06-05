#ifndef LM7004_V3_NVP_6134_CHIP_H
#define LM7004_V3_NVP_6134_CHIP_H

#include "ADC/nvp6134/Chip.h"

namespace nvp6134 {
namespace lm7004v3 {

// Это чип на плате LM-7004-V3.
// содержит конфигурацию чипа на плате.

class Chip : public ::nvp6134::Chip {
public:
    Chip(::nvp6134::DriverCommunicator*, int id);
    ~Chip();

private:
    NVP6134_VI_MODE getViChannelMode(::nvp6134::ViChannel*);
    NVP6134_OUTMODE_SEL getVoChannelMode(::nvp6134::VoChannel*);
};

}
}

#endif // LM7004_V3_NVP_6134_CHIP_H

#include "Chip.h"
#include "ADC/nvp6134/ViChannel.h"

namespace nvp6134 {
namespace lm7004v3 {

Chip::Chip(DriverCommunicator *d, int id)
    : ::nvp6134::Chip(d, id) {
}

Chip::~Chip() {
}

NVP6134_VI_MODE Chip::getViChannelMode(ViChannel* ch) {
    switch(ch->videoFormat()) {
    case ViChannel::DF_CVBS_NTSC:
    case ViChannel::DF_CVBS_PAL:
        return NVP6134_VI_1440H; // этот формат юзаем мы, самый меньший который работал
        //      return NVP6134_VI_1920H; // этот формат юзает София, почему? Непонятно.
        //      return NVP6134_VI_1280H; // этот формат работает, но гличи на изображении не юзабельные
        //      return NVP6134_VI_960H; // этот не пашет
        //      return NVP6134_VI_720H; // этот не пашет, я бы по логике выбрал бы этот
    case ViChannel::DF_1080P_NTSC:
        return NVP6134_VI_1080P_2530;
    default:
        return NVP6134_VI_1080P_NOVIDEO;
    }
}

NVP6134_OUTMODE_SEL Chip::getVoChannelMode(::nvp6134::VoChannel*) {
    // Может быть такая логика это частный случай моего сетапа, хз...
    return NVP6134_OUTMODE_2MUX_MIX;

    // NOTE может быть здесь такая логика:
    // выбираем режим исходя из формата каналов
    // mix/не mix исходя из пункта 2.11 страница 25 nvp даташита
    // mux/не mux исходя из наличия сигналов на vi каналах

    // NOTE HiMPP Media Processing Software Development Reference.pdf
    // page 121
}

}
}

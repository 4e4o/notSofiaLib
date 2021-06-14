#include "MPP.h"
#include "Sys/Sys.h"
#include "VB/VideoBuffer.h"
#include "VI/Subsystem.h"
#include "VPSS/Subsystem.h"
#include "VENC/Subsystem.h"

#include <stdexcept>

namespace hisilicon::mpp {

MPP::MPP()
    : m_sys(nullptr),
      m_vb(nullptr),
      m_vi(nullptr),
      m_vpss(nullptr),
      m_venc(nullptr),
      m_veduCount(0) {
    registerDefaultTypes();
}

void MPP::registerDefaultTypes() {
    registerType([] (MPP * p) -> VideoBuffer* {
        return new VideoBuffer(p);
    }, false);
    registerType([] (MPP * p) -> Sys* {
        return new Sys(p);
    }, false);
}

bool MPP::configureImpl() {
    if (m_veduCount == 0)
        // HiMPP Media Processing Software Development Reference.pdf
        // page 512
        throw std::runtime_error("You must set veduCount for your hi chip");

    createLoops();

    // Здесь порядок важен
    addSubsystem<Sys, false>(m_sys);
    addSubsystem<VideoBuffer, false>(m_vb);

    return Configurator::configureImpl();
}

vi::Subsystem *MPP::addViSubsystem() {
    return addSubsystem<vi::Subsystem>(m_vi);
}

vpss::Subsystem *MPP::addVpssSubsystem() {
    return addSubsystem<vpss::Subsystem>(m_vpss);
}

venc::Subsystem *MPP::addVencSubsystem() {
    return addSubsystem<venc::Subsystem>(m_venc);
}

Sys *MPP::sys() const {
    return m_sys;
}

vi::Subsystem *MPP::vi() const {
    return m_vi;
}

vpss::Subsystem *MPP::vpss() const {
    return m_vpss;
}

venc::Subsystem *MPP::venc() const {
    return m_venc;
}

int MPP::veduCount() const {
    return m_veduCount;
}

void MPP::setVeduCount(int veduCount) {
    m_veduCount = veduCount;
}

}

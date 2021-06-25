#include "Motion.h"
#include "Chip.h"
#include "ViChannel.h"
#include "DriverCommunicator.h"

#include <stdexcept>

namespace nvp6134 {

Motion::Motion(ViChannel *c)
    : Holder<ViChannel *>(c),
      m_enabled(false),
      m_sensitivity(Sensitivity::MEDIUM),
      m_visualize(false) {
    // enable all block by default
    setAreaAll(true);
}

Motion::~Motion() {
}

bool Motion::enabled() const {
    return m_enabled;
}

void Motion::setEnabled(bool enabled) {
    m_enabled = enabled;

    auto *driver = value()->parent()->driver();
    driver->setViMotion(value());
}

Motion::Sensitivity Motion::sensitivity() const {
    return m_sensitivity;
}

void Motion::setSensitivity(const Sensitivity &sensitivity) {
    m_sensitivity = sensitivity;
}

void Motion::setAreaBlock(uint8_t row, uint8_t column, bool enabled) {
    if ((row >= AREA_HEIGHT) || (column >= AREA_WIDTH))
        throw std::runtime_error("Invalid range");

    m_area[row].at(column) = enabled;
}

void Motion::setAreaRow(uint8_t row, bool enabled) {
    if (row >= AREA_HEIGHT)
        throw std::runtime_error("Invalid range");

    m_area.at(row).fill(enabled);
}

void Motion::setAreaColumn(uint8_t column, bool enabled) {
    if (column >= AREA_WIDTH)
        throw std::runtime_error("Invalid range");

    for (int i = 0 ; i < AREA_HEIGHT ; i++)
        setAreaBlock(i, column, enabled);
}

void Motion::setAreaAll(bool enabled) {
    for (int i = 0 ; i < AREA_HEIGHT ; i++)
        setAreaRow(i, enabled);
}

const Motion::TArea &Motion::area() const {
    return m_area;
}

bool Motion::visualize() const {
    return m_visualize;
}

void Motion::setVisualize(bool visualize) {
    m_visualize = visualize;
}

Motion::TMotionEvent Motion::event() const {
    return m_event;
}

void Motion::setEvent(const TMotionEvent &event) {
    m_event = event;
}

ViChannel *Motion::channel() const {
    return value();
}

}

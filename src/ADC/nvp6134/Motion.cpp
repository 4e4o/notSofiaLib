#include "Motion.h"
#include "Chip.h"
#include "ViChannel.h"
#include "DriverCommunicator.h"

#include <stdexcept>

#define DEFAULT_TEMPORAL_SENS       TemporalSensitivity::MEDIUM
#define DEFAULT_PIXEL_SENS          PixelSensitivity::PS_1S8
#define DEFAULT_BRIGHTNESS_SENS     0x00

namespace nvp6134 {

Motion::Motion(ViChannel *c)
    : Holder<ViChannel *>(c),
      m_enabled(false),
      m_pixelSensitivity(DEFAULT_PIXEL_SENS),
      m_brightessSensitivity(DEFAULT_BRIGHTNESS_SENS),
      m_visualize(false) {
    setTemporalSensitivity(DEFAULT_TEMPORAL_SENS);
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

u_int8_t Motion::temporalSensitivity() const {
    return m_temporalSensitivity;
}

void Motion::setTemporalSensitivity(const TemporalSensitivity &s) {
    m_temporalSensitivity = static_cast<uint8_t>(s);
}

void Motion::setTemporalSensitivity(const uint8_t &s) {
    m_temporalSensitivity = s;
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

Motion::PixelSensitivity Motion::pixelSensitivity() const {
    return m_pixelSensitivity;
}

void Motion::setPixelSensitivity(const PixelSensitivity &pixelSensitivity) {
    m_pixelSensitivity = pixelSensitivity;
}

uint8_t Motion::brightessSensitivity() const {
    return m_brightessSensitivity;
}

void Motion::setBrightessSensitivity(const uint8_t &brightessSensitivity) {
    m_brightessSensitivity = brightessSensitivity;
}

}

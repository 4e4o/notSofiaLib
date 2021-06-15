#include "Application.h"
#include "Boards/7004_lm_v3/Board.h"

Application::Application(int count, char **argv)
    : AApplication(count, argv) {
    registerType<::boards::ABoard, boards::lm7004v3::Board>();
}

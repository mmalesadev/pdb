#pragma once
#include <gainput/gainput.h>
#include <memory>
#include <unordered_map>

namespace Pdb
{

class InputManager
{
public:
    enum Button { REWIND_BUTTON, PLAY_BUTTON, PAUSE_BUTTON, REWIND_FORWARD_BUTTON };

    InputManager();
    void init();
    void update();
    bool isButtonPressed(Button button);
private:
    gainput::InputManager manager_;
    gainput::DeviceId keyboardId_;
    std::unique_ptr<gainput::InputMap> map_;
};

}
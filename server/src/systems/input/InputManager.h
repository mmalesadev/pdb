#pragma once
#include <gainput/gainput.h>
#include <memory>
#include <unordered_map>

namespace Pdb
{

class InputManager
{
public:
    enum Button { BUTTON_Q, BUTTON_W, BUTTON_E, BUTTON_S, BUTTON_X, BUTTON_UP, BUTTON_DOWN, BUTTON_F, BUTTON_T, BUTTON_D};

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
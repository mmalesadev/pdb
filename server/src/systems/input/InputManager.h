#pragma once
#include <gainput/gainput.h>
#include <memory>
#include <unordered_map>

namespace Pdb
{

class InputManager
{
public:
    InputManager();
    void init();
    void handlePressedKeys();
private:
    enum Button { BUTTON_Q };
    gainput::InputManager manager_;
    gainput::DeviceId keyboardId_;
    std::unique_ptr<gainput::InputMap> map_;
};

}
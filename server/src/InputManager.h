#ifndef _INPUT_MANAGER_
#define _INPUT_MANAGER_
#include <gainput/gainput.h>
#include <memory>

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

#endif

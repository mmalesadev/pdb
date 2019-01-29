#include "InputManager.h"
#include <iostream>

InputManager::InputManager()
{
    init();
}

void InputManager::init()
{
    std::cout << "Initializing InputManager.\n";
    
    keyboardId_ = manager_.CreateDevice<gainput::InputDeviceKeyboard>(gainput::InputDevice::AutoIndex, gainput::InputDevice::DV_STANDARD);

    gainput::InputDevice* device = manager_.GetDevice(keyboardId_);
    if (device->GetState() == gainput::InputDevice::DeviceState::DS_OK) 
        std::cout << "Keyboard FOUND. Id: " << device->GetDeviceId() << "\n";
    else if (device->GetState() == gainput::InputDevice::DS_UNAVAILABLE)
    {
        std::cout << "Keyboard UNAVAILABLE.\n";
        exit(0);
    }
    else
    {
        std::cout << "Keyboard NOT FOUND.\n";
        exit(0);
    }
    map_ = std::make_unique<gainput::InputMap>(manager_);

    map_->MapBool(Button::BUTTON_Q, keyboardId_, gainput::KeyReturn);
    std::cout << "Initialized InputManager.\n";

    gainput::DeviceButtonSpec buttonSpec;
    map_->GetMappings(Button::BUTTON_Q, &buttonSpec, 1);
    std::cout << buttonSpec.deviceId << " " << buttonSpec.buttonId << "\n";
}

void InputManager::handlePressedKeys()
{
    manager_.Update();
    
    // if (map_->GetBoolWasDown(Button::BUTTON_Q))
    // {
    //     std::cout << "Q pressed\n";
    //     //std::string milenaMessageCall = "milena_say Q pressed";
    //     //system(milenaMessageCall.c_str());
    // }
}

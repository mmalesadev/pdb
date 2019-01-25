#include "InputManager.h"
#include <iostream>

InputManager::InputManager()
{
    init();
}

void InputManager::init()
{
    std::cout << "Initializing InputManager.\n";
    keyboardId_ = manager_.CreateDevice<gainput::InputDeviceKeyboard>(gainput::InputDevice::AutoIndex, gainput::InputDevice::DV_RAW);
    gainput::InputDevice* device = manager_.GetDevice(keyboardId_);
    if (device->GetState() == gainput::InputDevice::DeviceState::DS_OK) std::cout << "Keyboard FOUND.\n";
    else
    {
        std::cout << "Keyboard NOT FOUND.\n";
        exit(0);
    }
    map_ = std::make_unique<gainput::InputMap>(manager_);

    map_->MapBool(Button::BUTTON_Q, keyboardId_, gainput::KeyQ);
    std::cout << "Initialized InputManager.\n";
}

void InputManager::handlePressedKeys()
{
    manager_.Update();
    if (map_->GetBoolWasDown(Button::BUTTON_Q))
    {
        std::cout << "Q pressed\n";
    }
}

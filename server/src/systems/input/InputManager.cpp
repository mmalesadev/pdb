#include "InputManager.h"
#include <iostream>
#include "spdlog/spdlog.h"

InputManager::InputManager()
{
    init();
}

void InputManager::init()
{
    spdlog::get("console")->info("Initializing InputManager.");
    
    keyboardId_ = manager_.CreateDevice<gainput::InputDeviceKeyboard>(gainput::InputDevice::AutoIndex, gainput::InputDevice::DV_STANDARD);

    gainput::InputDevice* device = manager_.GetDevice(keyboardId_);
    if (device->GetState() == gainput::InputDevice::DeviceState::DS_OK)
    { 
        spdlog::get("console")->info("Keyboard FOUND. Id: {}.", device->GetDeviceId());
    }
    else if (device->GetState() == gainput::InputDevice::DS_UNAVAILABLE)
    {
        spdlog::get("console")->info("Keyboard UNAVAILABLE.");
        exit(0);
    }
    else
    {
        spdlog::get("console")->info("Keyboard NOT FOUND.");
        exit(0);
    }
    map_ = std::make_unique<gainput::InputMap>(manager_);

    map_->MapBool(Button::BUTTON_Q, keyboardId_, gainput::KeyReturn);

    gainput::DeviceButtonSpec buttonSpec;
    map_->GetMappings(Button::BUTTON_Q, &buttonSpec, 1);
    spdlog::get("console")->info("Initialized InputManager.");
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

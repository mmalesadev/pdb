#include "InputManager.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

InputManager::InputManager()
{
    init();
}

void InputManager::init()
{
    BOOST_LOG_TRIVIAL(info) << "Initializing InputManager.";
    
    keyboardId_ = manager_.CreateDevice<gainput::InputDeviceKeyboard>(gainput::InputDevice::AutoIndex, gainput::InputDevice::DV_RAW);

    gainput::InputDevice* device = manager_.GetDevice(keyboardId_);
    if (device->GetState() == gainput::InputDevice::DeviceState::DS_OK)
    { 
        BOOST_LOG_TRIVIAL(info) << "Keyboard FOUND. Id: " << device->GetDeviceId() << ".";
    }
    else if (device->GetState() == gainput::InputDevice::DS_UNAVAILABLE)
    {
        BOOST_LOG_TRIVIAL(info) << "Keyboard UNAVAILABLE.";
        exit(0);
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "Keyboard NOT FOUND.";
        exit(0);
    }
    map_ = std::make_unique<gainput::InputMap>(manager_);

    map_->MapBool(Button::BUTTON_Q, keyboardId_, gainput::KeyQ);
    map_->MapBool(Button::BUTTON_W, keyboardId_, gainput::KeyW);
    map_->MapBool(Button::BUTTON_S, keyboardId_, gainput::KeyS);
    map_->MapBool(Button::BUTTON_E, keyboardId_, gainput::KeyE);
    map_->MapBool(Button::BUTTON_X, keyboardId_, gainput::KeyX);
    map_->MapBool(Button::BUTTON_UP, keyboardId_, gainput::KeyUp);
    map_->MapBool(Button::BUTTON_DOWN, keyboardId_, gainput::KeyDown);
    map_->MapBool(Button::BUTTON_F, keyboardId_, gainput::KeyF);
    map_->MapBool(Button::BUTTON_T, keyboardId_, gainput::KeyT);

    BOOST_LOG_TRIVIAL(info) << "Initialized InputManager.";
}

void InputManager::update()
{
    manager_.Update();
}

bool InputManager::isButtonPressed(Button button)
{
    return map_->GetBoolWasDown(button);
}

}
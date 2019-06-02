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
    map_->MapBool(Button::BUTTON_A, keyboardId_, gainput::KeyA);
    map_->MapBool(Button::BUTTON_S, keyboardId_, gainput::KeyS);
    map_->MapBool(Button::BUTTON_R, keyboardId_, gainput::KeyR);
    map_->MapBool(Button::BUTTON_E, keyboardId_, gainput::KeyE);
    map_->MapBool(Button::BUTTON_X, keyboardId_, gainput::KeyX);
    map_->MapBool(Button::BUTTON_UP, keyboardId_, gainput::KeyUp);
    map_->MapBool(Button::BUTTON_DOWN, keyboardId_, gainput::KeyDown);
    map_->MapBool(Button::BUTTON_F, keyboardId_, gainput::KeyF);
    map_->MapBool(Button::BUTTON_T, keyboardId_, gainput::KeyT);
    map_->MapBool(Button::BUTTON_D, keyboardId_, gainput::KeyD);
    map_->MapBool(Button::KeyKpEqual, keyboardId_, gainput::KeyKpEqual);
    map_->MapBool(Button::KeyKpDivide, keyboardId_, gainput::KeyKpDivide);
    map_->MapBool(Button::KeyKpMultiply, keyboardId_, gainput::KeyKpMultiply);
    map_->MapBool(Button::KeyKpSubtract, keyboardId_, gainput::KeyKpSubtract);
    map_->MapBool(Button::KeyKpAdd, keyboardId_, gainput::KeyKpAdd);
    map_->MapBool(Button::KeyKpEnter, keyboardId_, gainput::KeyKpEnter);
    map_->MapBool(Button::KeyKpInsert, keyboardId_, gainput::KeyKpInsert);
    map_->MapBool(Button::KeyKpEnd, keyboardId_, gainput::KeyKpEnd);
    map_->MapBool(Button::KeyKpDown, keyboardId_, gainput::KeyKpDown);
    map_->MapBool(Button::KeyKpPageDown, keyboardId_, gainput::KeyKpPageDown);
    map_->MapBool(Button::KeyKpLeft, keyboardId_, gainput::KeyKpLeft);
    map_->MapBool(Button::KeyKpBegin, keyboardId_, gainput::KeyKpBegin);
    map_->MapBool(Button::KeyKpRight, keyboardId_, gainput::KeyKpRight);
    map_->MapBool(Button::KeyKpHome, keyboardId_, gainput::KeyKpHome);
    map_->MapBool(Button::KeyKpUp, keyboardId_, gainput::KeyKpUp);
    map_->MapBool(Button::KeyKpPageUp, keyboardId_, gainput::KeyKpPageUp);
    map_->MapBool(Button::KeyKpDelete, keyboardId_, gainput::KeyKpDelete);

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
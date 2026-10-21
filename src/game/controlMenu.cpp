/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "controlMenu.hpp"


ControlMenu::ControlMenu(const Window& _window, float _X, float _Y, float _panelW, BoardInteracter& _interacter)
: Template(_window),
interacter(_interacter),
panelBackplate(_window, _panelW/2, 0.5, _panelW, 1.0, 2.0, GREY, BLACK),
modeText(_window, {"Show mode:", "Режим отображения:"}, {_panelW/2, 0.15, .frame=1}),
modeSwitchBox(_window, {_panelW/2, 0.185, .textColor = BLACK, .backColor = WHITE}, 0.14,
    {{"Elements", "Элементы"}, {"Thermal", "Температурный"}, {"Pressure", "Давление"}, {"Random", "Случайные"}}),
pickedPressure(_window, {"Pressure: %6.1f", "Давление: %6.1f"},
    {_panelW/4, 0.3, .frame=1, .height=GUI::Main, .horAli=GUI::Left}),
pickedTemperature(_window, {"Temperature: %4.1f", "Температура: %4.1f"},
    {_panelW/4, 0.33, .frame=1, .height=GUI::Main, .horAli=GUI::Left}),
pickedSpeed(_window, {"Speed: %3.1f %3.1f %3.1f", "Скорость: %3.1f %3.1f %3.1f"},
    {_panelW/4, 0.36, .frame=1, .height=GUI::Main, .horAli=GUI::Left}),
buildText(_window, {"Build object:", "Объект постройки:"}, {_panelW/2, 0.5, .frame=1}),
buildSwitchBox(_window, {_panelW/2, 0.535, .textColor = BLACK, .backColor = WHITE}, 0.14,
    {{"Not selected", "Не выбран"}, {"Demplish", "Снести"}, {"Wall", "Стена"}, {"Vent", "Вентилятор"},
    {"Heater", "Нагреватель"}, {"Check valve", "Обратный клапан"}, {"Cooler", "Холодильник"}}),
resetButton(_window, {"Reset", "Сброс"}, {_panelW/2, 0.8, .frame=1}) {}

void ControlMenu::reset() {
    mousePress = 0;
    pickedPressure.setValues(0.0f);
    pickedTemperature.setValues(0.0f);
    pickedSpeed.setValues(0.0f, 0.0f, 0.0f);
}

bool ControlMenu::click(const Mouse _mouse) {
    // Check, if in interface part
    if (panelBackplate.in(_mouse)) {
        // Check, if changing show mode
        if (modeSwitchBox.click(_mouse)) {
            interacter.setShowState(modeSwitchBox.getValue());
            return true;
        }
        // Update building menu
        if (buildSwitchBox.click(_mouse)) {
            // !
            // interacter.
            return true;
        }
        // Check, if reset
        if (resetButton.in(_mouse)) {
            interacter.reset();
            return true;
        }
        return true;
    }
    // Check, if start interaction
    mousePress = _mouse.getState();

    // Start camera movement
    if (mousePress == SDL_BUTTON_MMASK) {
        interacter.grid.click(_mouse.getX(), _mouse.getY());
        return true;
    }
    return false;
}

void ControlMenu::unclick(const Mouse _mouse) {
    // Resetting movement
    mousePress = 0;
}

bool ControlMenu::scroll(const Mouse _mouse, float _wheelY) {
    interacter.grid.zoom(_wheelY, _mouse);
    return true;
}

bool ControlMenu::press(SDL_Keycode _key) {
    if (modeSwitchBox.press(_key)) {
        return true;
    }
    if (buildSwitchBox.press(_key)) {
        return true;
    }
    if (_key == SDLK_R) {
        // Check, if try rotate part
        // ! interacter.holdingCell.rotate();
        return true;
    }
    if (buildSwitchBox.press(_key)) {
        return true;
    }
    return false;
}

void ControlMenu::update(const Mouse _mouse) {
    // Movement
    if (mousePress == SDL_BUTTON_MMASK) {
        interacter.grid.update(_mouse.getX(), _mouse.getY());
    }

    // Finding connected cell
    SDL_Point pos = interacter.getPosition(_mouse);

    // Check, if building
    /*if (buildSwitchBox.getValue() != 0) {
        holdingCellRect.x = _mouse.getX() - holdingCellRect.w/2;
        holdingCellRect.y = _mouse.getY() - holdingCellRect.h/2;
    }*/

    if (interacter.isLegimate(pos)) {
        // Check pressing on field
        switch (mousePress) {
        case SDL_BUTTON_LMASK:
            // Check, if try to do build
            // !

            break;

        case SDL_BUTTON_RMASK:
            //interacter.applyTemperature(position, 1.0);
            break;

        case SDL_BUTTON_X1MASK:
            //interacter.reduceMass(position, 2);
            break;

        case SDL_BUTTON_X2MASK:
            //interacter.applyTemperature(position, -1.0);
            break;

        default:
            break;
        }
        // Update showing parameter
        pickedPressure.setValues(interacter.getPressure(pos));
        pickedTemperature.setValues(interacter.getTemperature(pos));
        pickedSpeed.setValues(interacter.getSpeed(pos), 0.0f, 0.0f);
    } else {
        pickedPressure.setValues(0.0f);
        pickedTemperature.setValues(0.0f);
        pickedSpeed.setValues(0.0f, 0.0f, 0.0f);
    }
}

void ControlMenu::blit() const {
    // Panel interface
    panelBackplate.blit();
    modeText.blit();
    modeSwitchBox.blit();
    pickedPressure.blit();
    pickedTemperature.blit();
    pickedSpeed.blit();
    buildText.blit();
    buildSwitchBox.blit();
    resetButton.blit();

    // Building
    /*if (buildSwitchBox.getValue() != 0) {
        holdingCell.blitNormal(window, holdingCellRect);
    }*/
}

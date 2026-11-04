/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "controlMenu.hpp"


ControlMenu::ControlMenu(const Window& _window, float _X, float _W, BoardInteracter& _interacter)
: Template(_window),
interacter(_interacter),
panelBackplate(_window, _X, 0.5, _W, 1.0, 2.0, GREY, BLACK),
modeText(_window, {"Show mode:", "Режим отображения:"}, {_X, 0.15, .frame=1}),
modeSwitchBox(_window, {_X, 0.185, .textColor = BLACK, .backColor = WHITE}, 0.14, {{"Elements", "Элементы"},
    {"Thermal", "Температурный"}, {"Pressure", "Давление"}, {"Speed", "Скорость"}, {"Random", "Случайные"}}),
pickedElement(_window, {"Element: %f %f", "Элемент: %f %f"},
    {_X-_W/4, 0.36, .frame=1, .height=GUI::Main, .horAli=GUI::Left}),
pickedPressure(_window, {"Pressure: %6.1f", "Давление: %6.1f"},
    {_X-_W/4, 0.39, .frame=1, .height=GUI::Main, .horAli=GUI::Left}),
pickedTemperature(_window, {"Temperature: %4.1f", "Температура: %4.1f"},
    {_X-_W/4, 0.42, .frame=1, .height=GUI::Main, .horAli=GUI::Left}),
pickedUpSpeed(_window, {"Speed of up: %3.1f", "Скорость верхнего: %3.1f"},
    {_X-_W/4, 0.39, .frame=1, .height=GUI::Main, .horAli=GUI::Left}),
pickedLeftUpSpeed(_window, {"Speed of left up: %3.1f", "Скорость левого верхнего: %3.1f"},
    {_X-_W/4, 0.39, .frame=1, .height=GUI::Main, .horAli=GUI::Left}),
pickedLeftDownSpeed(_window, {"Speed of left down: %3.1f", "Скорость левого нижнего: %3.1f"},
    {_X-_W/4, 0.39, .frame=1, .height=GUI::Main, .horAli=GUI::Left}),
buildText(_window, {"Build object:", "Объект постройки:"}, {_X, 0.5, .frame=1}),
buildSwitchBox(_window, {_X, 0.535, .textColor = BLACK, .backColor = WHITE}, 0.14,
    {{"Not selected", "Не выбран"}, {"Demplish", "Снести"}, {"Wall", "Стена"}, {"Vent", "Вентилятор"},
    {"Heater", "Нагреватель"}, {"Check valve", "Обратный клапан"}, {"Cooler", "Холодильник"}}),
resetButton(_window, {"Reset", "Сброс"}, {_X, 0.6, .frame=1}),
hintText(_window, {"Control:", "Управление:"}, {_X, 0.72, .frame=1}),
interactionText(_window, {"Increase/decrease - LMB/RMB", "Увеличить/уменьшить - левая/правая кнопка"}, {_X, 0.75, .frame=1}),
movementText(_window, {"Movement - MMB", "Перемещение - средняя кнопка"}, {_X, 0.78, .frame=1}),
setText(_window, {"Set wall - button 4", "Установить стенку - кнопка 4"}, {_X, 0.81, .frame=1}),
stoppedText(_window, {"Stopped", "Остановленно"}, {0.5, 0.05, .frame=1}) {}

void ControlMenu::reset() {
    mousePress = 0;
    pos.type = 0;
    pickedElement.setValues(0, 0, 0);
    pickedPressure.setValues(0.0f);
    pickedTemperature.setValues(0.0f);
    pickedUpSpeed.setValues(0.0f);
    pickedLeftUpSpeed.setValues(0.0f);
    pickedLeftDownSpeed.setValues(0.0f);
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
        /*if (buildSwitchBox.click(_mouse)) {
            // !
            // interacter.
            return true;
        }*/
        // Check, if reset
        if (resetButton.in(_mouse)) {
            interacter.reset();
            return true;
        }
        return true;
    }
    switch (_mouse.getState()) {
    case SDL_BUTTON_MMASK:
        interacter.grid.click(_mouse);
        mousePress = _mouse.getState();
        return true;

    case SDL_BUTTON_LMASK:
    case SDL_BUTTON_RMASK:
        mousePress = _mouse.getState();
        return true;

    case SDL_BUTTON_X1MASK:
        pos = interacter.getPosition(_mouse);
        if (interacter.isLegimate(pos)) {
            interacter.applyAction(pos);
        }
        return true;

    default:
        break;
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
    /*if (buildSwitchBox.press(_key)) {
        return true;
    }*/
    if (_key == SDLK_R) {
        // Check, if try rotate part
        // ! interacter.holdingCell.rotate();
        return true;
    }
    if (_key == SDLK_SPACE) {
        BoardInteracter::running ^= true;
        return true;
    }
    /*if (buildSwitchBox.press(_key)) {
        return true;
    }*/
    return false;
}

void ControlMenu::update(const Mouse _mouse) {
    // Movement
    if (mousePress == SDL_BUTTON_MMASK) {
        interacter.grid.update(_mouse);
    }

    // Finding connected cell
    pos = interacter.getPosition(_mouse);
    pickedElement.setValues(pos.x, pos.y);

    // Check, if building
    /*if (buildSwitchBox.getValue() != 0) {
        holdingCellRect.x = _mouse.getX() - holdingCellRect.w/2;
        holdingCellRect.y = _mouse.getY() - holdingCellRect.h/2;
    }*/

    if (interacter.isLegimate(pos)) {
        // Check pressing on field
        switch (mousePress) {
        case SDL_BUTTON_LMASK:
            // Check, if try to build
            // !
            interacter.applyMovement(pos, 1.0);
            break;

        case SDL_BUTTON_RMASK:
            interacter.applyMovement(pos, -1.0);
            break;

        default:
            break;
        }
        // Update showing parameter
        CellInfo info = interacter.getInfo(pos);
        switch (pos.type) {
        case 0:
            pickedTemperature.setValues(info.temperature);
            pickedPressure.setValues(info.pressure);
            break;

        case 1:
            pickedUpSpeed.setValues(info.speed);
            break;

        case 2:
            pickedLeftUpSpeed.setValues(info.speed);
            break;

        default:
            pickedLeftDownSpeed.setValues(info.speed);
            break;
        }
    } else {
        pickedPressure.setValues(0.0f);
        pickedTemperature.setValues(0.0f);
        pickedUpSpeed.setValues(0.0f);
        pickedLeftUpSpeed.setValues(0.0f);
        pickedLeftDownSpeed.setValues(0.0f);
    }
}

void ControlMenu::blit() const {
    // Panel interface
    panelBackplate.blit();
    modeText.blit();
    modeSwitchBox.blit();

    // Selected element
    pickedElement.blit();
    switch (pos.type) {
    case 0:  // Cell
        pickedPressure.blit();
        pickedTemperature.blit();
        break;

    case 1:  // Upper channel
        pickedUpSpeed.blit();
        break;

    case 2:  // Left up channel
        pickedLeftUpSpeed.blit();
        break;

    default:
        pickedLeftDownSpeed.blit();
        break;
    }
    /*buildText.blit();
    buildSwitchBox.blit();*/
    resetButton.blit();

    hintText.blit();
    movementText.blit();
    interactionText.blit();
    setText.blit();

    if (!BoardInteracter::running) {
        stoppedText.blit();
    }

    // Building
    /*if (buildSwitchBox.getValue() != 0) {
        holdingCell.blitNormal(window, holdingCellRect);
    }*/
}

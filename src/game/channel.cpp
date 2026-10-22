/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "channel.hpp"


Channel::Channel() {}

void Channel::reset() {
    sp = 0.0;
    width = 1;
}

void Channel::setWall() {
    width = 0;
}

bool Channel::isWall() const {
    return width == 0;
}

void Channel::tryAdd(float _val) {
    sp += _val*width;
}

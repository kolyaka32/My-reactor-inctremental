/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "channel.hpp"


Channel::Channel() {}

void Channel::reset() {
    sp = 0.0;
    wall = false;
}

void Channel::tryAdd(float _val) {
    if (!wall) {
        sp += _val;
    }
}

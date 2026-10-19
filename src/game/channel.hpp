/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "../data/app.hpp"


// Transition between two cells
class Channel {
private:
    /* data */

public:
    Channel();
    void reset();

    bool wall;  // Is channel is accesible to change
    float sp;  // Speed
    void tryAdd(float val);
};

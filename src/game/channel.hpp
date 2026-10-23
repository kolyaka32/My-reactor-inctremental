/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "../data/app.hpp"


// Transition between two cells
class Channel {
public:
    Uint8 width;  // Width of chanel (0 if wall)
    float sp;  // Speed

    Channel();
    void reset();
    void setWall();
    void switchState();
    bool isWall() const;
    void tryAdd(float val);
};

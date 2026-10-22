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

    //bool wall;  // Is channel is accesible to change
    Uint8 width;  // Width of chanel (0 if wall)
    float sp;  // Speed

    void setWall();
    bool isWall() const;
    void tryAdd(float val);
};

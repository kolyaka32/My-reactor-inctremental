/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "board.hpp"


Board::Board(unsigned _width, unsigned _height) noexcept
: height(_height),
width(_width) {
    // Allocating arrays
    cells = new Cell[height*width];
    upChannels = new Channel[(height+1)*width];
    upLeftChannels = new Channel[(height+1)*(width+1)];
    downLeftChannels = new Channel[(height+1)*(width+1)];
}

Board::~Board() noexcept {
    delete[] cells;
    delete[] upChannels;
    delete[] upLeftChannels;
    delete[] downLeftChannels;
}

void Board::reset() {
    for (int i=0; i < height*width; ++i) {
        cells[i].reset();
    }
    for (int i=0; i < (height+1)*width; ++i) {
        upChannels[i].reset();
    }
    for (int i=0; i < (height+1)*(width+1); ++i) {
        upLeftChannels[i].reset();
        downLeftChannels[i].reset();
    }

    // Placing vertical borders
    for (int x=0; x < width; ++x) {
        upChannels[x].setWall();
        upChannels[height*width+x].setWall();
    }
    for (int x=1; x < width; x+=2) {
        downLeftChannels[x].setWall();
        upLeftChannels[x+1].setWall();
        downLeftChannels[height*(width+1)+x].setWall();
        upLeftChannels[height*(width+1)+x].setWall();
    }
    for (int y=0; y < height+1; ++y) {
        upLeftChannels[y*(width+1)].setWall();
        upLeftChannels[y*(width+1)+width].setWall();
        downLeftChannels[y*(width+1)].setWall();
        downLeftChannels[y*(width+1)+width].setWall();
    }
}

bool Board::isLegimate(ChannelPos _pos) const {
    switch (_pos.type) {
    case 0:  // Cell
        return _pos.x >= 0 && _pos.x < width &&
            _pos.y >= 0 && _pos.y < height;

    case 1:  // Up channel
        return _pos.x >= 0 && _pos.x < width &&
            _pos.y >= 0 && _pos.y < height+1;

    case 2:  // Left up channel
        return _pos.x >= 0 && _pos.x < width &&
            _pos.y >= 0 && _pos.y < height;

    default:  // Left down channel
        return _pos.x >= 0 && _pos.x < width &&
            _pos.y >= 0 && _pos.y < height;
    }
}

CellInfo Board::getInfo(ChannelPos _pos) const {
    /*switch (_pos.type) {
    case 0:  // Cell
        return {cells[_pos.y*width+_pos.x].temperature,
            cells[_pos.y*width+_pos.x].pressure};

    case 1:  // Up channel
        return {.speed = upChannels[_pos.y*width+_pos.x].sp};

    case 2:  // Left up channel
        return {.speed = upLeftChannels[_pos.y*(width+1)+_pos.x].sp};

    default:  // Left down channel
        return {.speed = downLeftChannels[(_pos.y+1)*(width+1)+_pos.x].sp};
    }*/
}

void Board::applyMovement(ChannelPos _pos, float _val) {
    /*switch (_pos.type) {
    case 0:  // Cell
        // Nothing
        cells[_pos.y*width+_pos.x].temperature += _val;
        return;

    case 1:  // Up channel
        upChannels[_pos.y*width+_pos.x].sp += _val;
        return;

    case 2:  // Left up channel
        upLeftChannels[_pos.y*(width+1)+_pos.x].sp += _val;
        return;

    default:  // Left down channel
        downLeftChannels[(_pos.y+1)*(width+1)+_pos.x].sp += _val;
        return;
    }*/
}

/*void Board::checkNext() {
    static int counter = 0;
    downLeftChannels[counter].wall = true;
    downLeftChannels[counter+1].wall = false;
    counter = (counter+1) % ((width+1)*(height+1));
}*/

void Board::projectUnit(Cell& _cell, Channel& _c1, Channel& _c2,
    Channel& _c3, Channel& _c4, Channel& _c5, Channel& _c6) {
    // Check wall count
    int count = _c1.width + _c2.width + _c3.width + _c4.width + _c5.width + _c6.width;
    if (count) {
        // Calculate divergation
        float div = (_c1.sp - _c2.sp + _c3.sp - _c4.sp + _c5.sp - _c6.sp)/count;
        // Avaraging divergation spread between all channels
        _c1.tryAdd(-div);
        _c3.tryAdd(-div);
        _c5.tryAdd(-div);
        _c2.tryAdd(div);
        _c4.tryAdd(div);
        _c6.tryAdd(div);
        // Update pressure
        _cell.pressure += div/count;
    }
}

void Board::applyForces() {
    // Place forces from external places and cells
}

void Board::project() {
    // Deviation compensation
    // Odd cells
    for (int y=0; y < height; ++y) {
        for (int x=0; x < width; x+=2) {
            projectUnit(cells[y*width+x],
                upChannels[y*width+x],
                upChannels[(y+1)*width+x],
                upLeftChannels[y*(width+1)+x],
                upLeftChannels[y*(width+1)+x+1],
                downLeftChannels[(y+1)*(width+1)+x],
                downLeftChannels[y*(width+1)+x+1]
            );
        }
    }
    // Even cells
    for (int y=0; y < height; ++y) {
        for (int x=1; x < width; x+=2) {
            projectUnit(cells[y*width+x],
                upChannels[y*width+x],
                upChannels[(y+1)*width+x],
                upLeftChannels[y*(width+1)+x],
                upLeftChannels[(y+1)*(width+1)+x+1],
                downLeftChannels[y*(width+1)+x],
                downLeftChannels[y*(width+1)+x+1]
            );
        }
    }
}

void Board::advect() {
    // Transfer of fluid (save momentum)
    /*float dt0 = 0.3*width;
    for (int i=1; i<=width; i++) {
        for (int j=1; j<=width; j++) {
            float x = i - dt0 * u[j*width+i];
            float y = j - dt0 * v[j*width+i];
            if (x<0.5) x=0.5;
            if (x>width+0.5) x=width+ 0.5;
            if (y<0.5) y=0.5;
            if (y>width+0.5) y=width+ 0.5;
            int i0=(int)x;
            int j0=(int)y;
            float s1 = x-i0;
            float s0 = 1-s1;
            float t1 = y-j0;
            float t0 = 1-t1;
            d[j0*width+i0] = s0*(t0*d0[j0*width+i0] + t1*d0[(j0+1)*width+i0]) +
                s1*(t0*d0[j0*width+i0+1)] + t1*d0[(j0+1)*width+i0+1]);
        }
    }*/
}

SDL_FColor Board::getSpeedColor(SDL_Point _pos) const {
    return {upChannels[_pos.y*(width+1)+_pos.x].sp/10.0f,
        upLeftChannels[_pos.y*(width+1)+_pos.x].sp/10.0f,
        downLeftChannels[(_pos.y+1)*(width+1)+_pos.x].sp/10.0f};
}

SDL_FColor Board::getRandomColor() const {
    return {SDL_randf(), SDL_randf(), SDL_randf()};
}

void Board::blitUnit(const Window& _window, SDL_FRect _rect, SDL_FColor _color, bool _up, bool _upLeft, bool _downLeft) const {
    SDL_Vertex vertexes[] = {
        {{_rect.x+_rect.w/4,   _rect.y},           _color},
        {{_rect.x+_rect.w*3/4, _rect.y},           _color},
        {{_rect.x,             _rect.y+_rect.h/2}, _color},
        {{_rect.x+_rect.w,     _rect.y+_rect.h/2}, _color},
        {{_rect.x+_rect.w/4,   _rect.y+_rect.h},   _color},
        {{_rect.x+_rect.w*3/4, _rect.y+_rect.h},   _color},
    };
    int order[] = {0, 1, 2,    1, 2, 3,    2, 3, 4,    3, 4, 5};
    _window.drawGeometry(vertexes, SDL_arraysize(vertexes), order, SDL_arraysize(order));
    if (_up) {
        _window.drawLine(vertexes[0].position, vertexes[1].position);
    }
    if (_upLeft) {
        _window.drawLine(vertexes[0].position, vertexes[2].position);
    }
    if (_downLeft) {
        _window.drawLine(vertexes[2].position, vertexes[4].position);
    }
}

void Board::blitDown(const Window& _window, SDL_FRect _rect) const {
    _window.drawLine({_rect.x+_rect.w/4, _rect.y+_rect.h}, {_rect.x+_rect.w*3/4, _rect.y+_rect.h});
}

void Board::blitUpperRight(const Window& _window, SDL_FRect _rect) const {
    _window.drawLine({_rect.x+_rect.w*3/4, _rect.y}, {_rect.x+_rect.w, _rect.y+_rect.h/2});
}

void Board::blitDownRight(const Window& _window, SDL_FRect _rect) const {
    _window.drawLine({_rect.x+_rect.w, _rect.y+_rect.h/2}, {_rect.x+_rect.w*3/4, _rect.y+_rect.h});
}

void Board::blitBlockNormal(const Window& _window, SDL_FRect _rect, int _offset) const {
    float startY = _rect.y;
    for (int x=_offset; x < width; x+=2) {
        _rect.y = startY;
        for (int y=0; y < height; ++y) {
            blitUnit(_window, _rect, cells[y*width+x].getNormalColor(), upChannels[y*width+x].isWall(),
                upLeftChannels[y*(width+1)+x].isWall(), downLeftChannels[(y+1)*(width+1)+x].isWall());
            _rect.y += _rect.h;
        }
        _rect.x += _rect.w * 1.5;
    }
}

void Board::blitBlockThermal(const Window& _window, SDL_FRect _rect, int _offset) const {
    float startY = _rect.y;
    for (int x=_offset; x < width; x+=2) {
        _rect.y = startY;
        for (int y=0; y < height; ++y) {
            blitUnit(_window, _rect, cells[y*width+x].getThermalColor(), upChannels[y*width+x].isWall(),
                upLeftChannels[y*(width+1)+x].isWall(), downLeftChannels[(y+1)*(width+1)+x].isWall());
            _rect.y += _rect.h;
        }
        _rect.x += _rect.w * 1.5;
    }
}

void Board::blitBlockPressure(const Window& _window, SDL_FRect _rect, int _offset) const {
    float startY = _rect.y;
    for (int x=_offset; x < width; x+=2) {
        _rect.y = startY;
        for (int y=0; y < height; ++y) {
            blitUnit(_window, _rect, cells[y*width+x].getPressureColor(), upChannels[y*width+x].isWall(),
                upLeftChannels[y*(width+1)+x].isWall(), downLeftChannels[(y+1)*(width+1)+x].isWall());
            _rect.y += _rect.h;
        }
        _rect.x += _rect.w * 1.5;
    }
}

void Board::blitBlockSpeed(const Window& _window, SDL_FRect _rect, int _offset) const {
    float startY = _rect.y;
    for (int x=_offset; x < width; x+=2) {
        _rect.y = startY;
        for (int y=0; y < height; ++y) {
            blitUnit(_window, _rect, getSpeedColor({x, y}), upChannels[y*width+x].isWall(),
                upLeftChannels[y*(width+1)+x].isWall(), downLeftChannels[(y+1)*(width+1)+x].isWall());
            _rect.y += _rect.h;
        }
        _rect.x += _rect.w * 1.5;
    }
}

void Board::blitBlockRandom(const Window& _window, SDL_FRect _rect, int _offset) const {
    float startY = _rect.y;
    for (int x=_offset; x < width; x+=2) {
        _rect.y = startY;
        for (int y=0; y < height; ++y) {
            blitUnit(_window, _rect, getRandomColor(), upChannels[y*width+x].isWall(),
                upLeftChannels[y*(width+1)+x].isWall(), downLeftChannels[(y+1)*(width+1)+x].isWall());
            _rect.y += _rect.h;
        }
        _rect.x += _rect.w * 1.5;
    }
}

void Board::blitLines(const Window& _window, SDL_FRect _oddRect, SDL_FRect _evenRect) const {
    float startOddX = _oddRect.x;
    float startEvenX = _evenRect.x;
    // Upper border
    for (int x=0; x < width; x+=2) {
        if (downLeftChannels[x+1].isWall()) {
            blitUpperRight(_window, _oddRect);
        }
        _oddRect.x += _oddRect.w * 1.5;
        _evenRect.x += _evenRect.w * 1.5;
    }

    // Right border
    if (width%2 == 0) {
        _evenRect.x -= _evenRect.w * 1.5;
        for (int y=0; y < height; ++y) {
            if (upLeftChannels[(width+1)*y+width].isWall()) {
                blitDownRight(_window, _evenRect);
            }
            if (downLeftChannels[(width+1)*y+width].isWall()) {
                blitUpperRight(_window, _evenRect);
            }
            _evenRect.y += _evenRect.h;
        }
        _oddRect.y += _oddRect.h*(height-1);
        _evenRect.y -= _evenRect.h;
    } else {
        _oddRect.x -= _oddRect.w * 1.5;
        for (int y=0; y < height; ++y) {\
            if (upLeftChannels[y*(width+1)+width].isWall()) {
                blitDownRight(_window, _oddRect);
            }
            if (downLeftChannels[y*(width+1)+width].isWall()) {
                blitUpperRight(_window, _oddRect);
            }
            _oddRect.y += _oddRect.h;
        }
        _evenRect.y += _evenRect.h*(height-1);
        _oddRect.y -= _oddRect.h;
    }

    // Down border
    _oddRect.x = startOddX;
    for (int x=0; x < width; x+=2) {
        if (upChannels[width*height+x].isWall()) {
            blitDown(_window, _oddRect);
        }
        _oddRect.x += _oddRect.w * 1.5;
    }
    _evenRect.x = startEvenX;
    for (int x=1; x < width; x+=2) {
        if (upChannels[width*height+x].isWall()) {
            blitDown(_window, _evenRect);
        }
        _evenRect.x += _evenRect.w * 1.5;
    }
    _evenRect.x = startEvenX;
    for (int x=1; x < width-1; x+=2) {
        if (upLeftChannels[(width+1)*height+x].isWall()) {
            blitDownRight(_window, _evenRect);
        }
        _evenRect.x += _evenRect.w * 1.5;
    }
}

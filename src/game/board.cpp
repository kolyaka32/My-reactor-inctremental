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

    reset();
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
        upChannels[x].wall = true;
        upChannels[height*width+x].wall = true;
    }
    for (int x=1; x < width; x+=2) {
        downLeftChannels[x].wall = true;
        upLeftChannels[x+1].wall = true;
        downLeftChannels[height*(width+1)+x].wall = true;
        upLeftChannels[height*(width+1)+x].wall = true;
    }
    for (int y=0; y < height+1; ++y) {
        upLeftChannels[y*(width+1)].wall = true;
        upLeftChannels[y*(width+1)+width].wall = true;
        downLeftChannels[y*(width+1)].wall = true;
        downLeftChannels[y*(width+1)+width].wall = true;
    }
}

bool Board::isLegimate(SDL_Point _pos) const {
    return _pos.x > 0 && _pos.x <= width &&
        _pos.y > 0 && _pos.y <= height;
}

float Board::getTemperature(SDL_Point _pos) const {
    return cells[_pos.y*width+_pos.x].temperature;
}

float Board::getPressure(SDL_Point _pos) const {
    return cells[_pos.y*width+_pos.x].pressure;
}

float Board::getSpeed(SDL_Point _pos) const {
    return upChannels[_pos.y*(width+1)+_pos.x].sp;
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
    int count = _c1.wall + _c2.wall + _c3.wall + _c4.wall + _c5.wall + _c6.wall;
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
                upLeftChannels[y*width+x],
                upLeftChannels[y*width+x+1],
                downLeftChannels[y*width+x],
                downLeftChannels[(y-1)*width+x+1]
            );
        }
    }
    // Even cells
    for (int y=0; y < height; ++y) {
        for (int x=1; x < width; x+=2) {
            projectUnit(cells[y*width+x],
                upChannels[y*width+x],
                upChannels[(y+1)*width+x],
                upLeftChannels[y*width+x],
                upLeftChannels[(y+1)*width+x+1],
                downLeftChannels[y*width+x],
                downLeftChannels[y*width+x+1]
            );
        }
    }
}

void Board::advect() {
    // Transfer of fluid (save momentum)
    // !
    const int N=width;

    /*float dt0 = 0.3*N;
    for (int i=1; i<=N; i++) {
        for (int j=1; j<=N; j++) {
            float x = i - dt0 * u[IX(i,j)];
            float y = j - dt0 * v[IX(i,j)];
            if (x<0.5) x=0.5;
            if (x>N+0.5) x=N+ 0.5;
            if (y<0.5) y=0.5;
            if (y>N+0.5) y=N+ 0.5;
            int i0=(int)x;
            int j0=(int)y;
            float s1 = x-i0;
            float s0 = 1-s1;
            float t1 = y-j0;
            float t0 = 1-t1;
            d[IX(i,j)] = s0*(t0*d0[IX(i0,j0)]+t1*d0[IX(i0,j0+1)])+
                s1*(t0*d0[IX(i0+1,j0)]+t1*d0[IX(i0+1,j0+1)]);
        }
    }*/
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
            blitUnit(_window, _rect, cells[y*width+x].getNormalColor(), upChannels[y*width+x].wall,
                upLeftChannels[y*(width+1)+x].wall, downLeftChannels[(y+1)*(width+1)+x].wall);
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
            blitUnit(_window, _rect, cells[y*width+x].getThermalColor(), upChannels[y*width+x].wall,
                upLeftChannels[y*(width+1)+x].wall, downLeftChannels[(y+1)*(width+1)+x].wall);
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
            blitUnit(_window, _rect, cells[y*width+x].getPressureColor(), upChannels[y*width+x].wall,
                upLeftChannels[y*(width+1)+x].wall, downLeftChannels[(y+1)*(width+1)+x].wall);
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
            blitUnit(_window, _rect, cells[y*width+x].getRandomColor(), upChannels[y*width+x].wall,
                upLeftChannels[y*(width+1)+x].wall, downLeftChannels[(y+1)*(width+1)+x].wall);
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
        if (downLeftChannels[x+1].wall) {
            blitUpperRight(_window, _oddRect);
        }
        _oddRect.x += _oddRect.w * 1.5;
        _evenRect.x += _evenRect.w * 1.5;
    }

    // Right border
    if (width%2 == 0) {
        _evenRect.x -= _evenRect.w * 1.5;
        for (int y=0; y < height; ++y) {
            if (upLeftChannels[(width+1)*y+width].wall) {
                blitDownRight(_window, _evenRect);
            }
            if (downLeftChannels[(width+1)*y+width].wall) {
                blitUpperRight(_window, _evenRect);
            }
            _evenRect.y += _evenRect.h;
        }
        _oddRect.y += _oddRect.h*(height-1);
        _evenRect.y -= _evenRect.h;
    } else {
        _oddRect.x -= _oddRect.w * 1.5;
        for (int y=0; y < height; ++y) {\
            if (upLeftChannels[y*(width+1)+width].wall) {
                blitDownRight(_window, _oddRect);
            }
            if (downLeftChannels[y*(width+1)+width].wall) {
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
        if (upChannels[width*height+x].wall) {
            blitDown(_window, _oddRect);
        }
        _oddRect.x += _oddRect.w * 1.5;
    }
    _evenRect.x = startEvenX;
    for (int x=1; x < width; x+=2) {
        if (upChannels[width*height+x].wall) {
            blitDown(_window, _evenRect);
        }
        _evenRect.x += _evenRect.w * 1.5;
    }
    _evenRect.x = startEvenX;
    for (int x=1; x < width-1; x+=2) {
        if (upLeftChannels[(width+1)*height+x].wall) {
            blitDownRight(_window, _evenRect);
        }
        _evenRect.x += _evenRect.w * 1.5;
    }
}

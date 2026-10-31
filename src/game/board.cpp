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
        cells[i].reset(i);
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
            _pos.y > 0 && _pos.y < height;

    case 2:  // Left up channel
        if (_pos.x % 2 == 0) {
            return _pos.x > 0 && _pos.x < width &&
                _pos.y > 0 && _pos.y < height;
        } else {
            return _pos.x > 0 && _pos.x < width &&
                _pos.y >= 0 && _pos.y < height;
        }

    default:  // Left down channel
        if (_pos.x % 2 == 0) {
            return _pos.x > 0 && _pos.x < width &&
                _pos.y >= 0 && _pos.y < height;
        } else {
            return _pos.x > 0 && _pos.x < width &&
                _pos.y >= 0 && _pos.y < height-1;
        }
    }
}

CellInfo Board::getInfo(ChannelPos _pos) const {
    switch (_pos.type) {
    case 0:  // Cell
        return {cells[_pos.y*width+_pos.x].temperature,
            cells[_pos.y*width+_pos.x].pressure};

    case 1:  // Up channel
        return {.speed = upChannels[_pos.y*width+_pos.x].sp};

    case 2:  // Left up channel
        return {.speed = upLeftChannels[_pos.y*(width+1)+_pos.x].sp};

    default:  // Left down channel
        return {.speed = downLeftChannels[(_pos.y+1)*(width+1)+_pos.x].sp};
    }
}

Cell& Board::getCell(int _x, int _y) {
    if (_x < 0 || _x >= width ||
        _y < 0 || _y >= height) {
        logger.important("Innaccesible cell");
        return cells[0];
    }
    return cells[_y*width+_x];
}

Channel& Board::getUpChannel(int _x, int _y) {
    if (_x < 0 || _x >= width ||
        _y < 0 || _y > height) {
        logger.important("Innaccesible up channel");
        return upChannels[0];
    }
    return upChannels[_y*width+_x];
}

Channel& Board::getLeftUpChannel(int _x, int _y) {
    if (_x < 0 || _x > width ||
        _y < 0 || _y > height) {
        logger.important("Innaccesible left up channel");
        return upLeftChannels[0];
    }
    return upLeftChannels[_y*(width+1)+_x];
}

Channel& Board::getLeftDownChannel(int _x, int _y) {
    if (_x < 0 || _x > width ||
        _y < -1 || _y >= height) {
        logger.important("Innaccesible left down channel");
        return downLeftChannels[0];
    }
    return downLeftChannels[(_y+1)*(width+1)+_x];
}

void Board::applyMovement(ChannelPos _pos, float _val) {
    switch (_pos.type) {
    case 0:
        getCell(_pos.x, _pos.y).temperature += _val;
        return;

    case 1:
        getUpChannel(_pos.x, _pos.y).tryAdd(_val);
        return;

    case 2:
        getLeftUpChannel(_pos.x, _pos.y).tryAdd(_val);
        return;

    default:
        getLeftDownChannel(_pos.x, _pos.y).tryAdd(_val);
        return;
    }
}

void Board::applyAction(ChannelPos _pos) {
    switch (_pos.type) {
    case 0:
        // Nothing
        return;

    case 1:
        getUpChannel(_pos.x, _pos.y).switchState();
        return;

    case 2:
        getLeftUpChannel(_pos.x, _pos.y).switchState();
        return;

    default:
        getLeftDownChannel(_pos.x, _pos.y).switchState();
        return;
    }
}

void Board::projectUnit(Cell& _cell, Channel& _h1, Channel& _h2,
    Channel& _h3, Channel& _h4, Channel& _h5, Channel& _h6) {
    // Check wall count
    int count = _h1.width + _h2.width + _h3.width + _h4.width + _h5.width + _h6.width;
    if (count) {
        // Calculate divergation
        float div = (_h1.sp - _h2.sp + _h3.sp - _h4.sp + _h5.sp - _h6.sp)/count;
        // Avaraging divergation spread between all channels
        _h1.tryAdd(-div);
        _h3.tryAdd(-div);
        _h5.tryAdd(-div);
        _h2.tryAdd(div);
        _h4.tryAdd(div);
        _h6.tryAdd(div);
        // Update pressure
        _cell.pressure += div/count;
    }
}

void Board::advectMass(Cell& _c1, Cell& _c2, Channel& _h) {
    if (_h.isWall()) {
        return;
    }
    if (_h.sp > 0) {
        _c2.temperature += (_c1.temperature - _c2.temperature)*_h.sp/(_h.sp+10.0f)/2;
    } else {
        _c1.temperature += (_c1.temperature - _c2.temperature)*_h.sp/(_h.sp+10.0f)/2;
    }
}

void Board::advectSpeed(Channel& _h1, Channel& _h2) {
    if (_h1.isWall() || _h2.isWall()) {
        return;
    }
    float avr = (_h1.sp - _h2.sp)/2*resistance;
    _h1.sp -= avr;
    _h2.sp += avr;
}

void Board::applyForces() {
    // Place forces from external places and cells
}

void Board::project() {
    // Deviation compensation
    // Odd cells
    for (int y=0; y < height; ++y) {
        for (int x=0; x < width; x+=2) {
            projectUnit(getCell(x, y),
                getUpChannel(x, y),
                getUpChannel(x, y+1),
                getLeftUpChannel(x, y),
                getLeftUpChannel(x+1, y),
                getLeftDownChannel(x, y),
                getLeftDownChannel(x+1, y-1)
            );
        }
    }
    // Even cells
    for (int y=0; y < height; ++y) {
        for (int x=1; x < width; x+=2) {
            projectUnit(getCell(x, y),
                getUpChannel(x, y),
                getUpChannel(x, y+1),
                getLeftUpChannel(x, y),
                getLeftUpChannel(x+1, y+1),
                getLeftDownChannel(x, y),
                getLeftDownChannel(x+1, y)
            );
        }
    }
}

void Board::advectMass() {
    // Transfer of fluid itself (transfer between cells)
    // Vertical
    for (int y=1; y<height; y++) {
        for (int x=0; x<width; x++) {
            advectMass(getCell(x, y-1), getCell(x, y), getUpChannel(x, y));
        }
    }
    // Up left
    for (int y=0; y<height; y++) {
        for (int x=1; x<width; x+=2) {
            advectMass(getCell(x-1, y), getCell(x, y), getLeftUpChannel(x, y));
        }
    }
    for (int y=1; y<height; y++) {
        for (int x=2; x<width; x+=2) {
            advectMass(getCell(x-1, y-1), getCell(x, y), getLeftUpChannel(x, y));
        }
    }
    // Down left
    for (int y=0; y<height-1; y++) {
        for (int x=1; x<width; x+=2) {
            advectMass(getCell(x-1, y+1), getCell(x, y), getLeftDownChannel(x, y));
        }
    }
    for (int y=0; y<height; y++) {
        for (int x=2; x<width; x+=2) {
            advectMass(getCell(x-1, y), getCell(x, y), getLeftDownChannel(x, y));
        }
    }
}

void Board::advectSpeed() {
    // Transfer of fluid's speed (save momentum)
    // Vertical
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            advectSpeed(getUpChannel(x, y), getUpChannel(x, y+1));
        }
    }
    // Up left
    for (int y=0; y<height-1; y++) {
        for (int x=1; x<width; x+=2) {
            advectSpeed(getLeftUpChannel(x, y), getLeftUpChannel(x+1, y+1));
        }
    }
    for (int y=0; y<height; y++) {
        for (int x=2; x<width; x+=2) {
            advectSpeed(getLeftUpChannel(x, y), getLeftUpChannel(x+1, y));
        }
    }
    // Down left
    for (int y=0; y<height-1; y++) {
        for (int x=1; x<width; x+=2) {
            advectSpeed(getLeftDownChannel(x-1, y+1), getLeftDownChannel(x, y));
        }
    }
    for (int y=1; y<height; y++) {
        for (int x=2; x<width; x+=2) {
            advectSpeed(getLeftDownChannel(x-1, y), getLeftDownChannel(x, y));
        }
    }
}

SDL_FColor Board::getSpeedColor(SDL_Point _pos) const {
    return {upChannels[_pos.y*(width+1)+_pos.x].sp/10.0f+0.5f,
        upLeftChannels[_pos.y*(width+1)+_pos.x].sp/10.0f+0.5f,
        downLeftChannels[(_pos.y+1)*(width+1)+_pos.x].sp/10.0f+0.5f};
}

SDL_FColor Board::getRandomColor() const {
    return {SDL_randf(), SDL_randf(), SDL_randf()};
}

void Board::blitUnit(const Window& _window, SDL_FRect _rect, SDL_FColor _color) const {
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
}

void Board::blitWalls(const Window& _window, SDL_FRect _rect, bool _up, bool _upLeft, bool _downLeft) const {
    SDL_FPoint p0 = {_rect.x+_rect.w/4,   _rect.y};
    SDL_FPoint p1 = {_rect.x+_rect.w*3/4, _rect.y};
    SDL_FPoint p2 = {_rect.x,             _rect.y+_rect.h/2};
    SDL_FPoint p4 = {_rect.x+_rect.w/4,   _rect.y+_rect.h};
    if (_up) {
        _window.drawLine(p0, p1);
    }
    if (_upLeft) {
        _window.drawLine(p0, p2);
    }
    if (_downLeft) {
        _window.drawLine(p2, p4);
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
            blitUnit(_window, _rect, cells[y*width+x].getNormalColor());
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
            blitUnit(_window, _rect, cells[y*width+x].getThermalColor());
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
            blitUnit(_window, _rect, cells[y*width+x].getPressureColor());
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
            blitUnit(_window, _rect, getSpeedColor({x, y}));
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
            blitUnit(_window, _rect, getRandomColor());
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
    // Main odd part
    for (int y=0; y < height; ++y) {
        _oddRect.x = startOddX;
        for (int x=0; x < width; x+=2) {
            blitWalls(_window, _oddRect, upChannels[y*width+x].isWall(), upLeftChannels[y*(width+1)+x].isWall(),
                downLeftChannels[(y+1)*(width+1)+x].isWall());
            _oddRect.x += _oddRect.w * 1.5;
        }
        _oddRect.y += _oddRect.h;
    }
    // Main even part
    for (int y=0; y < height; ++y) {
        _evenRect.x = startEvenX;
        for (int x=1; x < width; x+=2) {
            blitWalls(_window, _evenRect, upChannels[y*width+x].isWall(), upLeftChannels[y*(width+1)+x].isWall(),
                downLeftChannels[(y+1)*(width+1)+x].isWall());
            _evenRect.x += _evenRect.w * 1.5;
        }
        _evenRect.y += _evenRect.h;
    }

    // Right border
    _oddRect.x -= _oddRect.w * 1.5;
    _evenRect.x -= _evenRect.w * 1.5;
    if (width%2 == 0) {
        _evenRect.y -= _evenRect.h*height;
        for (int y=0; y < height; ++y) {
            if (upLeftChannels[y*(width+1)+width].isWall()) {
                blitDownRight(_window, _evenRect);
            }
            if (downLeftChannels[y*(width+1)+width].isWall()) {
                blitUpperRight(_window, _evenRect);
            }
            _evenRect.y += _evenRect.h;
        }
    } else {
        _oddRect.y -= _oddRect.h*height;
        for (int y=0; y < height; ++y) {
            if (upLeftChannels[y*(width+1)+width].isWall()) {
                blitDownRight(_window, _oddRect);
            }
            if (downLeftChannels[y*(width+1)+width].isWall()) {
                blitUpperRight(_window, _oddRect);
            }
            _oddRect.y += _oddRect.h;
        }
    }

    // Down border
    _oddRect.y -= _oddRect.h;
    _evenRect.y -= _evenRect.h;
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

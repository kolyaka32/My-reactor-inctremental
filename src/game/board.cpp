/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "board.hpp"


Board::Board() {
    reset();
}

void Board::reset() {
    for (int i=0; i < height*width; ++i) {
        cells[i].reset();
    }
}

int Board::getWidth() const {
    return width;
}

int Board::getHeight() const {
    return height;
}

bool Board::isLegimate(SDL_Point _pos) const {
    return _pos.x > 0 && _pos.x <= width &&
        _pos.y > 0 && _pos.y <= height;  // ! Check end positions
}

const Cell& Board::operator[](SDL_Point _pos) const {
    return cells[_pos.y*width+_pos.x];
}

/*void Board::resetCell(SDL_Point _pos) {
    cells[_pos.y*width+_pos.x].reset();
}

void Board::applyMass(SDL_Point _pos, float _deltaMass) {
    cells[_pos.y*width+_pos.x].applyMass(_deltaMass);
}

void Board::reduceMass(SDL_Point _pos, float _deltaMass) {
    cells[_pos.y*width+_pos.x].reduceMass(_deltaMass);
}

void Board::applyTemperature(SDL_Point _pos, float _temperature) {
    cells[_pos.y*width+_pos.x].applyTemperature(_temperature);
}

float Board::getPressure(SDL_Point _pos) const {
    return cells[_pos.y*width+_pos.x].getPressure();
}

float Board::getTemperature(SDL_Point _pos) const {
    return cells[_pos.y*width+_pos.x].getTemperature();
}*/

void Board::update() {
    // Vertical interactions
    /*for (int y=0; y < height-1; ++y) {
        for (int x=0; x < width; ++x) {
            // Exchanging with cell bellow
            cells[y*width+x].exchange(cells[(y+1)*width+x]);
        }
    }

    // Horizontal interactions
    for (int y=0; y < height; ++y) {
        for (int x=0; x < width-1; ++x) {
            // Exchanging with cell right to it
            cells[y*width+x].exchange(cells[y*width+x+1]);
        }
    }

    // Resetting side cells to global parameters
    /*for (int y=0; y < height; ++y) {
        cells[y*width].exchange();  // Left cells
        cells[y*width+width-1].exchange();  // Right cells
    }
    for (int x=1; x < width-1; ++x) {
        cells[x].exchange();  // Upper cells
        cells[height*width-x-1].exchange();  // Bottom cells
    }*/
}

/*void Board::applyChanges() {
    // Copying saved array to main
    for (int i=0; i < height*width; ++i) {
        cells[i].applyChanges();
    }
}

void Board::blitNormal(const Window& _window, SDL_FRect _cellRect) const {
    for (int y=1; y < height-1; ++y) {
        for (int x=1; x < width-1; ++x) {
            cells[y*width+x].blitNormal(_window, _cellRect);
            _cellRect.x += _cellRect.w;
        }
        _cellRect.x -= _cellRect.w * (width-2);
        _cellRect.y += _cellRect.h;
    }
}

void Board::blitThermal(const Window& _window, SDL_FRect _cellRect) const {
    for (int y=1; y < height-1; ++y) {
        for (int x=1; x < width-1; ++x) {
            cells[y*width+x].blitThermal(_window, _cellRect);
            _cellRect.x += _cellRect.w;
        }
        _cellRect.x -= _cellRect.w * (width-2);
        _cellRect.y += _cellRect.h;
    }
}

void Board::blitPressure(const Window& _window, SDL_FRect _cellRect) const {
    for (int y=1; y < height-1; ++y) {
        for (int x=1; x < width-1; ++x) {
            cells[y*width+x].blitPressure(_window, _cellRect);
            _cellRect.x += _cellRect.w;
        }
        _cellRect.x -= _cellRect.w * (width-2);
        _cellRect.y += _cellRect.h;
    }
}

void Board::blitLines(const Window& _window, SDL_FRect _cellRect) const {
    // Draw separating lines
    _window.setDrawColor(BLACK);
    // Fining opposite side
    const SDL_FPoint oppositeP = {_cellRect.x + _cellRect.w*(width-2),
        _cellRect.y + _cellRect.h*(height-2)};

    // Vertical lines
    float x = _cellRect.x;
    for (int i=0; i < width-1; ++i) {
        _window.drawLine(x, _cellRect.y, x, oppositeP.y);
        x += _cellRect.w;
    }

    // Horizontal lines
    float y = _cellRect.y;
    for (int i=0; i < height-1; ++i) {
        _window.drawLine(_cellRect.x, y, oppositeP.x, y);
        y += _cellRect.h;
    }
}*/

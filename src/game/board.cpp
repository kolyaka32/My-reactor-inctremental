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
    return width-2;
}

int Board::getHeight() const {
    return height-2;
}

void Board::setCell(SDL_Point _pos, const Cell _cell) {
    cells[_pos.y*width+_pos.x].state = _cell.state;
}

void Board::resetCell(SDL_Point _pos) {
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
}

void Board::update() {
    // Vertical interactions
    for (int y=0; y < height-1; ++y) {
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

    // Updating cells by it special behevior
    for (int y=1; y < height-1; ++y) {
        for (int x=1; x < width-1; ++x) {
            switch (cells[y*width+x].state) {
            case Cell::VentUp:
                cells[y*width+x].vent(cells[(y+1)*width+x], cells[(y-1)*width+x]);
                break;

            case Cell::VentRight:
                cells[y*width+x].vent(cells[y*width+x-1], cells[y*width+x+1]);
                break;

            case Cell::VentDown:
                cells[y*width+x].vent(cells[(y-1)*width+x], cells[(y+1)*width+x]);
                break;

            case Cell::VentLeft:
                cells[y*width+x].vent(cells[y*width+x+1], cells[y*width+x-1]);
                break;

            case Cell::Heater:
                cells[y*width+x].applyTemperature(1.0);

            case Cell::ValveUp:
                cells[y*width+x].exchangeValved(cells[(y+1)*width+x], cells[(y-1)*width+x]);
                break;

            case Cell::ValveRight:
                cells[y*width+x].exchangeValved(cells[y*width+x-1], cells[y*width+x+1]);
                break;

            case Cell::ValveDown:
                cells[y*width+x].exchangeValved(cells[(y-1)*width+x], cells[(y+1)*width+x]);
                break;

            case Cell::ValveLeft:
                cells[y*width+x].exchangeValved(cells[y*width+x+1], cells[y*width+x-1]);
                break;

            case Cell::CoolerUp:
                cells[y*width+x].cool(cells[(y+1)*width+x], cells[(y-1)*width+x]);
                break;

            case Cell::CoolerRight:
                cells[y*width+x].cool(cells[y*width+x-1], cells[y*width+x+1]);
                break;

            case Cell::CoolerDown:
                cells[y*width+x].cool(cells[(y-1)*width+x], cells[(y+1)*width+x]);
                break;

            case Cell::CoolerLeft:
                cells[y*width+x].cool(cells[y*width+x+1], cells[y*width+x-1]);
                break;

            default:
                break;
            }
        }
    }

    // Resetting side cells to global parameters
    for (int y=0; y < height; ++y) {
        cells[y*width].exchange();  // Left cells
        cells[y*width+width-1].exchange();  // Right cells
    }
    for (int x=1; x < width-1; ++x) {
        cells[x].exchange();  // Upper cells
        cells[height*width-x-1].exchange();  // Bottom cells
    }
}

void Board::applyChanges() {
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
}

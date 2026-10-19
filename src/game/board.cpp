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
    verChanels = new Channel[height*width];
    upRightChannels = new Channel[height*width];
    downRightChannels = new Channel[height*width];

    reset();
}

Board::~Board() noexcept {
    delete[] cells;
    delete[] verChanels;
    delete[] upRightChannels;
    delete[] downRightChannels;
}

void Board::reset() {
    for (int i=0; i < height*width; ++i) {
        cells[i].reset();
        verChanels[i].reset();
        upRightChannels[i].reset();
        downRightChannels[i].reset();
    }
}

int Board::getWidth() const {
    return width;
}

int Board::getHeight() const {
    return height;
}

bool Board::isLegimate(SDL_Point _pos) const {
    return _pos.x > 1 && _pos.x < width-1 &&
        _pos.y > 1 && _pos.y < height-1;  // ! Check end positions
}

const Cell& Board::operator[](SDL_Point _pos) const {
    return cells[_pos.y*width+_pos.x];
}

void Board::projectUnit(Channel& _c1, Channel& _c2, Channel& _c3, Channel& _c4,
    Channel& _c5, Channel& _c6) {
    // Check deviation
    float dev = (_c1.sp - _c2.sp + _c3.sp - _c4.sp + _c5.sp - _c6.sp)/6;

    // Avaraging deviation spread between all channels
    _c1.tryAdd(-dev);
    _c2.tryAdd(dev);
    _c3.tryAdd(-dev);
    _c4.tryAdd(dev);
    _c5.tryAdd(-dev);
    _c6.tryAdd(dev);
}

void Board::applyForces() {
    // Place forces from external places and cells
}

void Board::project() {
    // Save of mass
    // Odd cells
    for (int y=1; y < height-1; ++y) {
        for (int x=1; x < width-1; x+=2) {
            projectUnit(
                verChanels[y*width+x],
                verChanels[(y-1)*width+x],
                upRightChannels[y*width+x],
                upRightChannels[y*width+x-1],
                downRightChannels[y*width+x],
                downRightChannels[(y-1)*width+x-1]
            );
        }
    }
    // Even cells
    for (int y=1; y < height-1; ++y) {
        for (int x=2; x < width-1; x+=2) {
            projectUnit(
                verChanels[y*width+x],
                verChanels[(y-1)*width+x],
                upRightChannels[y*width+x],
                upRightChannels[(y+1)*width+x-1],
                downRightChannels[y*width+x],
                downRightChannels[y*width+x-1]
            );
        }
    }
}

void Board::advect() {
    // Transfer of fluid (save momentum)
}

/*void Board::update() {
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

    // Resetting side cells to global parameters
    for (int y=0; y < height; ++y) {
        cells[y*width].exchange();  // Left cells
        cells[y*width+width-1].exchange();  // Right cells
    }
    for (int x=1; x < width-1; ++x) {
        cells[x].exchange();  // Upper cells
        cells[height*width-x-1].exchange();  // Bottom cells
    }
}*/

/*void Board::applyChanges() {
    // Copying saved array to main
    for (int i=0; i < height*width; ++i) {
        cells[i].applyChanges();
    }
}*/

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
    verChanels = new Channel[(height+1)*width];
    // ! Recalc
    upLeftChanels = new Channel[(height+1)*width];
    downLeftChanels = new Channel[(height+1)*width];

    reset();
}

Board::~Board() noexcept {
    delete[] cells;
    delete[] verChanels;
    delete[] upLeftChanels;
    delete[] downLeftChanels;
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
    for (int y=0; y < height; ++y) {
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
}*/

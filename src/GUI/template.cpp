/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"


GUI::Template::Template(const Window& _window) noexcept
: window(_window) {}

void GUI::Template::move(float X, float Y) {}

void GUI::Template::blit() const {}

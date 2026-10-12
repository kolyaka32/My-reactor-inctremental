/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"

#if (USE_SDL_FONT) && (PRELOAD_FONTS)


GUI::TypeField::TypeField(const Window& _window, const TextArgument&& _arg,
    size_t _maxLength, const char* _startText) noexcept
: TextureTemplate(_window),
argument(std::move(_arg)),
font(window.createFontCopy(_arg.font, _arg.height)),
maxLength(_maxLength),
inverseTexture(nullptr) {
    // Setting position of text rects
    rect.h = TTF_GetFontHeight(font);
    rect.y = window.getHeight()*argument.Y - rect.h*argument.verAli/2;
    inversedRectDest = {0.0, rect.y, 0.0, rect.h};
    caretRect = {0.0, rect.y, 2.0, rect.h};
    inversedRectSrc = {0.0, 0.0, 0.0, rect.h};

    setString(_startText);
}

GUI::TypeField::TypeField(TypeField&& _object) noexcept
: TextureTemplate(std::move(_object)),
argument(std::move(_object.argument)),
font(_object.font),
maxLength(_object.length),
length(_object.length),
caret(_object.caret),
needSwapCaret(_object.needSwapCaret),
selectLength(_object.selectLength),
showCaret(_object.showCaret),
caretRect(_object.caretRect),
inversedRectDest(_object.inversedRectDest),
inversedRectSrc(_object.inversedRectSrc),
inverseTexture(_object.inverseTexture),
pressed(_object.pressed),
selected(_object.selected) {
    // Copying previous text
    memcpy(buffer, _object.buffer, maxLength);
}

GUI::TypeField::~TypeField() noexcept {
    // Check, if not moved
    if (texture) {
        // Clearing rest texture
        SDL_DestroyTexture(texture);
        SDL_DestroyTexture(inverseTexture);
    }
    if (font) {
        // Clearing font
        TTF_CloseFont(font);
    }
}

void GUI::TypeField::updateTexture() {
    // Checking, if string exsist
    if (length) {
        // Clearing previous
        if (texture) {
            SDL_DestroyTexture(texture);
            SDL_DestroyTexture(inverseTexture);
        }

        // Creating main text texture
        SDL_Surface* mainSurface = TTF_RenderText_Shaded(font, buffer, length,
            argument.textColor, argument.backColor);
        texture = window.createTextureAndFree(mainSurface);

        // Create inversed text texture
        SDL_Surface* inversedSurface = TTF_RenderText_Shaded(font, buffer, length,
            argument.backColor, argument.textColor);
        inverseTexture = window.createTextureAndFree(inversedSurface);

        // Resetting place of text with saving aligment
        rect = argument.getRect(window, texture);
    }
    updateSelected();
}

void GUI::TypeField::updateSelected() {
    if (length) {
        // Update caret place
        if (caret) {
            int caretX = 0;
            TTF_GetStringSize(font, buffer, caret, &caretX, nullptr);
            caretRect.x = rect.x + caretX - 1;
        } else {
            caretRect.x = rect.x - 1;
        }

        // Inversing selected part of text, if need
        if (selectLength) {
            // Getting start position and length of selected part
            int startPosition, length;
            if (selectLength > 0) {
                if (caret == 0) {
                    startPosition = 0;
                } else {
                    TTF_GetStringSize(font, buffer, caret, &startPosition, nullptr);
                }
                TTF_GetStringSize(font, buffer+caret, selectLength, &length, nullptr);
            } else {
                if (-selectLength == caret) {
                    startPosition = 0;
                } else {
                    TTF_GetStringSize(font, buffer, caret + selectLength, &startPosition, nullptr);
                }
                TTF_GetStringSize(font, buffer+caret+selectLength, -selectLength, &length, nullptr);
            }
            inversedRectSrc.x = startPosition;
            inversedRectDest.x = inversedRectSrc.x + rect.x;
            inversedRectDest.w = inversedRectSrc.w = length;
        }
    } else {
        caretRect.x = argument.X*window.getWidth() - 1;
    }
}

bool GUI::TypeField::writeString(const char* _str) {
    if (!selected) {
        return false;
    }
    // Resetting
    pressed = false;
    deleteSelected();

    // Counting codepoints
    int codelen = 0;
    for (int i=0; i < length;) {
        if (int len = getNextChar(buffer+i)) {
            codelen++;
            i += len;
        } else {
            i++;
        }
    }
    // Parsing recieved text
    for (const char* c = _str; *c && (*c != '\n');) {
        // Check codepoint length
        if (int len = getNextChar(c)) {
            // Moving all after codepoint
            for (int i = length; i > caret; --i) {
                buffer[i + len - 1] = buffer[i-1];
            }
            memcpy(buffer+caret, c, len);
            length += len;
            caret += len;
            c += len;
            codelen++;
            if (codelen == maxLength) {
                break;
            }
        } else {
            c++;
        }
    }
    updateTexture();
    return true;
}

void GUI::TypeField::writeClipboard() {
    // Getting and writing clipboard to caret
    char* clippboard = SDL_GetClipboardText();
    writeString(clippboard);
    SDL_free(clippboard);
}

void GUI::TypeField::copyToClipboard() {
    if (selectLength) {
        // Static memory for write clipbpard
        static char clipboardText[100];
        if (selectLength < 0) {
            memcpy(clipboardText, buffer + caret + selectLength, min(-selectLength, (int)sizeof(clipboardText)));
        } else {
            memcpy(clipboardText, buffer + caret, min(selectLength, (int)sizeof(clipboardText)));
        }
        clipboardText[abs(selectLength)] = '\0';
        SDL_SetClipboardText(clipboardText);
    }
}

void GUI::TypeField::deleteSelected() {
    if (selectLength) {
        if (selectLength < 0) {
            for (size_t i=caret; i < length; ++i) {
                buffer[i + selectLength] = buffer[i];
            }
            caret += selectLength;
            length += selectLength;
        } else {
            for (size_t i=caret; i < length - selectLength; ++i) {
                buffer[i] = buffer[i + selectLength];
            }
            length -= selectLength;
        }
        selectLength = 0;
    }
}

GUI::Code GUI::TypeField::type(SDL_Keycode _code) {
    // Checking, if box selected
    if (!selected) {
        return None;
    }
    // Getting current shft and control state
    SDL_Keymod keyMods = SDL_GetModState();

    // Additional actions for control commands
    if (keyMods & SDL_KMOD_CTRL) {
        switch (_code) {
        case SDLK_BACKSPACE:
            // Coping after caret
            if (selectLength == 0) {
                if (caret == 0) {
                    return Some;
                }
                selectLength = getPrevBlock(buffer+caret, -caret);
            }
            deleteSelected();
            break;

        case SDLK_DELETE:
            // Coping after caret
            if (selectLength == 0) {
                if (caret == length) {
                    return Some;
                }
                selectLength = getNextBlock(buffer+caret, length-caret);
            }
            deleteSelected();
            break;

        case SDLK_LEFT:
            if (keyMods & SDL_KMOD_SHIFT) {
                if (caret > 0) {
                    int offset = getPrevBlock(buffer+caret, -caret);
                    caret += offset;
                    selectLength -= offset;
                }
            } else {
                if (selectLength) {
                    if (selectLength < 0) {
                        caret += selectLength;
                    }
                    selectLength = 0;
                } else if (caret > 0) {
                    caret += getPrevBlock(buffer+caret, -caret);
                }
            }
            updateSelected();
            return Some;

        case SDLK_RIGHT:
            if (keyMods & SDL_KMOD_SHIFT) {
                if (caret < length) {
                    int offset = getNextBlock(buffer+caret, length-caret);
                    caret += offset;
                    selectLength -= offset;
                }
            } else {
                if (selectLength) {
                    if (selectLength > 0) {
                        caret += selectLength;
                    }
                    selectLength = 0;
                } else if (caret < length) {
                    caret += getNextBlock(buffer+caret, length-caret);
                }
            }
            updateSelected();
            return Some;

        case SDLK_V:
            writeClipboard();
            break;

        case SDLK_C:
            copyToClipboard();
            break;

        case SDLK_X:
            copyToClipboard();
            deleteSelected();
            break;

        case SDLK_A:
            // Selecing all text
            caret = length;
            selectLength = -length;
            break;

        default:
            return None;
        }
        // Updating texture after modifiying text
        updateTexture();
        return Some;
    }
    // Normal switching for extra inputs
    switch (_code) {
    case SDLK_BACKSPACE:
        if (selectLength == 0) {
            if (caret == 0) {
                return Some;
            }
            selectLength = getPrevChar(buffer+caret);
        }
        deleteSelected();
        break;

    case SDLK_DELETE:
        if (selectLength == 0) {
            if (caret == length) {
                return Some;
            }
            selectLength = getNextChar(buffer+caret);
        }
        deleteSelected();
        break;

    case SDLK_LEFT:
        if (keyMods & SDL_KMOD_SHIFT) {
            if (caret > 0) {
                int offset = getPrevChar(buffer+caret);
                caret += offset;
                selectLength -= offset;
            }
        } else {
            if (selectLength) {
                if (selectLength < 0) {
                    caret += selectLength;
                }
                selectLength = 0;
            } else if (caret > 0) {
                caret += getPrevChar(buffer+caret);
            }
        }
        updateSelected();
        return Some;

    case SDLK_RIGHT:
        if (keyMods & SDL_KMOD_SHIFT) {
            if (caret < length) {
                int offset = getNextChar(buffer+caret);
                caret += offset;
                selectLength -= offset;
            }
        } else {
            if (selectLength) {
                if (selectLength > 0) {
                    caret += selectLength;
                }
                selectLength = 0;
            } else if (caret < length) {
                caret += getNextChar(buffer+caret);
            }
        }
        updateSelected();
        return Some;

    case SDLK_END:
    case SDLK_PAGEDOWN:
        if (keyMods & SDL_KMOD_SHIFT) {
            selectLength += caret - length;
        } else {
            selectLength = 0;
        }
        caret = length;
        updateSelected();
        return Some;

    case SDLK_HOME:
    case SDLK_PAGEUP:
        if (keyMods & SDL_KMOD_SHIFT) {
            selectLength += caret;
        } else {
            selectLength = 0;
        }
        caret = 0;
        updateSelected();
        return Some;

    case SDLK_PASTE:
        writeClipboard();
        break;

    case SDLK_COPY:
        copyToClipboard();
        break;

    case SDLK_CUT:
        copyToClipboard();
        deleteSelected();
        break;

    case SDLK_ESCAPE:
        if (selectLength) {
            selectLength = 0;
            updateSelected();
        } else {
            // Stop entering
            // Resetting selection
            selected = false;
            pressed = false;
            showCaret = false;
            // Stoping entering any letters
            window.stopTextInput();
            return Finished;
        }
        break;

    case SDLK_RETURN:
    case SDLK_RETURN2:
        // Stop entering
        updateSelected();
        // Resetting selection
        selected = false;
        pressed = false;
        selectLength = 0;
        showCaret = false;
        // Stoping entering any letters
        window.stopTextInput();
        // Return action
        return Activate;

    default:
        return None;
    }
    // Updating texture after modifiying text
    updateTexture();
    return Some;
}

bool GUI::TypeField::checkOff(const Mouse _mouse) {
    if (selected && !in(_mouse)) {
        // Resetting selection
        selected = false;
        pressed = false;

        // Stoping entering any letters
        window.stopTextInput();

        // Clearing caret
        showCaret = false;
        selectLength = 0;

        updateSelected();

        // Return, that finish text input
        return true;
    }
    return false;
}

GUI::Code GUI::TypeField::click(const Mouse _mouse) {
    if (in(_mouse)) {
        // Resetting values
        pressed = true;
        showCaret = true;
        selectLength = 0;
        if (!selected) {
            // Starting using keyboard
            window.startTextInput();
            selected = true;
        }

        // Getting current mouse position at text
        if (length) {
            TTF_MeasureString(font, buffer, length, _mouse.getX()-rect.x, NULL, &caret);
        } else {
            caret = 0;
        }
        // Showing caret
        updateSelected();
        return Some;
    }
    return None;
}

void GUI::TypeField::unclick() {
    pressed = false;
}

void GUI::TypeField::move(float _X, float _Y) {
    TextureTemplate::move(_X, _Y);
    argument.X += _X;
    argument.Y += _Y;
    caretRect.x += _X*window.getWidth();
    caretRect.y += _Y*window.getHeight();
    inversedRectDest.x += _X*window.getWidth();
    inversedRectDest.y += _Y*window.getHeight();
}

void GUI::TypeField::update(float _mouseX) {
    if (pressed) {
        size_t measure;
        if (length) {
            TTF_MeasureString(font, buffer, length, _mouseX-rect.x, NULL, &measure);
        } else {
            measure = 0;
        }
        selectLength += caret - measure;
        caret = measure;
        updateSelected();
    }
    if (selected && getTime() > needSwapCaret) {
        // Inversing show state
        showCaret ^= true;

        // Update timer
        needSwapCaret = getTime() + 400;
    }
}

void GUI::TypeField::blit() const {
    // Rendering main text
    if (length) {
        window.blit(texture, rect);
    }

    // Rendering selection text (as reversed)
    if (selectLength) {
        window.blit(inverseTexture, &inversedRectDest, &inversedRectSrc);
    }

    // Rendering caret
    if (showCaret) {
        window.setDrawColor({50, 50, 50, 50});
        window.drawRect(caretRect);
    }
}

const char* GUI::TypeField::getString() {
    buffer[length] = '\0';
    return buffer;
}

void GUI::TypeField::setString(const char* _newString) {
    // resetting flags
    selected = false;
    pressed = false;
    showCaret = false;
    length = 0;
    selectLength = 0;

    window.stopTextInput();

    if (_newString == nullptr) {
        return;
    }

    // Counting actual codepoints
    int codeCount = 0;
    // Parsing new text
    for (const char* c = _newString; *c && (*c != '\n');) {
        // Check codepoint length
        if (int len = getNextChar(c)) {
            // Copying codepoint
            memcpy(buffer + length, c, len);
            length += len;
            c += len;
            // Check on max length
            codeCount++;
            if (codeCount == maxLength) {
                break;
            }
        } else {
            c++;
        }
    }
    updateTexture();
}

int GUI::TypeField::getNextChar(const char* _str) const {
    // Mask: 0yyyyyyy
    if ((_str[0] & 0b10000000) == 0b0) {
        return 1;
    }
    // Mask: 110yyyyy 10yyyyyyyy
    if ((_str[1] & 0b11000000) != 0b10000000) {
        return 0;
    }
    if ((_str[0] & 0b11100000) == 0b11000000) {
        return 2;
    }
    // Mask: 1110yyyy 10yyyyyyyy 10yyyyyyyy
    if ((_str[2] & 0b11000000) != 0b10000000) {
        return 0;
    }
    if ((_str[0] & 0b11110000) == 0b11100000) {
        return 3;
    }
    // Mask: 11110yyy 10yyyyyyyy 10yyyyyyyy 10yyyyyyyy
    if ((_str[3] & 0b11000000) != 0b10000000) {
        return 0;
    }
    if ((_str[0] & 0b11111000) == 0b11110000) {
        return 4;
    }
    // In other - error
    return 0;
}

int GUI::TypeField::getPrevChar(const char* _str) const {
    // Mask: 0yyyyyyy
    if ((_str[-1] & 0b10000000) == 0b0) {
        return -1;
    }
    // Mask: 110yyyyy 10yyyyyyyy
    if ((_str[-1] & 0b11000000) != 0b10000000) {
        return 0;
    }
    if ((_str[-2] & 0b11100000) == 0b11000000) {
        return -2;
    }
    // Mask: 1110yyyy 10yyyyyyyy 10yyyyyyyy
    if ((_str[-2] & 0b11000000) != 0b10000000) {
        return 0;
    }
    if ((_str[-3] & 0b11110000) == 0b11100000) {
        return -3;
    }
    // Mask: 11110yyy 10yyyyyyyy 10yyyyyyyy 10yyyyyyyy
    if ((_str[-3] & 0b11000000) != 0b10000000) {
        return 0;
    }
    if ((_str[-4] & 0b11111000) == 0b11110000) {
        return -4;
    }
    // In other - error
    return 0;
}

bool GUI::TypeField::isSpec(char _c) const {
    return (_c==' ') || (_c==',') || (_c=='.') || (_c=='/') || (_c=='\\');
}

int GUI::TypeField::getNextBlock(const char* _str, int _len) const {
    int i=0;
    if (isSpec(_str[0])) {
        // Move by spaces
        for (;isSpec(_str[i])&&(i<_len);++i) {}
    } else {
        // Move by charachters
        for (;!isSpec(_str[i])&&(i<_len);++i) {}
    }
    return i;
}

int GUI::TypeField::getPrevBlock(const char* _str, int _len) const {
    int i=0;
    if (isSpec(_str[-1])) {
        // Move by spaces
        for (;isSpec(_str[i-1])&&(i>_len);--i) {}
    } else {
        // Move by charachters
        for (;!isSpec(_str[i-1])&&(i>_len);--i) {}
    }
    return i;
}

#endif  // (USE_SDL_FONT) && (PRELOAD_FONTS)

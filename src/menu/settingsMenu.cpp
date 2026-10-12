/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "settingsMenu.hpp"
#include "../data/cycleTemplate.hpp"


SettingsMenu::SettingsMenu(const Window& _window)
: SubWindow(_window, 0.5, 0.5, 0.42, 0.75),
settingButton{_window, 0.97, 0.045, 0.04, Textures::SettingsButton},
//titleText{window, 0.5, 0.13, {"Pause", "Пауза", "Pause", "Паўза"},
//    2, GUI::Aligment::Midle, Height::Info},
titleText{_window, {"Pause", "Пауза"}, {0.5, 0.16, .frame=2, .height=GUI::Title}},
flags {
    {_window, 0.4, 0.3, 0.16, Textures::FlagUSA},
    {_window, 0.6, 0.3, 0.16, Textures::FlagRUS},
    //{_window, 0.35, 0.45, 0.25, Textures::FlagGER},
    //{_window, 0.65, 0.45, 0.25, Textures::FlagBEL},
},
#if (PRELOAD_MUSIC)
musicText{_window, {"Music", "Музыка"}, {0.5, 0.6, .frame=1}},
//musicText{_window, {"Music", "Музыка", "Die Musik", "Музыка"}, {0.5, 0.6, .frame=1}},
musicSlider{_window, 0.5, 0.64, 0.2, audio.music.getVolume()},
#endif
#if (PRELOAD_SOUNDS)
soundText{_window, {"Sounds", "Звуки"}, {0.5, 0.68, .frame=1}},
//soundText{_window, {"Sounds", "Звуки", "Geräusche", "Гук"}, {0.5, 0.68, .frame=1}},
soundSlider{_window, 0.5, 0.72, 0.2, audio.sounds.getVolume()},
#endif
exitButton{_window, {"Exit", "Выход"}, {0.5, 0.78, .frame=1}},
closeButton{_window, {"Close", "Закрыть"}, {0.5, 0.83, .frame=1}}
//closeButton{_window, {"Close", "Закрыть", "Ausfahrt", "Выхад"}, {0.5, 0.82, .frame=1}}
{}

bool SettingsMenu::click(const Mouse _mouse) {
    // Check, if click on setting butoon
    if (settingButton.in(_mouse)) {
        toggle();
        return true;
    }
    // Clicking in menu
    if (active) {
        if (exitButton.in(_mouse)) {
            App::setNextCycle(Cycle::None);
            CycleTemplate::stop();
            close();
            return true;
        }
        if (closeButton.in(_mouse)) {
            close();
            return true;
        }
        // Check on changing language
        for (unsigned i = 0; i < (unsigned)Language::Count; ++i) {
            if (flags[i].in(_mouse)) {
                if (LanguagedText::setLanguage((Language)i)) {
                    // Changing language
                    window.updateTitle();
                    // Restarting game
                    CycleTemplate::restart();
                    return true;
                }
            }
        }
        // Resetting holding object
        holdingSlider = 0;
        #if (PRELOAD_MUSIC)
        if (musicSlider.in(_mouse)) {
            holdingSlider = 1;
            return true;
        }
        #endif
        #if (PRELOAD_SOUNDS)
        if (soundSlider.in(_mouse)) {
            holdingSlider = 2;
            return true;
        }
        #endif
        return true;
    }
    return false;
}

void SettingsMenu::unClick() {
    if (active) {
        // Resetting selected box
        holdingSlider = 0;
    }
}

bool SettingsMenu::scroll(const Mouse _mouse, float _wheelY) {
    if (active) {
        // Checking scroll on sliders
        #if (PRELOAD_MUSIC)
        if (musicSlider.in(_mouse)) {
            audio.music.setVolume(musicSlider.scroll(_wheelY));
            return true;
        }
        #endif
        #if (PRELOAD_SOUNDS)
        if (soundSlider.in(_mouse)) {
            audio.sounds.setVolume(soundSlider.scroll(_wheelY));
            return true;
        }
        #endif
        return true;
    }
    return false;
}

void SettingsMenu::update() {
    if (active) {
        // Creating and finding mouse position
        Mouse mouse;
        mouse.updatePos();

        // Updating pressing on sliders
        switch (holdingSlider) {
        // Music slier
        #if (PRELOAD_MUSIC)
        case 1:
            audio.music.setVolume(musicSlider.setValue(mouse.getX()));
            break;
        #endif

        // Sound slider
        #if (PRELOAD_SOUNDS)
        case 2:
            // Updating sound slider state
            audio.sounds.setVolume(soundSlider.setValue(mouse.getX()));

            // Playing sound effect for understanding loud
            if (getTime() > nextSound) {
                audio.sounds.play(Sounds::SliderSound);
                nextSound = getTime() + 400;
            }
            break;
        #endif

        default:
            break;
        }
    }
}

void SettingsMenu::blit() const {
    // Draw pause button
    settingButton.blit();

    // Drawing menu if need
    if (active) {
        background.blit();
        titleText.blit();

        // Blitting language buttons
        for (unsigned i = 0; i < unsigned(Language::Count); ++i) {
            flags[i].blit();
        }
        // Music slider
        #if (PRELOAD_MUSIC)
        musicText.blit();
        musicSlider.blit();
        #endif
        // Sound slier
        #if (PRELOAD_SOUNDS)
        soundSlider.blit();
        soundText.blit();
        #endif
        // Buttons
        closeButton.blit();
        exitButton.blit();
    }
}

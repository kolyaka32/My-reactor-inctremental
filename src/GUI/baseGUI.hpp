/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include <vector>
#include <array>
#include "../data/app.hpp"


// Namespace of objects for GUI (Graphic User Interface)
namespace GUI {
    // Text aligment type
    enum Aligment : int {
        Left = 0,
        Up = 0,
        Midle = 1,
        Right = 2,
        Down = 2,
    };


    // Codes of actions, returning from functions
    enum {
        None,      // Noting happen
        Some,      // Action with current object
        Finished,  // Finished interacting with current object (ESCAPE button)
        Activate,  // Finished interaction and activate next action (ENTER button)
        Button1,   // Pressed 1 button
        Button2,   // Pressed 2 button
        Button3,   // Pressed 3 button
        // Etc..
    };

    // Type for returning code of action
    typedef int Code;


    // Object, that can be drawn at screen
    class Template {
     protected:
        const Window& window;

     public:
        Template(const Window& window) noexcept;
        // Move whole object, in part of screen
        virtual void move(float X, float Y);
        // Draw this object at screen
        virtual void blit() const;
    };


    // Object with texture, that can be drawn
    class TextureTemplate : public Template {
     protected:
        SDL_Texture* texture;
        SDL_FRect rect;

     public:
        TextureTemplate(const Window& window, SDL_Texture* texture = nullptr);
        TextureTemplate(const Window& window, SDL_FRect rect, SDL_Texture* texture = nullptr);
        TextureTemplate(TextureTemplate&& object) noexcept;
        void move(float X, float Y) override;
        void blit() const override;
        virtual bool in(const Mouse mouse) const;
    };


    // Class of rounded backplate for better understability
    class RoundedBackplate : public TextureTemplate {
     public:
        RoundedBackplate(const Window& window, float centerX, float centerY, float width, float height,
            int radius, int border, Color frontColor = GREY, Color backColor = BLACK) noexcept;
        RoundedBackplate(const Window& window, const SDL_FRect& rect, int radius, int border,
            Color frontColor = GREY, Color backColor = BLACK) noexcept;
        RoundedBackplate(RoundedBackplate&& object) noexcept;
        ~RoundedBackplate() noexcept;
    };


    // Class with rectangular backplate for typeBox
    class RectBackplate : public TextureTemplate {
     public:
        RectBackplate(const Window& window, float centerX, float centerY, float width, float height,
            float border, Color frontColor = GREY, Color backColor = BLACK) noexcept;
        RectBackplate(const Window& window, const SDL_FRect& rect,
            float border, Color frontColor = GREY, Color backColor = BLACK) noexcept;
        RectBackplate(const RectBackplate& copyObject) noexcept;
        RectBackplate(RectBackplate&& moveObject) noexcept;
        ~RectBackplate() noexcept;
    };


    // Class for sub menu with flag of openning
    class SubWindow : public Template {
     protected:
        // Flag of showing
        bool active;
        // Graphical part
        GUI::RoundedBackplate background;

     public:
        SubWindow(const Window& window, float X, float Y, float W, float H) noexcept;
        SubWindow(SubWindow&& object) noexcept;
        void open();
        void close();
        void toggle();
        bool isOpen() const;
        virtual void reset();
        virtual bool escape();
        void move(float X, float Y) override;
        void blit() const override;
    };


    #if (USE_SDL_IMAGE) && (PRELOAD_TEXTURES)
    // Class of slider bar with point on it to control need parameter
    class Slider : public TextureTemplate {
     private:
        SDL_Texture* buttonTexture;  // Texture of line (upper part of slider)
        SDL_FRect buttonRect;        // Place for rendering upper part

     public:
        // Create slide with need line and button images
        Slider(const Window& window, float X, float Y, float width, float startValue,
            Textures lineImage = Textures::SliderLine, Textures buttonImage = Textures::SliderButton);
        Slider(Slider&& object) noexcept;
        float setValue(float mouseX);  // Setting new state from mouse position
        float scroll(float wheelY);    // Checking mouse wheel action
        void move(float X, float Y) override;
        void blit() const override;
    };


    // Class of buttons with image on it
    class ImageButton : public TextureTemplate {
     public:
        ImageButton(const Window& window, float X, float Y, float width, Textures name) noexcept;
        ImageButton(ImageButton&& object) noexcept;
    };
    #endif  // (USE_SDL_IMAGE) && (PRELOAD_TEXTURES)


    #if (USE_SDL_IMAGE) && (PRELOAD_ANIMATIONS)
    class Animation : public TextureTemplate {
     private:
        const Animations type;
        const IMG_Animation* animation;
        unsigned frame = 0;
        timer prevTick;

     public:
        Animation(const Window& window, float X, float Y, float width, float height, Animations type);
        Animation(const Window& window, const SDL_FRect& destination, Animations type);
        Animation(Animation&& object) noexcept;
        ~Animation() noexcept;
        void update();
    };
    #endif  // (USE_SDL_IMAGE) && (PRELOAD_ANIMATIONS)


    // Text part
    #if (USE_SDL_FONT) && (PRELOAD_FONTS)
    // Heights of text (for uniformity)
    enum Height : int {
        Title = 40,     // Intuding title
        SubTitle = 28,  // Title in subcycles
        Info = 32,
        Main = 24,      // Main text
        Medium = 20,
        Small = 12,     // Small text
    };

    // Struct for easier store and change arguments for text classes
    struct TextArgument {
        float X, Y;

        int frame = 0;  // 0 if hasn't, value if offset
        Fonts font = Fonts::Main;
        Height height = Height::Main;
        Aligment horAli = Aligment::Midle;  // horizontal aligment
        Aligment verAli = Aligment::Midle;  // vertical aligment
        Color textColor = WHITE;
        Color backColor = BLACK;

        template <typename ...Args>
        SDL_Texture* createTexture(const Window& window, const LanguagedText& texts, Args... args) const;
        SDL_Texture* createTexture(const Window& window, const char* text) const;
        SDL_FRect getRect(const Window& window, const SDL_Texture* texture) const;
        SDL_FRect getRect(const Window& window, float W, float H) const;
    };

    template <typename ...Args>
    SDL_Texture* TextArgument::createTexture(const Window& window, const LanguagedText& texts, Args... args) const {
        // Getting text with arguments
        char buffer[100];
        SDL_snprintf(buffer, sizeof(buffer), texts.getString().c_str(), args...);
        // Creating text itself
        return createTexture(window, buffer);
    }


    // Static text (not changing in runtime)
    class StaticText : public TextureTemplate {
     public:
        template <typename ...Args>
        StaticText(const Window& window, const LanguagedText&& texts,
            const TextArgument&& arguments, Args... args) noexcept
        : TextureTemplate(window) {
            texture = arguments.createTexture(window, std::move(texts), args...);
            rect = arguments.getRect(window, texture);
        }
        StaticText(StaticText&& object) noexcept;
        ~StaticText() noexcept;
    };


    // Dynamicly updated text
    class DynamicText : public TextureTemplate {
     private:
        const LanguagedText texts;
        const TextArgument argument;

     public:
        DynamicText(const Window& window, const LanguagedText&& texts,
            const TextArgument&& arguments) noexcept;
        DynamicText(DynamicText&& object) noexcept;
        ~DynamicText() noexcept;
        template <typename ...Args>
        void setValues(Args&& ...args) {
            window.destroy(texture);  // Clearing previous
            texture = argument.createTexture(window, texts, args...);
            rect = argument.getRect(window, texture);
        }
    };


    // Class of field, where user can type text
    class TypeField : public TextureTemplate {
     protected:
        TextArgument argument;

        // Variables
        TTF_Font* font;               // Font (mostly for text measurement)
        char buffer[100];             // String, that was typed
        const size_t maxLength;       // Maximal size of typing zone (excluding \0)
        size_t length = 0;            // Length of buffer text (in bytes)
        size_t caret = 0;             // Position of place, where user type
        timer needSwapCaret = 0;      // Time, when next need to change caret
        int selectLength = 0;         // Length of selected box

        SDL_FRect caretRect;          // Place, where caret should be at screen
        SDL_FRect inversedRectDest;   // Rect of inversed selected text, where should be drawn
        SDL_FRect inversedRectSrc;    // Part of text, that should be reversed (relative)
        SDL_Texture* inverseTexture;  // Texture of inversed selected box
        bool showCaret = false;       // Flag, if need to show caret
        bool pressed = false;         // Flag if currently mouse is pressed and selecting text
        bool selected = false;        // Flag if currently typing in this field

        void updateTexture();         // Creat new texture of updated text
        void updateSelected();        // Update reversed rect position (selected part)
        void deleteSelected();        // Clearing selected part
        void writeClipboard();        // Write clipboard content after caret
        void copyToClipboard();       // Writing selected text to clipboard
        bool isSpec(char c) const;    // Return if special character ( ,./\*-+)
        int getNextChar(const char* str) const;  // Return next utf8 codepoint length
        int getPrevChar(const char* str) const;  // Return previous utf8 codepoint length (signed)
        int getNextBlock(const char* str, int length) const;  // Return length of next block (of chars/spaces)
        int getPrevBlock(const char* str, int length) const;  // Return length of previous block (of chars/spaces)

     public:
        TypeField(const Window& window, const TextArgument&& arguments,
            size_t length = 16, const char* start = "") noexcept;
        TypeField(TypeField&& object) noexcept;
        ~TypeField() noexcept;
        const char* getString();             // Return typed string
        void setString(const char* string);  // Replace text with new string
        // Main cycle
        bool writeString(const char* str);   // Write string to buffer at caret position
        Code type(SDL_Keycode code);         // Processing special keycodes (like arrows, home, CTRL-C...)   
        void update(float mouseX);           // Highlated area of typing
        bool checkOff(const Mouse mouse);    // Check if click in other place, true if end entering
        Code click(const Mouse mouse);       // Set caret for typing at specified place
        void unclick();                      // Reset pressing
        void move(float X, float Y) override;  // Move current box
        void blit() const override;          // Draw current text with selection at screen
    };


    // Object for type text with backplate for visability
    class TypeBox : public TypeField {
     private:
        GUI::RectBackplate backplate;

     public:
        TypeBox(const Window& window, const TextArgument&& arguments,
            size_t len = 16, const char* start = "") noexcept;
        TypeBox(TypeBox&& object) noexcept;
        bool in(const Mouse mouse) const override;
        void move(float X, float Y) override;
        void blit() const override;
    };


    // Class of buttons with text on it
    class TextButton : public StaticText {
     private:
        GUI::RoundedBackplate backplate;

     public:
        TextButton(const Window& window, const LanguagedText&& texts, const TextArgument&& arguments);
        TextButton(TextButton&& object) noexcept;
        void move(float X, float Y) override;
        void blit() const override;
    };


    // Object for selecting variants from list
    class SwitchBox : Template {
     private:
        unsigned selected = 0;
        bool opened = false;

        // Draw options
        const float height;
        SDL_FRect background;
        const SDL_Color backColor;
        std::vector<StaticText> drawnTexts;
        SDL_Texture* arrowTexture;
        SDL_FRect arrowRect;

     public:
        SwitchBox(const Window& window, const TextArgument argument, float W,
            std::initializer_list<LanguagedText> texts, unsigned startOption = 0) noexcept;
        SwitchBox(SwitchBox&& object) noexcept;
        void set(unsigned value);
        unsigned getValue() const;
        Code click(const Mouse mouse);
        void move(float X, float Y) override;
        void blit() const override;
    };


    // Class of appearing for time and hidden by time text
    class InfoBox : public StaticText {
     private:
        timer endTime;  // Time, when stop showing
        const timer decayTime;  // Time of full decay

     public:
        InfoBox(const Window& window, const LanguagedText&& texts,
            const TextArgument&& arguments, unsigned decayTime = 500) noexcept;
        InfoBox(InfoBox&& object) noexcept;
        void update();
        void reset();
    };


    // Class for box with message and actions with it
    class OneOptionBox : public SubWindow {
     private:
        GUI::StaticText title;
        GUI::TextButton button;

     public:
        OneOptionBox(const Window& window, const TextArgument&& argument, float W, float H,
            const LanguagedText&& titleText, const LanguagedText&& buttonText) noexcept;
        OneOptionBox(OneOptionBox&& object) noexcept;
        Code click(const Mouse mouse);
        void move(float X, float Y) override;
        void blit() const override;
    };


    // Class for box with message and actions with it
    class TwoOptionBox : public SubWindow {
     private:
        GUI::StaticText title;
        GUI::TextButton button1, button2;

     public:
        TwoOptionBox(const Window& window, float X, float Y, float W, float H,
            const LanguagedText&& titleText, const LanguagedText&& button1Text,
            const LanguagedText&& button2Text) noexcept;
        TwoOptionBox(TwoOptionBox&& object) noexcept;
        Code click(const Mouse mouse);
        void move(float X, float Y) override;
        void blit() const override;
    };

    #endif  // (USE_SDL_FONT) && (PRELOAD_FONTS)

    // Menu for scrolling items
    template <class Item, class SourceItem>
    class ScrollBox : public Template {
     protected:
        // Parameters of showed list
        const int maxItems;  // Total number of elements, showing max at one screen
        int startField = 0;  // Position, from which show
        int endField = 0;    // Position, up to showing
        const float blockPos;  // Start Y position of blocks (relative)
        const float blockHeight;  // Height of one block (relative)
        // Items itself in reverse order for easier appending
        std::vector<Item> items;
        // Adding text of absence of objects
        #if (USE_SDL_FONT) && (PRELOAD_FONTS)
        GUI::StaticText emptySavesText;
        #endif

        // Slider for showing position
        SDL_FRect sliderRect;
        SDL_FRect sliderBackRect;
        bool holding = false;
        float holdPosition;

        void moveUp();
        void moveDown();
        void placeItem(int pos, const SourceItem& item);

     public:
        // Create menu for scrolling objects, placed at center with (X, Y) and size.
        // Shows "maxShowedItems" items at a time
        ScrollBox(const Window& window, float X, float Y, float W, float H,
            int maxShowedItems, const LanguagedText&& emptyItemsText) noexcept;
        ScrollBox(const Window& window, float X, float Y, float W, float H,
            int maxShowedItems, std::vector<SourceItem> items, const LanguagedText&& emptyItemsText) noexcept;
        ScrollBox(ScrollBox&& object) noexcept;
        ~ScrollBox() noexcept;
        void addItem(const SourceItem& field);
        void clear();
        // Interaction
        Code click(const Mouse mouse);
        void unclick();
        void update(const Mouse mouse);
        bool scroll(const Mouse mouse, float wheelY);
        void move(float X, float Y) override;
        void blit() const override;
    };

}  // namespace GUI

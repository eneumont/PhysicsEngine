#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
Rectangle editorRect;

ncEditorData_t ncEditorData;

Vector2 anchor01 = { 900, 10 };
Texture2D cursorTexture;

bool WindowBox000Active = true;

void InitEditor() {
    GuiLoadStyle("raygui/styles/lavanda/style_lavanda.rgs");

    Image i = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(i);
    UnloadImage(i);
    HideCursor();

    ncEditorData.SliderBar003Value = 2;
    ncEditorData.SliderBar001Value = 0.1f;
    ncEditorData.SliderBar002Value = 1;
}

void UpdateEditor(Vector2 position) {
    //
}

void DrawEditor(Vector2 position) {
    if (WindowBox000Active) {
        WindowBox000Active = !GuiWindowBox((Rectangle) { anchor01.x + 0, anchor01.y + 0, 288, 696 }, "SAMPLE TEXT");
        GuiSliderBar((Rectangle) { anchor01.x + 88, anchor01.y + 48, 120, 16 }, "Mass Min", NULL, & ncEditorData.SliderBar001Value, 0, 100);
        GuiSliderBar((Rectangle) { anchor01.x + 88, anchor01.y + 112, 120, 16 }, "Mass Max", NULL, & ncEditorData.SliderBar002Value, 0, 100);
        GuiSliderBar((Rectangle) { anchor01.x + 88, anchor01.y + 160, 120, 16 }, "Gravitation", NULL, & ncEditorData.SliderBar003Value, 0, 100);
    }

    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, GREEN);

    GuiUnlock();
}
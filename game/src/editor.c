#include "editor.h"
#include "body.h"
#include "render.h"
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

    /*ncEditorData.SliderBar003Value = 2;
    ncEditorData.SliderBar001Value = 0.1f;
    ncEditorData.SliderBar002Value = 1;*/

    ncEditorData.anchor01 = (Vector2){ 808, 24 };
    ncEditorData.anchor02 = (Vector2){ 816, 72 };
    ncEditorData.anchor03 = (Vector2){ 816, 264 };

    ncEditorData.EditorBoxActive = true;
    ncEditorData.BodyTypeEditMode = false;
    ncEditorData.DampingValue = 0.0f;
    ncEditorData.GravitationValue = 0.0f;
    ncEditorData.BodyTypeActive = 0;
    ncEditorData.GravityScaleValue = 0.0f;
    ncEditorData.MassMaxValue = 0.0f;
    ncEditorData.MassMinValue = 0.0f;

    editorRect = (Rectangle){ ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 304, 616 };
}

void UpdateEditor(Vector2 position) {
    //
}

void DrawEditor(Vector2 position) {
    if (ncEditorData.BodyTypeEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive) {
        ncEditorData.EditorBoxActive = !GuiWindowBox((Rectangle) { ncEditorData.anchor01.x + -16, ncEditorData.anchor01.y + 0, 288, 656 }, "SAMPLE TEXT");
        GuiGroupBox((Rectangle) { ncEditorData.anchor02.x + 0, ncEditorData.anchor02.y + 0, 232, 168 }, "Body");
        GuiSliderBar((Rectangle) { ncEditorData.anchor02.x + 80, ncEditorData.anchor02.y + 64, 120, 16 }, "Mass Min", NULL, & ncEditorData.MassMinValue, 0.1, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor02.x + 80, ncEditorData.anchor02.y + 88, 120, 16 }, "Mass Max", NULL, & ncEditorData.MassMaxValue, 0.1, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor02.x + 80, ncEditorData.anchor02.y + 112, 120, 16 }, "Damping", NULL, & ncEditorData.DampingValue, 0.1, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor02.x + 80, ncEditorData.anchor02.y + 136, 120, 16 }, "Gravity Scale", NULL, & ncEditorData.GravityScaleValue, 0, 20);
        GuiGroupBox((Rectangle) { ncEditorData.anchor03.x + 0, ncEditorData.anchor03.y + 0, 232, 96 }, "World");
        GuiSliderBar((Rectangle) { ncEditorData.anchor03.x + 80, ncEditorData.anchor03.y + 16, 120, 16 }, "Gravitation", NULL, & ncEditorData.GravitationValue, 0, 20);
        if (GuiDropdownBox((Rectangle) { ncEditorData.anchor02.x + 64, ncEditorData.anchor02.y + 24, 120, 24 }, "Dynamic;Kinematic;Static", & ncEditorData.BodyTypeActive, ncEditorData.BodyTypeEditMode)) ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;
    }

    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, GREEN);
    
    GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position) {
    for (ncBody* body = bodies; body; body = body->next)  {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass))) {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position) {
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}
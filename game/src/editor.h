#pragma once
#include "raylib.h"

typedef struct ncEditorData {
    Vector2 anchor01;

    bool EditorBoxActive;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float MassValue;
    float kValue;
    float DampingValue;
    float GravityScaleValue;
    float GravityValue;
    float GravitationValue;
    float RestitutionValue;
    float TimestepValue;
    bool ResetBtnPressed;
    bool SimulateBtnActive;
} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor();

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position); 
void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);
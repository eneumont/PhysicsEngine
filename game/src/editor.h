#pragma once
#include "raylib.h"

typedef struct ncEditorData {
	float SliderBar001Value;
	float SliderBar002Value;
	float SliderBar003Value;
} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor();
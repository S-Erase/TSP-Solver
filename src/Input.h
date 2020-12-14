#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include "Selection.h"
#include "Vertex.h"

class Input
{
private:
	//Mouse variables
	static int mx0, my0, mx1, my1;
	static SDL_Rect mrect;
	static int mx, my;
	static bool mousedown;

	//Keyboard variables
	static bool shiftpressed;
	static bool ctrlpressed;
	static bool altpressed;
	static bool shiftpressed2;
	static bool ctrlpressed2;
	static bool altpressed2;
public:
	static inline int GetMouseX() { return mx; }
	static inline int GetMouseY() { return my; }
	static inline bool IsMousePressed() { return mousedown; }
	static inline int GetMouseXAtMousePress() { return mx0; }
	static inline int GetMouseYAtMousePress() { return my0; }
	static inline SDL_Rect* GetMouseRectanglePointer() { return &mrect; }

	static inline bool IsShiftPressed() { return shiftpressed; }
	static inline bool IsCtrlPressed() { return ctrlpressed; }
	static inline bool IsAltPressed() { return altpressed; }
	static inline bool WasShiftPressedAtMousePress() { return shiftpressed2; }
	static inline bool WasCtrlPressedAtMousePress() { return ctrlpressed2; }
	static inline bool WasAltPressedAtMousePress() { return altpressed2; }

	static void Update(SDL_Event& EV);
	static bool IsMouseInRectangle(SDL_Rect rect);
	static bool IsVertexInMouseRectangle(Vertex<int> v);
	static bool IsMouseInRoundedRectangle(SDL_Rect rect, double p);
};

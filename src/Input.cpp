#include "Input.h"

//Mouse variables
int Input::mx0 = -1, Input::my0 = -1, Input::mx1 = -1, Input::my1 = -1;
SDL_Rect Input::mrect = { -1,-1,0,0 };
int Input::mx = -1, Input::my = -1;
bool Input::mousedown = false;

//Keyboard variables
bool Input::shiftpressed = false;
bool Input::ctrlpressed = false;
bool Input::altpressed = false;
bool Input::shiftpressed2 = false;
bool Input::ctrlpressed2 = false;
bool Input::altpressed2 = false;

void Input::Update(SDL_Event& EV) {
	switch (EV.type) {
	case SDL_KEYDOWN:
		if (EV.key.state == SDL_PRESSED) {
			switch (EV.key.keysym.sym) {
			case SDLK_LSHIFT:
				shiftpressed = true;
				break;
			case SDLK_LCTRL:
				ctrlpressed = true;
				break;
			case SDLK_LALT:
				altpressed = true;
				break;
			}
		}
		break;

	case SDL_KEYUP:
		if (EV.key.state == SDL_RELEASED) {
			switch (EV.key.keysym.sym) {
			case SDLK_LSHIFT:
				shiftpressed = false;
				break;
			case SDLK_LCTRL:
				ctrlpressed = false;
				break;
			case SDLK_LALT:
				altpressed = false;
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		mousedown = false;

		mx0 = -1, my0 = -1, mx1 = -1, my1 = -1;
		mrect = { -1,-1,0,0 };

		shiftpressed2 = false;
		ctrlpressed2 = false;
		altpressed2 = false;
		break;
	case SDL_MOUSEBUTTONDOWN:
		mousedown = true;

		mx0 = EV.button.x;
		my0 = EV.button.y;
		mx1 = EV.button.x;
		my1 = EV.button.y;
		mrect = { mx0,my0,0,0 };

		if (shiftpressed) { shiftpressed2 = true; }
		if (ctrlpressed) { ctrlpressed2 = true; }
		if (altpressed) { altpressed2 = true; }
		hovered_vertex2 = hovered_vertex;
		break;
	case SDL_MOUSEMOTION:
		if (mousedown) {
			mx1 = EV.button.x;
			my1 = EV.button.y;
			mrect = { std::min(mx0,mx1),std::min(my0,my1),abs(mx0 - mx1),abs(my0 - my1) };
		}
		mx = EV.button.x;
		my = EV.button.y;
		break;
	}
}

bool Input::IsMouseInRectangle(SDL_Rect rect) {
	bool resx = mx >= rect.x && mx <= (rect.x + rect.w);
	bool resy = my >= rect.y && my <= (rect.y + rect.h);
	return resx && resy;
}
bool Input::IsVertexInMouseRectangle(Vertex<int> v) {
	bool resx = v.x >= mrect.x && v.x <= (mrect.x + mrect.w);
	bool resy = v.y >= mrect.y && v.y <= (mrect.y + mrect.h);
	return resx && resy;
}

bool Input::IsMouseInRoundedRectangle(SDL_Rect rect, double p) //p = 1 (Diamond), 2 (Ellipse), ->inf (->Rectangle)
{
	double dx = mx - (double)rect.x - (double)rect.w / 2, dy = my - (double)rect.y - (double)rect.h / 2;
	bool res = pow(abs(dx) * 2 / rect.w, p) + pow(abs(dy) * 2 / rect.h, p) <= 1;
	return res;
}
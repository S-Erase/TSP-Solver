#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <iostream>

#define WINDOW_WIDTH window->GetWindowWidth()
#define WINDOW_HEIGHT window->GetWindowHeight()

// Pointers to our window and surface
class Window {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	int width, height;
public:
	Window(const char* title, int w, int h)
		: width(w), height(h)
	{
		int windowflags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, windowflags);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
		if (!renderer) {
			std::cout << "Error Creating Renderer: " << SDL_GetError() << std::endl;
			system("pause");
			abort();
		}
	}
	~Window()
	{
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
		SDL_DestroyWindow(window);
		window = NULL;
	}

	void Update() { SDL_GetWindowSize(window, &width, &height); }
	inline int GetWindowWidth() const { return width; }
	inline int GetWindowHeight() const { return height; }
	inline SDL_Window* GetWindow() const { return window; }
	inline SDL_Renderer* GetRenderer() const { return renderer; }
};

extern std::unique_ptr<Window> window;
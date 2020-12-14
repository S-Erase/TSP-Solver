#pragma once
#include <cmath>
#include <iomanip>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <sstream>
#include "Window.h"
#include "Graph.h"
#include "Selection.h"

//Render variables
extern double window_pos[2];
extern double window_scale;


extern SDL_Color COLOR_DEFAULT;
extern SDL_Color COLOR_HOVERED;
extern SDL_Color COLOR_SELECTED;
extern SDL_Color COLOR_SELECTED_HOVERED;

extern bool show_grid;
extern unsigned char vertex_render_mode; //0:Nothing, 1:Coordinates on selected, 2:Coordinates on all

extern SDL_Rect Menu_toggle_rect0;
extern SDL_Rect Menu_toggle_rect1;
extern SDL_Rect Menu_toggle_rect2;
extern SDL_Rect Menu_toggle_rect3;
extern SDL_Rect Menu_toggle_rect4;

extern SDL_Rect Menu_main_rect5; //Path length display box

class Texture {
public:
	Texture(const char* filepath) {
		SDL_Surface* surface = IMG_Load(filepath);
		m_Texture = SDL_CreateTextureFromSurface(window->GetRenderer(), surface);
		if (!m_Texture) {
			std::cout << "Error loading texture: " << SDL_GetError() << std::endl;
			system("pause");
			abort();
		}
		SDL_FreeSurface(surface);
		surface = NULL;
	}
	~Texture() {
		SDL_DestroyTexture(m_Texture);
		m_Texture = NULL;
	}
	SDL_Texture* GetTextureID() const { return m_Texture; }
private:
	SDL_Texture* m_Texture;
};

class Quad {
public:
	Quad(std::shared_ptr<Texture> texture, SDL_Rect src, SDL_Rect dest)
		: m_texture(texture), srcrect(src), destrect(dest)
	{}
	inline void Render() const { SDL_RenderCopy(window->GetRenderer(), m_texture->GetTextureID(), &srcrect, &destrect); }
	inline void SetSourceX(int newX) { srcrect.x = newX; }
	inline void SetSourceY(int newY) { srcrect.y = newY; }
private:
	std::shared_ptr<Texture> m_texture;
	SDL_Rect srcrect;
	SDL_Rect destrect;
};

extern std::shared_ptr<Texture> vertex_texture;
extern std::shared_ptr<Texture> font_texture;
extern std::shared_ptr<Texture> Menu_toggle_texture;
extern std::shared_ptr<Texture> Menu_buttons_texture;

void SDL_SetRenderDrawColor(SDL_Renderer* render, SDL_Color color);
void SDL_SetTextureColorMod(std::shared_ptr<Texture> texture, SDL_Color color);

signed char sgn(double num);

void Render_String(std::string str, int x, int y, SDL_Color color);

Vertex<double> ViewtoWorld(Vertex<int> inV);
Vertex<int> WorldtoView(Vertex<double> inV);

void Render_Coordinates(Vertex<int> v0, unsigned int prec, SDL_Color color);
void Render_Coordinates2(double num1, double num2, unsigned int prec, int x, int y, SDL_Color color);

void Render_Number(double number, unsigned int prec, int x, int y, SDL_Color color);


void RenderGrid();

//Rendering the Graph
void RenderVertex(unsigned int vi, unsigned int prec, SDL_Color color);

void RenderEdge(unsigned int v0i, unsigned int v1i, bool render_weight);

void RenderGraph();

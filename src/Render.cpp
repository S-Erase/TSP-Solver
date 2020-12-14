#include "Render.h"

//Render variables
double window_pos[2] = { 0,0 };
double window_scale = 1;

std::shared_ptr<Texture> vertex_texture;
std::shared_ptr<Texture> font_texture;
std::shared_ptr<Texture> Menu_toggle_texture;
std::shared_ptr<Texture> Menu_buttons_texture;

SDL_Color COLOR_DEFAULT = { 16, 32, 128, 255 };
SDL_Color COLOR_HOVERED = { 0, 16, 64, 255 };
SDL_Color COLOR_SELECTED = { 255, 128, 0, 255 };
SDL_Color COLOR_SELECTED_HOVERED = { 224, 96, 0, 255 };

bool show_grid = true;
unsigned char vertex_render_mode = 0; //0:Nothing, 1:Coordinates on selected, 2:Coordinates on all

SDL_Rect Menu_toggle_rect0 = { 0,0,30,30 };
SDL_Rect Menu_toggle_rect1 = { 0,0,15,30 };
SDL_Rect Menu_toggle_rect2 = { 30,0,30,30 };
SDL_Rect Menu_toggle_rect3 = { 180,0,30,30 };
SDL_Rect Menu_toggle_rect4 = { 180,0,15,30 };

SDL_Rect Menu_main_rect5 = { 180,210,180,30 }; //Path length display box

void SDL_SetRenderDrawColor(SDL_Renderer* render, SDL_Color color) {
	SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
}
void SDL_SetTextureColorMod(std::shared_ptr<Texture> texture, SDL_Color color) {
	SDL_SetTextureColorMod(texture->GetTextureID(), color.r, color.g, color.b);
}

signed char sgn(double num) {
	if (num > 0) { return 1; }
	else if (num < 0) { return -1; }
	else { return 0; }
}

void Render_String(std::string str, int x, int y, SDL_Color color = { 0, 0, 0, 0 }) {
	if (color.a != 0)
		SDL_SetTextureColorMod(font_texture->GetTextureID(), color.r, color.g, color.b);
	SDL_Rect glyph_destrect = { x,y,8,12 };
	SDL_Rect glyph_srcrect = { 0,0,8,12 };
	for (char &ch: str) {
		glyph_srcrect.x = 8 * (ch % 16);
		glyph_srcrect.y = 12 * floor(ch / 16) - 24;
		SDL_RenderCopy(window->GetRenderer(), font_texture->GetTextureID(), &glyph_srcrect, &glyph_destrect);
		glyph_destrect.x += 6;
	}
}

Vertex<double> ViewtoWorld(Vertex<int> inV)
{
	Vertex<double> outV = { (inV.x - (double)WINDOW_WIDTH / 2) / window_scale + window_pos[0], (inV.y - (double)WINDOW_HEIGHT / 2) / window_scale + window_pos[1] };
	return outV;
}

Vertex<int> WorldtoView(Vertex<double> inV)
{
	Vertex<int> outV = { (inV.x - window_pos[0]) * window_scale + WINDOW_WIDTH / 2, (inV.y - window_pos[1]) * window_scale + WINDOW_HEIGHT / 2 };
	return outV;
}
void Render_Coordinates(Vertex<double> v0, unsigned int prec, SDL_Color color = { 0, 0, 0, 0 }) {
	std::string vertexstr = "";
	std::ostringstream coordx, coordy;
	coordx << std::fixed << std::setprecision(prec) << v0.x;
	coordy << std::fixed << std::setprecision(prec) << v0.y;
	vertexstr = "(" + coordx.str() + "," + coordy.str() + ")";

	Vertex<int> renderv0 = WorldtoView(v0);
	Render_String(vertexstr, renderv0.x - 3 * vertexstr.size(), renderv0.y - 18, color);
}

void Render_Coordinates2(double num1, double num2, unsigned int prec, int x, int y, SDL_Color color = { 0,0,0,0 }) {
	std::string vertexstr = "";
	std::ostringstream coordx, coordy;
	coordx << std::fixed << std::setprecision(prec) << num1;
	coordy << std::fixed << std::setprecision(prec) << num2;
	vertexstr = "(" + coordx.str() + "," + coordy.str() + ")";

	Render_String(vertexstr, x, y, color);
}

void Render_Number(double number, unsigned int prec, int x, int y, SDL_Color color = { 0,0,0,0 }) {
	std::ostringstream strs;
	strs << std::fixed << std::setprecision(prec) << number;
	std::string str = strs.str();

	Render_String(str, x, y, color);
}

void RenderGrid() {
	double scale = log10(WINDOW_HEIGHT) - log10(window_scale);
	double gridsize;
	int x2, y2;

	Vertex<double> v0 = ViewtoWorld({ 0, 0 });
	Vertex<double> v1 = ViewtoWorld({ WINDOW_WIDTH, WINDOW_HEIGHT });

	//Subgrid
	gridsize = pow(10, floor(scale - 0.8));
	if (WINDOW_HEIGHT < 20 * gridsize * window_scale) {
		SDL_SetRenderDrawColor(window->GetRenderer(), { 224,224,224, 255 });
		for (int i = floor(v0.x / gridsize); i < v1.x/ gridsize; i++) {
			x2 = (i * gridsize - window_pos[0]) * window_scale + (double)WINDOW_WIDTH / 2;
			SDL_RenderDrawLine(window->GetRenderer(), x2, 0, x2, WINDOW_HEIGHT);
		}
		for (int i = floor(v0.y / gridsize); i < v1.y / gridsize; i++) {
			y2 = (i * gridsize - window_pos[1]) * window_scale + (double)WINDOW_HEIGHT / 2;
			SDL_RenderDrawLine(window->GetRenderer(), 0, y2, WINDOW_WIDTH, y2);
		}
	}

	//Grid
	gridsize = pow(10, floor(scale + 0.2));
	SDL_SetRenderDrawColor(window->GetRenderer(), { 192, 192, 192, 255 });
	for (int i = ceil(v0.x / gridsize); i < v1.x / gridsize; i++) {
		x2 = (i * gridsize - window_pos[0]) * window_scale + (double)WINDOW_WIDTH / 2;
		SDL_RenderDrawLine(window->GetRenderer(), x2, 0, x2, WINDOW_HEIGHT);
	}
	for (int i = ceil(v0.y / gridsize); i < v1.y / gridsize; i++) {
		y2 = (i * gridsize - window_pos[1]) * window_scale + (double)WINDOW_HEIGHT / 2;
		SDL_RenderDrawLine(window->GetRenderer(), 0, y2, WINDOW_WIDTH, y2);
	}
	int prec = 2;
	if (floor(scale + 0.2) < -2) 
		prec = -floor(scale + 0.2);
	SDL_SetTextureColorMod(font_texture, { 192, 192, 192, 255 });
	for (int i = floor(v0.x / gridsize); i < v1.x / gridsize; i++) {
		for (int j = floor(v0.y / gridsize); j < v1.y / gridsize; j++) {
			x2 = (i * gridsize - window_pos[0]) * window_scale + (double)WINDOW_WIDTH / 2;
			y2 = (j * gridsize - window_pos[1]) * window_scale + (double)WINDOW_HEIGHT / 2;
			Render_Coordinates2(i * gridsize, j * gridsize, prec, x2, y2 - 12);
		}
	}

	//Axes
	x2 = -window_pos[0] * window_scale + (double)WINDOW_WIDTH / 2;
	y2 = -window_pos[1] * window_scale + (double)WINDOW_HEIGHT / 2;
	SDL_SetRenderDrawColor(window->GetRenderer(), { 128,128,128, 255 });
	SDL_RenderDrawLine(window->GetRenderer(), x2, 0, x2, WINDOW_HEIGHT);
	SDL_RenderDrawLine(window->GetRenderer(), 0, y2, WINDOW_WIDTH, y2);
}

//Rendering the Graph
void RenderVertex(unsigned int vi, unsigned int prec, SDL_Color color) {
	Vertex<double> v0 = main_graph.get_vertex(vi);
	Vertex<int> renderv0 = WorldtoView(v0);

	if (-3 < renderv0.x && renderv0.x < WINDOW_WIDTH+3 && -3 < renderv0.y && renderv0.y < WINDOW_HEIGHT+3) {
		if (vertex_render_mode == 2 || (vertex_render_mode == 1 && IsVertexSelected(vi))) 
			Render_Coordinates(v0, prec, color);
		SDL_SetTextureColorMod(vertex_texture->GetTextureID(), color.r, color.g, color.b);
		SDL_Rect dest = { renderv0.x - 2, renderv0.y - 2,5,5 };
		SDL_RenderCopy(window->GetRenderer(), vertex_texture->GetTextureID(), NULL, &dest);
	}
}

void RenderEdge(unsigned int v0i, unsigned int v1i, bool render_weight) {
	Vertex<double> v0 = main_graph.get_vertex(v0i), v1 = main_graph.get_vertex(v1i);

	Vertex<int> renderv0 = WorldtoView(v0), renderv1 = WorldtoView(v1);

	SDL_RenderDrawLine(window->GetRenderer(), renderv0.x, renderv0.y, renderv1.x, renderv1.y);
	if (render_weight) {
		int x2, y2, len, dx, dy;
		x2 = (renderv0.x + renderv1.x) / 2, y2 = (renderv0.y + renderv1.y) / 2;
		len = sqrt(pow(renderv1.x - renderv0.x, 2) + pow(renderv1.y - renderv0.y, 2));
		if (len) {
			dx = 6;
			dy = -12;
		}
		else {
			dx = 8 * (renderv1.y - renderv0.y) / len, dy = 8 * (renderv0.x - renderv1.x) / len;
			dy *= sgn(dx);
			dx *= sgn(dx);
			if (dy < 0)
				dy -= 12;
		}
		unsigned int prec = 2;
		double weight_mag = log10(abs(main_graph.get_weight(v0i, v1i)));
		if (weight_mag < -1) 
			prec = 1 - floor(weight_mag);
		Render_Number(main_graph.get_weight(v0i, v1i), prec, x2 + dx, y2 + dy);
		
	}
}

void RenderGraph() {
	unsigned int N = main_graph.vertex_count(), M = selected_vertices.size();

	//Renders vertices
	double window_logscale = log10(WINDOW_HEIGHT) - log10(window_scale) + 0.2;
	unsigned int prec = 2;
	if (floor(window_logscale) < -1) 
		prec = 1 - floor(window_logscale);

	//Renders edges
	if (selected_vertices.size() == 2) {
		SDL_SetRenderDrawColor(window->GetRenderer(), COLOR_SELECTED);
		SDL_SetTextureColorMod(font_texture, COLOR_SELECTED);
		unsigned int v0 = selected_vertices[0], v1 = selected_vertices[1];
		RenderEdge(v0, v1, true);
	}

	for (unsigned int i = 0; i < N; i++) {
		if (i == hovered_vertex) {
			if (IsVertexSelected(i))
				RenderVertex(i, prec, COLOR_SELECTED_HOVERED);
			else
				RenderVertex(i, prec, COLOR_HOVERED);
		}
		else {
			if (IsVertexSelected(i))
				RenderVertex(i, prec, COLOR_SELECTED);
			else 
				RenderVertex(i, prec, COLOR_DEFAULT);
		}
	}
}

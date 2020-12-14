#include "Menu.h"
#include "TSPAlgorithms.h"

int ControlMode = CONTROL_CREATION; //-1:Locked, 0:Creation Mode, 1:Selection Mode, 2:Number entry, 3:Read Only
bool Editable = true;

bool menu_hovered = false; //When mouse hovers over menu
bool menu_hovered2 = false; //When mouse hovered over menu last LMouse was pressed
bool menu_toggle_hovered = false;

//Variables for computing the fps
Uint32 FPS_startt, FPS_endt;

MenuButton::MenuButton(int rectx, int recty, std::function<void()> func)
	: button_func(func)
{
	src_rect.x = rectx;
	src_rect.y = recty;
}
//Button functions

void ChangeControls()
{
	ControlMode++;
	ControlMode %= 2;
	button_controls.SetSourceY(ControlMode * 30);
	selected_vertices.clear();
}
void ChangeCoordSettings()
{
	vertex_render_mode++;
	vertex_render_mode %= 3;
	button_coords.SetSourceY(60 + vertex_render_mode * 30);
}
void ToggleGrid()
{
	show_grid = !show_grid;
	button_grid.SetSourceY(150 + show_grid * 30);
}
void ToggleWeightMenu()
{
	if (WeightMenu.active) //Turn off
	{
		WeightMenu.active = false;
		button_weight.SetSourceX(180);
	}
	else if (Editable) //Turn on
	{
		WeightMenu.active = true;
		button_weight.SetSourceX(360);
	}
}
void ResetView()
{
	window_pos[0] = 0;
	window_pos[1] = 0;
	window_scale = 1;
}
void ClearGraph()
{
	if (Editable) {
		main_graph.clear_graph();
		selected_vertices.clear();
	}
}
void ChangeSelectionSettings()
{
	SELECTION_MODE++;
	SELECTION_MODE %= 4;
	button_selection.SetSourceY(210 + SELECTION_MODE * 30);
}
void StartTSP()
{
	//In multiple frames
	if (!TSPrunning) {
		if (Editable) {
			TSP_startt = SDL_GetTicks();
			//Restricts user from editing graph
			Editable = false;
			selected_vertices.clear();
			WeightMenu.active = false;
			button_weight.SetSourceX(540);

			//Computes optimum path
			TSPrunning = true;
			TSPperm = {};
			TSPalg2 = 0;
		}
		else {
			Editable = true;
		}
	}
}

void SetNorm(GraphNorm newnorm)
{
	GraphNorm norm = main_graph.get_norm();
	main_graph.set_norm(newnorm);
	button_norms[(int)norm]->SetSourceX(180);
	button_norms[(int)newnorm]->SetSourceX(360);
}

//Buttons
MenuButton button_controls(0, 0, ChangeControls);
MenuButton button_coords(0, 60, ChangeCoordSettings);
MenuButton button_grid(0, 180, ToggleGrid);
MenuButton button_weight(180, 150, ToggleWeightMenu);
MenuButton button_view(540, 30, ResetView);
MenuButton button_clear(540, 60, ClearGraph);
MenuButton button_selection(0, 210, ChangeSelectionSettings);
MenuButton button_TSP(540, 0, StartTSP);

MenuButton button_weight_euclid(360, 0, std::bind(SetNorm, GraphNorm::Euclidean));
MenuButton button_weight_taxicab(180, 30, std::bind(SetNorm, GraphNorm::Taxicab));
MenuButton button_weight_maximum(180, 60, std::bind(SetNorm, GraphNorm::Maximum));

std::vector<MenuButton*> button_norms;

void Menu::AddButton(MenuButton& button) {
	buttons.push_back(&button);
	dest_rects.push_back({ (int)pos.x, (int)pos.y + (int)buttoncount*30, 180, 30 });
	buttoncount++;
}

void const Menu::Render() {
	if (active)
	{
		for (unsigned int i = 0; i < buttoncount; i++)
			SDL_RenderCopy(window->GetRenderer(), Menu_buttons_texture->GetTextureID(), &(buttons[i]->src_rect), &dest_rects[i]);
	}
}

bool const Menu::IsMouseoverMenu()
{
	return active && (Input::GetMouseX() > pos.x && Input::GetMouseX() < pos.x + 180 && Input::GetMouseY() > pos.y && Input::GetMouseY() < pos.y + buttoncount * 30);
}

void Menu::UpdateonMouseDown()
{
	//Is mouse hovering over menu
	if (IsMouseoverMenu())
	{
		int buttonid = (Input::GetMouseY() - pos.y) / 30;
		PressedButton = buttons[buttonid];
	}
	else
		PressedButton = nullptr;
}

void Menu::UpdateonMouseUp()
{
	if (PressedButton != nullptr)
	{
		PressedButton->button_func();
		PressedButton = nullptr;
	}
}

//Menus
Menu MainMenu(0, 0);
Menu ViewMenu(180,30);
Menu EditMenu(180,60);
Menu WeightMenu(180,90);

void Load_Menu() {
	Menu_toggle_texture = std::make_shared<Texture>("src/img/menu_toggle.png");
	SDL_SetTextureAlphaMod(Menu_toggle_texture->GetTextureID(), 192);

	Menu_buttons_texture = std::make_shared<Texture>("src/img/menu_buttons.png");
	SDL_SetTextureAlphaMod(Menu_buttons_texture->GetTextureID(), 192);

	MainMenu.AddButton(button_controls);
	MainMenu.AddButton(button_coords);
	MainMenu.AddButton(button_grid);
	MainMenu.AddButton(button_weight);
	MainMenu.AddButton(button_view);
	MainMenu.AddButton(button_clear);
	MainMenu.AddButton(button_selection);
	MainMenu.AddButton(button_TSP);

	button_norms.reserve(3);
	button_norms.emplace_back(&button_weight_euclid);
	button_norms.emplace_back(&button_weight_taxicab);
	button_norms.emplace_back(&button_weight_maximum);

	WeightMenu.AddButton(button_weight_euclid);
	WeightMenu.AddButton(button_weight_taxicab);
	WeightMenu.AddButton(button_weight_maximum);
}

void RenderMenu() {
	if (!MainMenu.active) {
		SDL_RenderCopy(window->GetRenderer(), Menu_toggle_texture->GetTextureID(), &Menu_toggle_rect0, &Menu_toggle_rect0);
	}
	else {
		SDL_RenderCopy(window->GetRenderer(), Menu_toggle_texture->GetTextureID(), &Menu_toggle_rect2, &Menu_toggle_rect3);
		SDL_SetRenderDrawColor(window->GetRenderer(), 213, 213, 213, 192);
	}
	MainMenu.Render();
	WeightMenu.Render();
}
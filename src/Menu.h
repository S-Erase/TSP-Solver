#pragma once
#include <thread>
#include <mutex>
#include <functional>
#include "Graph.h"
#include "Render.h"
#include "Input.h"

#define CONTROL_CREATION	0
#define CONTROL_SELECTION	1

extern int ControlMode; //-1:Locked, 0:Creation Mode, 1:Selection Mode, 2:Number entry, 3:Read Only
extern bool Editable;

extern bool menu_hovered; //When mouse hovers over menu
extern bool menu_hovered2; //When mouse hovered over menu last LMouse was pressed
extern bool menu_toggle_hovered;


class MenuButton {
private:
public:
	SDL_Rect src_rect = { 0,0,180,30 };
	std::function<void()> button_func = nullptr;

	MenuButton(int rectx, int recty, std::function<void()> func);
	void SetSourceX(int x) { src_rect.x = x; }
	void SetSourceY(int y) { src_rect.y = y; }
};

class Menu
{
public:
	bool active = false;
	inline Menu(int x, int y) { pos = { x,y }; }

	void AddButton(MenuButton& button);
	void const Render();
	bool const IsMouseoverMenu();
	void UpdateonMouseDown();
	void UpdateonMouseUp();
private:
	std::vector<MenuButton*> buttons = {};
	std::vector<SDL_Rect> dest_rects = {};
	SDL_Rect m_Hitbox = {};
	Vertex<int> pos = { 0,0 };
	unsigned int buttoncount = 0;

	//Used for UpdateonMouse(Down/Up)()
	MenuButton* PressedButton = nullptr;
};

//Button functions

void ChangeControls();
void ChangeCoordSettings();
void ToggleGrid();
void ToggleWeightMenu();
void ResetView();
void ClearGraph();
void ChangeSelectionSettings();
void StartTSP();

//Buttons
extern MenuButton button_controls;
extern MenuButton button_coords;
extern MenuButton button_grid;
extern MenuButton button_weight;
extern MenuButton button_view;
extern MenuButton button_clear;
extern MenuButton button_selection;
extern MenuButton button_TSP;

extern std::vector<MenuButton*> button_norms;

extern MenuButton button_weight_euclid;
extern MenuButton button_weight_taxicab;
extern MenuButton button_weight_maximum;

//Menus
extern Menu MainMenu;
extern Menu ViewMenu;
extern Menu EditMenu;
extern Menu WeightMenu;

void Load_Menu();

void RenderMenu();
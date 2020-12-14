#include "Menu.h"
#include "TSPAlgorithms.h" //Algorithm does not work in its current state

std::stringstream vertexcountstrs;
unsigned int FPS_frames = 0;
std::stringstream FPS_strs;

void Initialize();

// You must include the command line parameters for your main function to be recognized by SDL
int main(int argc, char** args) {
	Initialize();

	FPS_startt = SDL_GetTicks(), FPS_endt = SDL_GetTicks();
	FPS_strs.str("FPS:0");

	bool Running = true;
	while (Running) {
		SDL_SetRenderDrawColor(window->GetRenderer(), 255, 255, 255, 255);
		SDL_RenderClear(window->GetRenderer());

		//Handles Mouse Hovering
		hovered_vertex = -1;
		menu_hovered = false;
		menu_toggle_hovered = false;

		if (!MainMenu.active) {
			if (Input::IsMouseInRoundedRectangle(Menu_toggle_rect0, 2.3) || Input::IsMouseInRectangle(Menu_toggle_rect1)) {
				menu_hovered = true;
				menu_toggle_hovered = true;
			}
		}
		else {
			if (Input::IsMouseInRoundedRectangle(Menu_toggle_rect3, 2.3) || Input::IsMouseInRectangle(Menu_toggle_rect4)) //Toggle button
			{
				menu_hovered = true;
				menu_toggle_hovered = true;
			}
		}
		if (WeightMenu.IsMouseoverMenu() || MainMenu.IsMouseoverMenu()) {
			menu_hovered = true;
		}

		if (!menu_hovered) //Menu is in front of the graph itself
		{
			unsigned int N = main_graph.vertex_count();
			double dist2 = 0;
			Vertex<int> renderv;
			for (unsigned int i = 0; i < N; i++) {
				renderv = WorldtoView(main_graph.get_vertex(i));
				dist2 = pow(Input::GetMouseX() - renderv.x, 2) + pow(Input::GetMouseY() - renderv.y, 2);
				if (dist2 <= 9) {
					hovered_vertex = i;
					break;
				}
			}
		}

		//Toggle Menu Button Availabilty and Rendering
		if (MainMenu.active) {
			//Graph weight button
			if (WeightMenu.active)
				button_weight.SetSourceX(360);
			else if (!Editable)
				button_weight.SetSourceX(540);
			else
				button_weight.SetSourceX(180);
			//Clear graph button

			if (Editable)
				button_clear.SetSourceY(60);
			else
				button_clear.SetSourceY(90);

		}

		SDL_Event EV;
		while (SDL_PollEvent(&EV) != 0) {
			//Before Mouse/Keyboard updates
			switch (EV.type) {
			case SDL_QUIT:
				Running = false;
			}
			//Update
			switch (EV.type) {
			case SDL_MOUSEBUTTONUP:
				if (selecting) {
					if (SELECTION_MODE == 0) {
						selected_vertices.clear();
					}
					Vertex<int> renderv;
					for (unsigned int i = 0; i < main_graph.vertex_count(); i++) {
						renderv = WorldtoView(main_graph.get_vertex(i));
						if (Input::IsVertexInMouseRectangle(renderv)) //Vertex is in box
						{
							switch (SELECTION_MODE) {
							case 0:
								Toggle_Vertex(i);
								break;
							case 1:
								if (!IsVertexSelected(i)) {
									Toggle_Vertex(i);
								}
								break;
							case 2:
								if (IsVertexSelected(i)) {
									Toggle_Vertex(i);
								}
								break;
							case 3:
								break;
							}
						}
						else {
							if (SELECTION_MODE == 3) {
								if (IsVertexSelected(i)) {
									Toggle_Vertex(i);
								}
							}
						}
					}
					selecting = false;
				}
				break;
			}
			Input::Update(EV);
			window->Update();
			//After Mouse/Keyboard updates

			switch (EV.type) {
			case SDL_MOUSEMOTION:
				if (Input::IsMousePressed() && !menu_hovered2 && hovered_vertex2 == -1) {
					if (!Input::WasShiftPressedAtMousePress() && !Input::WasCtrlPressedAtMousePress()) //Drag screen
					{
						window_pos[0] -= double(EV.motion.xrel) / window_scale;
						window_pos[1] -= double(EV.motion.yrel) / window_scale;
					}
					else if (Editable && !Input::WasShiftPressedAtMousePress() && Input::WasCtrlPressedAtMousePress()) //Drag selected vertices
					{
						unsigned int N = selected_vertices.size();
						for (unsigned int i = 0; i < N; i++) {
							main_graph.move_vertex(selected_vertices[i], double(EV.motion.xrel) / window_scale, double(EV.motion.yrel) / window_scale);
						}
					}
				}
				break;
			case SDL_MOUSEWHEEL: //Zoom in and out
				window_scale *= exp(double(EV.wheel.y) / 3);
				break;
			case SDL_KEYDOWN:
				switch (EV.key.keysym.sym) {
				case SDLK_r: //Reset View
					if (EV.key.repeat == 0) {
						ResetView();
					}
					break;
					/*case SDLK_0:
						cout << '\n' << "---------" << '\n';
						TSPalg = 0;
						break;
					case SDLK_1:
						cout << '\n' << "---------" << '\n';
						TSPalg = 1;
						break;
						*/
				case SDLK_c: //Clear Graph
					ClearGraph();
					break;
				case SDLK_f: //Fill screen with 100 vertices
					if (Editable && ControlMode == CONTROL_CREATION) {
						for (unsigned int i = 0; i < 100; i++) {
							main_graph.add_vertex(ViewtoWorld({ rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT }));
						}
					}
					break;
				case SDLK_a: //Deselect vertices
					if (Editable && Input::IsCtrlPressed() && !Input::IsShiftPressed()) { //Ctrl+A
						selected_vertices.clear();
					}
					break;
				case SDLK_DELETE: //Delete vertices
					if (Editable) {
						uint64_t N = selected_vertices.size();
						for (int i = N - 1; i >= 0; i--) {
							main_graph.remove_vertex(selected_vertices[i]);
						}
						selected_vertices.clear();
					}
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (menu_hovered) {
					menu_hovered2 = true;
					if (menu_toggle_hovered) {
						if (!MainMenu.active) {
							MainMenu.active = true;
						}
						else {
							MainMenu.active = false;
							button_weight.SetSourceY(120);
						}
					}
					else
					{
						MainMenu.UpdateonMouseDown();
						WeightMenu.UpdateonMouseDown();
					}
				}
				else {
					if (Input::WasShiftPressedAtMousePress() && !Input::WasCtrlPressedAtMousePress() && hovered_vertex == -1) {
						Vertex<double> v = ViewtoWorld({ Input::GetMouseXAtMousePress(), Input::GetMouseYAtMousePress() });
						switch (ControlMode) {
						case CONTROL_CREATION: //Creates a new vertex
							main_graph.add_vertex(v);
							break;
						case CONTROL_SELECTION:
							selecting = true;
							break;
						}
					}
					else //No relevant buttons pressed
					{
						if (hovered_vertex >= 0) //Hovering over a vertex
						{
							switch (ControlMode) {
							case CONTROL_CREATION: //Creation mode (At most 2 vertices selected)
								if (IsVertexSelected(hovered_vertex)) {
									for (unsigned int i = 0; i < selected_vertices.size(); i++) {
										if (selected_vertices[i] == hovered_vertex) {
											selected_vertices.erase(selected_vertices.begin() + i);
										}
									}
								}
								else {
									if (selected_vertices.size() == 2) {
										selected_vertices.erase(selected_vertices.begin());
									}
									selected_vertices.push_back(hovered_vertex);
								}
								break;
							case CONTROL_SELECTION: //Selection mode
								Toggle_Vertex(hovered_vertex);
								break;
							}
						}
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				menu_hovered2 = false;
				MainMenu.UpdateonMouseUp();
				WeightMenu.UpdateonMouseUp();
				break;
			}
		}

		//Run TSP
		if (TSPrunning) {
			unsigned int prec = 2;
			switch (TSPalg2) {
			case 1:
				TSPU_2opt(main_graph);
				break;
			case 0:
				TSPU_SimulatedAnnealing(main_graph);
				break;
			default:
				Editable = true;
				TSPrunning = false;
			}
		}

		//Calculate FPS
		FPS_endt = SDL_GetTicks();
		if (FPS_endt - FPS_startt > 1000) {
			FPS_strs.str("");
			FPS_strs << "FPS:" << FPS_frames;
			FPS_frames = 0;
			FPS_startt = SDL_GetTicks();
		}

		//Rendering
		if (show_grid) {
			RenderGrid();
		}
		if (!Editable && TSPperm.size() != 0) {
			SDL_SetRenderDrawColor(window->GetRenderer(), COLOR_DEFAULT);
			SDL_SetTextureColorMod(font_texture, COLOR_DEFAULT);
			if (main_graph.vertex_count() == 2) {
				RenderEdge(TSPperm[0], TSPperm[1], true);
			}
			else if (main_graph.vertex_count() > 2) {
				bool renderedges = !Editable;
				for (unsigned int i = 0; i < main_graph.vertex_count(); i++) {
					RenderEdge(TSPperm[i], TSPperm[(i + 1) % main_graph.vertex_count()], renderedges);
				}
			}
		}
		RenderGraph();
		RenderMenu();
		if (!Editable && MainMenu.active) {
			SDL_SetRenderDrawColor(window->GetRenderer(), 213, 213, 213, 192);
			SDL_RenderFillRect(window->GetRenderer(), &Menu_main_rect5);
			Render_String(TSPstrs.str(), 187, 218, COLOR_DEFAULT);
		}
		if (selecting) {
			SDL_SetRenderDrawColor(window->GetRenderer(), 192, 192, 192, 255);
			SDL_RenderDrawRect(window->GetRenderer(), Input::GetMouseRectanglePointer());
		}
		vertexcountstrs.str("");
		vertexcountstrs << "Vertex count:" << main_graph.vertex_count();
		Render_String(FPS_strs.str(), 2, WINDOW_HEIGHT - 26, COLOR_DEFAULT);
		Render_String(vertexcountstrs.str(), 2, WINDOW_HEIGHT - 13, COLOR_DEFAULT);

		// Update the window display
		SDL_RenderPresent(window->GetRenderer());

		SDL_Delay(1);
		FPS_frames++;
	}

	// Wait

	IMG_Quit();
	SDL_Quit();
	return 0;
}

void Initialize() {
	// Initialize SDL. SDL_Init will return -1 if it fails.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		abort();
	}

	window = std::make_unique<Window>("Travelling Salesman Solver 1.0", 960, 720);

	if (!window->GetWindow()) {
		std::cout << "Error creating window: " << SDL_GetError() << std::endl;
		system("pause");
		abort();
	}
	
	if (!IMG_Init(IMG_INIT_PNG)) {
		std::cout << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
		system("pause");
		abort();
	}

	vertex_texture = std::make_shared<Texture>("src/img/vertex.png");
	font_texture = std::make_shared<Texture>("src/img/font_texture.png");

	Load_Menu();

	std::string controls =
		R"(---Controls---
> Drag Screen - LMouse Drag
> Zoom in and out - Mouse Wheel
> Move selected Vertices - Ctrl+LMouse Drag
> Select/Deselect Vertex - LMouse
> Delete selected Vertices - Delete
> Deselect all Vertices - Ctrl+A
> Clear Graph - C
> Reset View - R

--Creation Mode--
> Create Vertex - Shift+LMouse
> Create 100 random vertices - F

--Selection Mode--
> Select Vertices - Shift+LMouse Drag)";
	std::cout << controls;
}
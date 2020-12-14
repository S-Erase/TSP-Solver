#include "Selection.h"
#include "Menu.h"

std::vector<int> selected_vertices = {}; //Kept in ascending order.
std::vector<int> shown_edge = {}; //Kept in ascending order.
int hovered_vertex = -1;
int hovered_vertex2 = -1;
bool selecting = false; //Used to create selection boxes
int SELECTION_MODE = 0; //0:New, 1:Add, 2:Subtract, 3:Intersection

bool IsVertexSelected(unsigned int v0) {
	unsigned int N = selected_vertices.size();
	if (N > 0) {
		switch (ControlMode) {
		case CONTROL_CREATION: //Creation Mode
			for (unsigned int i = 0; i < N; i++) {
				if (selected_vertices[i] == v0) {
					return true;
				}
			}
			break;
		case CONTROL_SELECTION: //Selection Mode
			int v_start = 0, v_avg, v_end = N - 1;
			while (v_end >= v_start) {
				v_avg = floor((v_start + v_end) / 2);
				if (selected_vertices[v_avg] == int(v0)) {
					return true;
				}
				else if (selected_vertices[v_avg] > int(v0)) {
					v_end = v_avg - 1;
				}
				else if (selected_vertices[v_avg] < int(v0)) {
					v_start = v_avg + 1;
				}

			}
			break;
		}
	}
	return false;
}

void Toggle_Vertex(unsigned int v0) {
	unsigned int N = selected_vertices.size();
	if (N > 0) {
		int v_start = 0, v_avg = (N - 1) / 2, v_end = N - 1;
		while (v_end >= v_start) {
			if (selected_vertices[v_avg] == int(v0)) { //Deselect
				selected_vertices.erase(selected_vertices.begin() + v_avg);
				return;
			}
			else if (selected_vertices[v_avg] > int(v0)) {
				v_end = v_avg - 1;
				v_avg = (v_start + v_end) / 2;
			}
			else if (selected_vertices[v_avg] < int(v0)) {
				v_start = v_avg + 1;
				v_avg = (v_start + v_end) / 2;
			}
		}
		selected_vertices.insert(selected_vertices.begin() + v_start, v0);
	}
	else {
		selected_vertices.insert(selected_vertices.begin(), v0);
	}

}
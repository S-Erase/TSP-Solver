#pragma once
#include <vector>

extern std::vector<int> selected_vertices; //Kept in ascending order.
extern std::vector<int> shown_edges; //Kept in ascending order.
extern int hovered_vertex;
extern int hovered_vertex2;
extern bool selecting; //Used to create selection boxes
extern int SELECTION_MODE; //0:New, 1:Add, 2:Subtract, 3:Intersection

bool IsVertexSelected(unsigned int v0);

void Toggle_Vertex(unsigned int v0);
#pragma once

#include <vector>
#include <iostream>
#include <string>
#include "Vertex.h"

enum class GraphNorm {
	Euclidean = 0, Taxicab, Maximum, L_p, Custom
};

class WGraph { //Weighted Graph
private:
	std::vector< Vertex<double> > vertices = {};
	std::vector<double> weights = {}; //(v0,v1) is stored in the v0*(v0-1)/2 + v1 th place (v0 > v1)
	size_t vertexcount = 0;
	GraphNorm norm = GraphNorm::Euclidean; //Options: Euclidean, L-p, Taxicab, Maximum, Custom
	double Lp = 1;
public:
	void add_vertex(Vertex<double> v);
	void clear_graph();
	void set_weight(uint64_t v0, uint64_t v1, double weight);
	double get_weight(uint64_t, uint64_t) const;
	void show_vertices() const;
	inline unsigned int vertex_count()  const { return vertexcount; };
	inline Vertex<double> get_vertex(uint64_t vindex)  const{ return vertices[vindex]; }
	void remove_vertex(uint64_t);
	void move_vertex(uint64_t, double, double);
	void set_norm(std::string);
	void set_norm(GraphNorm);
	inline GraphNorm get_norm()  const{ return norm; };
};

extern WGraph main_graph;


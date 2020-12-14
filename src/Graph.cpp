#include "Graph.h"

WGraph main_graph;

void WGraph::add_vertex(Vertex<double> v) {
	vertices.push_back(v);
	if (norm == GraphNorm::Custom) {
		for (unsigned int i = 0; i < vertexcount; i++) {
			weights.push_back(0);
		}
	}
	vertexcount++;
}

void WGraph::clear_graph() {
	vertices.clear();
	weights.clear();
	vertexcount = 0;
}

void WGraph::set_weight(uint64_t v0, uint64_t v1, double weight) {
	if (v0 >= vertexcount || v1 >= vertexcount) {
		std::cout << "set_weight error: Vertex index out of bounds.\n";
	}
	else if (v0 == v1) {
		std::cout << "set_weight error: Vertices cannot be identical.\n";
	}
	else if (norm != GraphNorm::Custom) {
		std::cout << "set_weight error: Norm must be Custom.\n";
	}
	else {
		unsigned int w0 = std::max(v0, v1);
		unsigned int w1 = std::min(v0, v1);
		weights[(w0 * (w0 - 1)) / 2 + w1] = weight;
	}
}

double WGraph::get_weight(uint64_t v0, uint64_t v1) const {
	if (v0 >= vertexcount || v1 >= vertexcount) {
		std::cout << "get_weight error: Vertex index out of bounds.\n";
		return 0;
	}
	else if (v0 == v1) {
		std::cout << "get_weight error: Vertex cannot be identical.\n";
		return 0;
	}
	else {
		double dx = abs(vertices[v0].x - vertices[v1].x);
		double dy = abs(vertices[v0].y - vertices[v1].y);
		switch (norm) {
		case GraphNorm::Euclidean:	return sqrt(dx * dx + dy * dy);
		case GraphNorm::Maximum:	return std::max(dx, dy);
		case GraphNorm::Taxicab:	return (dx + dy);
		case GraphNorm::L_p:		return pow(pow(dx, Lp) + pow(dy, Lp), 1 / Lp);
		case GraphNorm::Custom:
			dx = std::max(v0, v1);
			dy = std::min(v0, v1);
			return weights[(dx * (dx - 1)) / 2 + dy];
			break;
		}
	}
}

void WGraph::show_vertices() const {
	std::string list = "Vertices: {";
	for (unsigned int i = 0; i < vertexcount; i++) {
		list = list + " (" + std::to_string(vertices[i].x) + "," + std::to_string(vertices[i].y) + "),";
	}
	list.pop_back();
	list += " }\n";
	std::cout << list;
}

void WGraph::remove_vertex(uint64_t v0) {
	if (v0 >= vertexcount) {
		std::cout << "remove_vertex error: Vertex index out of bounds.\n";
	}
	else {
		vertices.erase(vertices.begin() + v0);
		vertexcount--;
		if (norm == GraphNorm::Custom) {
			for (unsigned int i = vertexcount; i > v0; i--) {
				weights.erase(weights.begin() + i * (i - 1) / 2 + v0);
			}
			for (int i = v0 - 1; i >= 0; i--) {
				weights.erase(weights.begin() + v0 * (v0 - 1) / 2 + i);
			}
		}
	}
}

void WGraph::move_vertex(uint64_t v0, double dx, double dy) {
	vertices[v0].x = vertices[v0].x + dx;
	vertices[v0].y = vertices[v0].y + dy;
}

void WGraph::set_norm(std::string newnorm) {
	if (newnorm == "Custom") //Custom
	{ 
		if (norm != GraphNorm::Custom)
		{
			if (vertexcount > 0) {
				for (unsigned int n = 0; n < vertexcount; n++) {
					if (n > 0) {
						for (unsigned int m = 0; m < n; m++) {
							weights.push_back(get_weight(n, m));
						}
					}
				}
				norm = GraphNorm::Custom;
			}
		}
	}
	else if (newnorm.substr(0, 2) == "L-") { //L-p norms
		double p;
		try {
			p = stod(newnorm.substr(2));
		}
		catch (...) {
			std::cout << "set_norm error: Invalid entry";
			return;
		}
		if (p < 1) {
			std::cout << "set_norm error: Invalid p parameter";
		}
		else {
			if (norm == GraphNorm::Custom) {
				weights.clear();
			}

			if (p == 1) {
				norm = GraphNorm::Taxicab;
			}
			else if (p == 2) {
				norm = GraphNorm::Euclidean;
			}
			else {
				norm = GraphNorm::L_p;
				Lp = p;
			}
		}
	}
	else if (newnorm == "Taxicab" || newnorm == "Euclidean" || newnorm == "Maximum") {
		if (norm == GraphNorm::Custom)
			weights.clear();

		if (newnorm == "Euclidean")
			norm = GraphNorm::Euclidean;
		else if (newnorm == "Taxicab")
			norm = GraphNorm::Taxicab;
		else if (newnorm == "Maximum")
			norm = GraphNorm::Maximum;
	}
	else {
		std::cout << "set_norm error: Invalid entry";
	}
}

void WGraph::set_norm(GraphNorm newnorm)
{
	if (newnorm == GraphNorm::Custom) //Custom
	{
		if (norm != GraphNorm::Custom)
		{
			if (vertexcount > 0) {
				for (unsigned int n = 0; n < vertexcount; n++) {
					if (n > 0) {
						for (unsigned int m = 0; m < n; m++) {
							weights.push_back(get_weight(n, m));
						}
					}
				}
				norm = GraphNorm::Custom;
			}
		}
	}
	else if (newnorm == GraphNorm::L_p) { //L-p norms
		if (norm == GraphNorm::Custom) {
			weights.clear();
		}

		if (Lp == 1) {
			norm = GraphNorm::Taxicab;
		}
		else if (Lp == 2) {
			norm = GraphNorm::Euclidean;
		}
		else {
			norm = GraphNorm::L_p;
		}
	}
	else {
		if (norm == GraphNorm::Custom)
			weights.clear();

		norm = newnorm;
	}
}

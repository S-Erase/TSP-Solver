#pragma once
#include <sstream>
#include <iomanip>
#include "Graph.h"
#include "Menu.h"

//Variables for computing the time taken to perform the algorithm
extern Uint32 TSP_startt, TSP_endt;
//Variables for solving the TSP
extern std::vector<uint64_t> TSPperm;
extern std::vector<uint64_t> TSPoldperm;
extern double TSPvalue;
extern double TSPnewvalue;
extern bool TSPrunning;
extern std::stringstream TSPstrs;
extern unsigned int TSPboxlen;
extern unsigned char TSPalg;
extern unsigned char TSPalg2;
extern unsigned int TSPiter0;
extern unsigned int TSPiter1;

//Support
void Twoopt(std::vector<uint64_t>& path, uint64_t i0, uint64_t i1);

/*vector<unsigned int> TSP_2optrefine(WGraph& gra, vector<unsigned int>& path) {
	unsigned int N0 = gra.vertex_count(), N1 = path.size();
	if (N0 != N1) {
		cout << "2-opt error: path has incorrect length.";
	}
	else if (N0 < 4) //No updates needed
	{
		return path;
	}
	else {
		vector<unsigned int> newpath = path;
		bool intersect = false;
		double sum0, sum1;

		for (unsigned int m = 2; m < N0 - 1; m++) {
			sum0 = gra.get_weight(newpath[0], newpath[1]) + gra.get_weight(newpath[m], newpath[(m + 1) % N0]);
			sum1 = gra.get_weight(newpath[0], newpath[m]) + gra.get_weight(newpath[1], newpath[(m + 1) % N0]);
			if (sum1 < sum0) {
				newpath = Twoopt(newpath, 0, m);
			}
		}

		for (unsigned int n = 1; n < N0 - 2; n++) {
			for (unsigned int m = n + 2; m < N0; m++) {
				sum0 = gra.get_weight(newpath[n], newpath[n + 1]) + gra.get_weight(newpath[m], newpath[(m + 1) % N0]);
				sum1 = gra.get_weight(newpath[n], newpath[m]) + gra.get_weight(newpath[n + 1], newpath[(m + 1) % N0]);
				if (sum1 < sum0) {
					newpath = Twoopt(newpath, n, m);
				}
			}
		}
		if (path == newpath) {
			return newpath;
		}
		else {
			return TSP_2optrefine(gra, newpath);
		}
	}
}*/

std::vector<uint64_t> idperm(size_t N);

double Random_UnitUniform();

//Algorithms (In one go)
/*vector<unsigned int> TSP_SimulatedAnnealing(WGraph& gra) 
{
	const unsigned int N = gra.vertex_count();
	vector<unsigned int> vertexset = idperm(N);
	if (N > 3) {
		const vector<unsigned int> ID_PERM = vertexset;

		vector<unsigned int> currentpath = {}; //Creates random initial permutation
		unsigned int dummy_int;
		for (unsigned int i = 0; i < N; i++) {
			dummy_int = rand() % (N - i);
			currentpath.push_back(vertexset[dummy_int]);
			vertexset.erase(vertexset.begin() + dummy_int);
		}
		vertexset = ID_PERM;
		vector<unsigned int> trialpath = currentpath; //
		unsigned int optind[2]; //indexes

		double currentval = 0;
		for (unsigned int i = 0; i < N; i++) {
			currentval += gra.get_weight(currentpath[i], currentpath[(i + 1) % N]);
		}
		cout << "Initial length: " << currentval << "\n";
		double trialval = currentval;
		
		for (unsigned int step = 0; step < (50 * N); step++) {
			//2-opt swap
			optind[0] = rand() % N;
			optind[1] = (rand() % (N - 3) + N + 2) % N;
			trialpath = Twoopt(currentpath, optind[0], optind[1]);
			//compute trial value
			trialval = 0;
			for (unsigned int i = 0; i < N; i++) {
				trialval += gra.get_weight(trialpath[i], trialpath[(i + 1) % N]);
			}
			//Test
			if (trialval <= currentval) {
				currentval = trialval;
				currentpath = trialpath;
			}
			else {
				double limit = exp((currentval - trialval) / temp);
				if (Random_Uniform(0,1) <= limit) { //Accepted
					currentval = trialval;
					currentpath = trialpath;
				}
				else {
					trialval = currentval;
					trialpath = currentpath;
				}
			}
			vertexset = ID_PERM;
			temp *= alpha;
		}
		return currentpath;
	}
	else {
		return vertexset;
	}
} */

/*vector<unsigned int> TSP_2opt(WGraph& gra) 
{
	const unsigned int N = gra.vertex_count();
	vector<unsigned int> vertexset = idperm(N);
	if (N > 3) {
		const vector<unsigned int> ID_PERM = vertexset;

		vector<unsigned int> perm = {}; //Creates random initial permutation
		unsigned int dummy_int;
		for (unsigned int i = 0; i < N; i++) {
			dummy_int = rand() % (N - i);
			perm.push_back(vertexset[dummy_int]);
			vertexset.erase(vertexset.begin() + dummy_int);
		}
		double initval = 0;
		for (unsigned int n = 0; n < N; n++) {
			initval += gra.get_weight(perm[n], perm[(n + 1) % N]);
		}
		cout << "Initial length: " << initval << "\n";
		return TSP_2optrefine(gra, perm);
	}
	else {
		return vertexset;
	}
}
*/

//Algorithms (In multiple frames)

//Variables for solving the TSP via Simulated annealing
extern double tempmax; //temperature
extern double temptarget;
extern double temp; //temperature
extern double alpha; //cooling rate

//Vairables for solving the TSP via 2-opt
extern unsigned int twoopt_i0;
extern unsigned int twoopt_i1;

void TSP_MultiThread(WGraph& gra);

void TSPU_SimulatedAnnealing(WGraph& gra);

void TSPU_2opt(WGraph& gra);
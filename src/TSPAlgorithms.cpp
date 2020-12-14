#include "TSPAlgorithms.h"

//Variables for computing the time taken to perform the algorithm
Uint32 TSP_startt, TSP_endt;
//Variables for solving the TSP
std::vector<uint64_t> TSPperm = {};
std::vector<uint64_t> TSPoldperm = {};
double TSPvalue = 0;
double TSPnewvalue = 0;
bool TSPrunning = false;
std::stringstream TSPstrs;
unsigned int TSPboxlen = 0;
unsigned char TSPalg = 0;
unsigned char TSPalg2 = 0;
unsigned int TSPiter0 = 0;
unsigned int TSPiter1 = 0;

//Support
void Twoopt(std::vector<uint64_t>& path, uint64_t i0, uint64_t i1) {
	unsigned int u0 = std::min(i0, i1), u1 = std::max(i0, i1);
	std::reverse(std::begin(path)+u0+1,std::begin(path)+u1+1); //Much faster
}

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

std::vector<uint64_t> idperm(size_t N) {
	std::vector<uint64_t> perm(N);
	for (uint64_t n = 0; n < N; n++) {
		perm[n] = n;
	}
	return perm;
}

double Random_UnitUniform() {
	double res = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX));
	return res;
}

//Algorithms (In multiple frames)

//Variables for solving the TSP via Simulated annealing
double tempmax = 500; //temperature
double temptarget = 1;
double temp = 500; //temperature
double alpha = 0.99; //cooling rate

//Vairables for solving the TSP via 2-opt
unsigned int twoopt_i0;
unsigned int twoopt_i1;

void TSP_MultiThread(WGraph& gra)
{
	const size_t N = gra.vertex_count();
	std::vector<uint64_t> vertexset = idperm(N);
	double TSPvalue;
	if (N <= 3) {
		Editable = false;
		TSPvalue = 0;
		switch (N) {
		case 2:
			TSPvalue = gra.get_weight(vertexset[0], vertexset[1]);
			break;
		case 3:
			TSPvalue += gra.get_weight(vertexset[0], vertexset[1]);
			TSPvalue += gra.get_weight(vertexset[1], vertexset[2]);
			TSPvalue += gra.get_weight(vertexset[2], vertexset[0]);
			break;
		}
		TSP_endt = SDL_GetTicks();
		std::cout << "Refined length: " << TSPvalue << ". Caluclated in " << TSP_endt - TSP_startt << "ms" << "\n";

		unsigned int prec = 2;
		if (abs(TSPvalue) < 0.1) {
			prec = 1 - floor(log10(abs(TSPvalue)));
		}
		TSPstrs.str(std::string());
		TSPstrs << "Calculated total weight: " << std::fixed << std::setprecision(prec) << TSPvalue;
		Menu_main_rect5.w = 6 * TSPstrs.str().size() + 14;
		TSPperm = vertexset;
		TSPrunning = false;
	}
	else {
		const std::vector<uint64_t> ID_PERM = vertexset;
		
		uint64_t dummy_int;
		TSPperm.clear();
		TSPperm.reserve(N);
		for (uint64_t i = 0; i < N; i++) {
			dummy_int = rand() % (N - i);
			TSPperm.emplace_back(vertexset[dummy_int]);
			vertexset.erase(vertexset.begin() + dummy_int);
		}
		vertexset = ID_PERM;

		TSPvalue = 0;
		for (uint64_t i = 0; i < N; i++) {
			TSPvalue += gra.get_weight(TSPperm[i], TSPperm[(i + 1) % N]);
		}
		double tempmax = 10 * TSPvalue / N; //Scales temperature to be proportional to the average length of the graph's edges
		double temp = tempmax;
		double temptarget = TSPvalue / N / 10;
		double alpha = exp(-1 / double(250 * N));

		double sum0, sum1;
		double progress;
		while (temp >= temptarget) {//2-opt swap
			twoopt_i0 = rand() % N;
			twoopt_i1 = (rand() % (N - 3) + twoopt_i0 + 2) % N;

			//compute trial value
			sum0 = gra.get_weight(TSPperm[twoopt_i0], TSPperm[(twoopt_i0 + 1) % N]) + gra.get_weight(TSPperm[twoopt_i1], TSPperm[(twoopt_i1 + 1) % N]);
			sum1 = gra.get_weight(TSPperm[twoopt_i0], TSPperm[twoopt_i1]) + gra.get_weight(TSPperm[(twoopt_i0 + 1) % N], TSPperm[(twoopt_i1 + 1) % N]);

			//Test
			if (sum1 <= sum0) {
				Twoopt(TSPperm, twoopt_i0, twoopt_i1);
			}
			else {
				double limit = exp((sum0 - sum1) / temp);
				if (Random_UnitUniform() <= limit) { //Accepted
					Twoopt(TSPperm, twoopt_i0, twoopt_i1);
				}
			}
			temp *= alpha;
			temptarget = TSPvalue / N / 10;
			
			progress = 100 * (log(tempmax) - log(temp)) / (log(tempmax) - log(temptarget));
			TSPstrs.str(std::string());
			TSPstrs << "Progress: " << std::fixed << std::setprecision(2) << progress << "%";
			Menu_main_rect5.w = 6 * TSPstrs.str().size() + 14;
		}

		TSPstrs.str("Two-opt Optimization in progress...");

		uint64_t twoopt_i0 = 0, twoopt_i1 = 2;
		std::vector<uint64_t> TSPoldperm = TSPperm;
		while (true) {
			sum0 = gra.get_weight(TSPperm[twoopt_i0], TSPperm[twoopt_i0 + 1]) + gra.get_weight(TSPperm[twoopt_i1], TSPperm[(twoopt_i1 + 1) % N]);
			sum1 = gra.get_weight(TSPperm[twoopt_i0], TSPperm[twoopt_i1]) + gra.get_weight(TSPperm[twoopt_i0 + 1], TSPperm[(twoopt_i1 + 1) % N]);
			if (sum1 < sum0) {
				Twoopt(TSPperm, twoopt_i0, twoopt_i1);
			}
			//Update indices
			twoopt_i1++;
			if (twoopt_i0 == 0 && twoopt_i1 == N - 1) {
				twoopt_i0 = 1, twoopt_i1 = 3;
			}
			else if (twoopt_i1 == N) {
				twoopt_i0++;
				twoopt_i1 = twoopt_i0 + 2;
			}
			// End
			if (twoopt_i0 == N - 2) {
				if (TSPperm == TSPoldperm) {
					break;
				}
				else {
					twoopt_i0 = 0, twoopt_i1 = 2;
					TSPoldperm = TSPperm;
				}
			}
		}

		Editable = false;
		button_controls.SetSourceY(30);
		TSPvalue = 0;
		for (uint64_t i = 0; i < N; i++) {
			TSPvalue += gra.get_weight(TSPperm[i], TSPperm[(i + 1) % N]);
		}
		TSP_endt = SDL_GetTicks();
		std::cout << "Refined length: " << TSPvalue << ". Caluclated in " << TSP_endt - TSP_startt << "ms" << std::endl;

		unsigned int prec = 2;
		if (abs(TSPvalue) < 0.1) {
			prec = 1 - floor(log10(abs(TSPvalue)));
		}
		TSPstrs.str(std::string());
		TSPstrs << "Calculated total weight: " << std::fixed << std::setprecision(prec) << TSPvalue;
		Menu_main_rect5.w = 6 * TSPstrs.str().size() + 14;
	}
}

void TSPU_SimulatedAnnealing(WGraph& gra)
{
	const size_t N = gra.vertex_count();
	std::vector<uint64_t> vertexset = idperm(N);
	TSPstrs.str("");
	if (N <= 3) {
		TSPrunning = false;
		Editable = false;
		TSPvalue = 0;
		switch (N) {
		case 2:
			TSPvalue = gra.get_weight(vertexset[0], vertexset[1]);
			break;
		case 3:
			TSPvalue += gra.get_weight(vertexset[0], vertexset[1]);
			TSPvalue += gra.get_weight(vertexset[1], vertexset[2]);
			TSPvalue += gra.get_weight(vertexset[2], vertexset[0]);
			break;
		}
		TSP_endt = SDL_GetTicks();
		std::cout << "Refined length: " << TSPvalue << ". Caluclated in " << TSP_endt - TSP_startt << "ms" << "\n";

		unsigned int prec = 2;
		if (abs(TSPvalue) < 0.1) {
			prec = 1 - floor(log10(abs(TSPvalue)));
		}
		TSPstrs << "Calculated total weight: " << std::fixed << std::setprecision(prec) << TSPvalue;
		Menu_main_rect5.w = 6 * TSPstrs.str().size() + 14;
		TSPperm = vertexset;
	}
	else {
		const std::vector<uint64_t> ID_PERM = vertexset;
		if (TSPperm.size() == 0) //Creates random initial permutation
		{
			uint64_t dummy_int;
			for (uint64_t i = 0; i < N; i++) {
				dummy_int = rand() % (N - i);
				TSPperm.push_back(vertexset[dummy_int]);
				vertexset.erase(vertexset.begin() + dummy_int);
			}
			vertexset = ID_PERM;

			TSPvalue = 0;
			for (unsigned int i = 0; i < N; i++) {
				TSPvalue += gra.get_weight(TSPperm[i], TSPperm[(i + 1) % N]);
			}
			tempmax = 10 * TSPvalue / N; //Scales temperature to be proportional to the average length of the graph's edges
			temp = tempmax;
			alpha = exp(-1 / double(250 * N));
			TSPiter0 = 0;
		}

		temptarget = TSPvalue / N / 10;

		double sum0, sum1;

		Uint32 startt = SDL_GetTicks();
		Uint32 currt = SDL_GetTicks();
		while (currt - startt < 16) //Loops until the next frame occurs
		{
			//2-opt swap
			twoopt_i0 = rand() % N;
			twoopt_i1 = (rand() % (N - 3) + twoopt_i0 + 2) % N;

			//compute trial value
			sum0 = gra.get_weight(TSPperm[twoopt_i0], TSPperm[(twoopt_i0 + 1) % N]) + gra.get_weight(TSPperm[twoopt_i1], TSPperm[(twoopt_i1 + 1) % N]);
			sum1 = gra.get_weight(TSPperm[twoopt_i0], TSPperm[twoopt_i1]) + gra.get_weight(TSPperm[(twoopt_i0 + 1) % N], TSPperm[(twoopt_i1 + 1) % N]);

			//Test
			if (sum1 <= sum0) {
				Twoopt(TSPperm, twoopt_i0, twoopt_i1);
				//	TSPiter1++;
			}
			else {
				double limit = exp((sum0 - sum1) / temp);
				if (Random_UnitUniform() <= limit) { //Accepted
					Twoopt(TSPperm, twoopt_i0, twoopt_i1);
					//		TSPiter1++;
				}
			}
			temp *= alpha;
			currt = SDL_GetTicks();
			//TSPiter0++;

			//if (TSPiter0 == 10000) {
			//	cout << "Acceptance rate: " << fixed << setprecision(2) << 100 * TSPiter1 / double(TSPiter0) << "%\n";
			//	TSPiter0 = 0, TSPiter1 = 0;
			//}
		}

		if (temp < temptarget) {
			TSPalg2 = 1;
			TSPoldperm = TSPperm;
			twoopt_i0 = 0, twoopt_i1 = 2;
			TSPstrs << "Progress: 99.995%";
		}
		else {
			double progress = 100 * (log(tempmax) - log(temp)) / (log(tempmax) - log(temptarget));
			TSPstrs << "Progress: " << std::fixed << std::setprecision(2) << progress << "%";
		}
		Menu_main_rect5.w = 6 * TSPstrs.str().size() + 14;
	}
}

void TSPU_2opt(WGraph& gra)
{
	const uint64_t N = gra.vertex_count();
	std::vector<uint64_t> vertexset = idperm(N);
	TSPstrs.str("");
	if (N <= 3) {
		TSPrunning = false;
		Editable = false;
		TSPvalue = 0;
		switch (N) {
		case 2:
			TSPvalue = gra.get_weight(vertexset[0], vertexset[1]);
			break;
		case 3:
			TSPvalue += gra.get_weight(vertexset[0], vertexset[1]);
			TSPvalue += gra.get_weight(vertexset[1], vertexset[2]);
			TSPvalue += gra.get_weight(vertexset[2], vertexset[0]);
			break;
		}
		TSP_endt = SDL_GetTicks();
		std::cout << "Refined length: " << TSPvalue << ". Caluclated in " << TSP_endt-TSP_startt << "ms" << std::endl;

		unsigned int prec = 2;
		if (abs(TSPvalue) < 0.1) {
			prec = 1 - floor(log10(abs(TSPvalue)));
		}
		TSPstrs << "Calculated total weight: " << std::fixed << std::setprecision(prec) << TSPvalue;
		Menu_main_rect5.w = 6 * TSPstrs.str().size() + 14;
		TSPperm = vertexset;
	}
	else {
		const std::vector<uint64_t> ID_PERM = vertexset;

		if (TSPoldperm.size() == 0) {
			uint64_t dummy_int;
			for (uint64_t i = 0; i < N; i++) {
				dummy_int = rand() % (N - i);
				TSPperm.push_back(vertexset[dummy_int]);
				TSPoldperm.push_back(vertexset[dummy_int]);
				vertexset.erase(vertexset.begin() + dummy_int);
			}
			double initval = 0;
			for (uint64_t n = 0; n < N; n++) {
				initval += gra.get_weight(TSPperm[n], TSPperm[(n + 1) % N]);
			}
			twoopt_i0 = 0, twoopt_i1 = 2;
		}

		Uint32 startt = SDL_GetTicks(), endt = SDL_GetTicks();
		double sum0, sum1;
		while (endt - startt < 16) {
			sum0 = gra.get_weight(TSPperm[twoopt_i0], TSPperm[twoopt_i0 + 1]) + gra.get_weight(TSPperm[twoopt_i1], TSPperm[(twoopt_i1 + 1) % N]);
			sum1 = gra.get_weight(TSPperm[twoopt_i0], TSPperm[twoopt_i1]) + gra.get_weight(TSPperm[twoopt_i0 + 1], TSPperm[(twoopt_i1 + 1) % N]);
			if (sum1 < sum0) {
				Twoopt(TSPperm, twoopt_i0, twoopt_i1);
			}
			//Update indices
			twoopt_i1++;
			if (twoopt_i0 == 0 && twoopt_i1 == N - 1) {
				twoopt_i0 = 1, twoopt_i1 = 3;
			}
			else if (twoopt_i1 == N) {
				twoopt_i0++;
				twoopt_i1 = twoopt_i0 + 2;
			}
			//End
			if (twoopt_i0 == N - 2) {
				if (TSPperm == TSPoldperm) {
					TSPrunning = false;
					break;
				}
				else {
					twoopt_i0 = 0, twoopt_i1 = 2;
					TSPoldperm = TSPperm;
				}
			}
			endt = SDL_GetTicks();
		}

		if (TSPrunning) {
			TSPstrs << "Computation in progress...";
		}
		else {
			TSPvalue = 0;
			for (unsigned int i = 0; i < N; i++) {
				TSPvalue += gra.get_weight(TSPperm[i], TSPperm[(i + 1) % N]);
			}
			TSP_endt = SDL_GetTicks();
			std::cout << "Refined length: " << TSPvalue << ". Caluclated in " << TSP_endt - TSP_startt << "ms" << std::endl;

			unsigned int prec = 2;
			if (abs(TSPvalue) < 0.1) {
				prec = 1 - floor(log10(abs(TSPvalue)));
			}
			TSPstrs << "Calculated total weight: " << std::fixed << std::setprecision(prec) << TSPvalue;
		}
		Menu_main_rect5.w = 6 * TSPstrs.str().size() + 14;
	}

}
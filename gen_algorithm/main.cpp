// C++ implementation of the above approach
#include <bits/stdc++.h>
#include <limits.h>
#include <utility>
#include <fstream>
#include <vector>
using namespace std;

class Source {
public:
    std::string filename;
    std::vector<int> vec;

public:
    Source(std::string filename_) {filename = filename_;}
    ~Source() {vec.clear();}

    void Read() {
        std::ifstream input;
        input.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        try {
            input.open(filename);
            int teg;
            while(true) {
                input >> teg;
                vec.push_back(teg);
            }

        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
};

// Number of cities in TSP
#define V 10

// Names of the cities
#define GENES ABCDE

// Starting Node Value
#define START 0

// Initial population size for the algorithm
#define POP_SIZE 20

// Structure of a GNOME
// string defines the path traversed
// by the salesman while the fitness value
// of the path is stored in an integer

int RatingVector[V] ;
int TimeLimit = 100;
int gen_thres = 40;

struct individual {
	string gnome;
	int fitness;
    int rating;
};

// Function to return a random number
// from start and end
int rand_num(int start, int end)
{
	int r = end - start;
	int rnum = start + rand() % r;
	return rnum;
}

// Function to check if the character
// has already occurred in the string
bool repeat(string s, char ch)
{
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ch)
			return true;
	}
	return false;
}

// Function to return a mutated GNOME
// Mutated GNOME is a string
// with a random interchange
// of two genes to create variation in species
string mutatedGene(string gnome)
{
    if(gnome.size() >= 4) {
        while (true) {
            int r = rand_num(1, gnome.size() - 1);
            int r1 = rand_num(1, gnome.size() - 1);
            if (r1 != r) {
                char temp = gnome[r];
                gnome[r] = gnome[r1];
                gnome[r1] = temp;
                break;
            }
        }
    }
	return gnome;
}

// Function to return a valid GNOME string
// required to create the population
string create_gnome()
{
	string gnome = "0";
    int gnome_future_size = rand_num(2, V+1);
    int gnome_alternative_size = 4;
	while (true) {
		if (gnome.size() == gnome_future_size) {
			gnome += gnome[0];
			break;
		}
		int temp = rand_num(1, V);
		if (!repeat(gnome, (char)(temp + 48)))
			gnome += (char)(temp + 48);
	}
	return gnome;
}

// Function to return the fitness value of a gnome.
// The fitness value is the path length
// of the path represented by the GNOME.
int cal_fitness(string gnome, int map[V][V])
{

//    int map[V][V] = { { 0, 2, INT_MAX, 12, 5 },
//                      { 2, 0, 4, 8, INT_MAX },
//                      { INT_MAX, 4, 0, 3, 3 },
//                      { 12, 8, 3, 0, 10 },
//                      { 5, INT_MAX, 3, 10, 0 } };
	int f = 0;
	for (int i = 0; i < gnome.size() - 1; i++) {
		if (map[gnome[i] - 48][gnome[i + 1] - 48] == INT_MAX)
			return INT_MAX;
		f += map[gnome[i] - 48][gnome[i + 1] - 48];
	}
	return f;
}

int fitness_rating (string gnome) {
    int f = 0;
    for (int i = 0; i < gnome.size() - 1; i++) {
        int c = gnome[i] - 48;
        f += RatingVector[gnome[i] - 48];
    }

    return f;
};


pair<string, string> selection (string& g1, string& g2) {
    int rand = rand_num(1, (int)min(g1.size(), g2.size()));
    string buf1 = "";
    string buf2 = "";
    //struct individual i1;
    for(int i = 0; i < rand; ++i) {
        buf1 += g1[i];
    }
    for(int i = rand; i < g2.size(); ++i) {
        if(!repeat(buf1, g2[i])) {
            buf1 += g2[i];
        }

    }
    buf1 += buf1[0];
    for(int i = 0; i < rand; ++i) {
        buf2 += g2[i];
    }
    for(int i = rand; i < g1.size(); ++i) {
        if(!repeat(buf2, g1[i])) {
            buf2 += g1[i];
        }
    }

    buf2 += buf2[0];
    return {buf1, buf2};
}


// Function to return the updated value
// of the cooling element.
int cooldown(int temp)
{
	return (90 * temp) / 100;
}

// Comparator for GNOME struct.
bool lessthan(struct individual t1,
			struct individual t2)
{
	return t1.fitness < t2.fitness;
}

bool morethen(struct individual t1,
              struct individual t2) {
    return  t1.rating > t2.rating;
}

// Utility function for TSP problem.
void TSPUtil(int map[V][V])
{
    string best_result;
    int best_fitness;

    int maximum_rating = 0;
	// Generation Number
	int gen = 1;
	// Number of Gene Iterations

	vector<struct individual> population;
	struct individual temp;

	// Populating the GNOME pool.
	for (int i = 0; i < POP_SIZE; i++) {
		temp.gnome = create_gnome();
		temp.fitness = cal_fitness(temp.gnome, map);
        temp.rating = fitness_rating(temp.gnome);
		population.push_back(temp);
        if (maximum_rating < temp.rating && temp.fitness < TimeLimit) {
            maximum_rating = temp.rating;
            best_result = temp.gnome;
        }
	}
    sort(population.begin(), population.end(), morethen);

	cout << "\nInitial population: " << endl
		<< "GNOME	 FITNESS VALUE     RATING\n";
	for (int i = 0; i < POP_SIZE; i++)
		cout << population[i].gnome << "         "
			<< population[i].fitness << "         " << population[i].rating << endl;
	cout << "\n";
	int temperature = 100000;

	// Iteration to perform
	// population crossing and gene mutation.
	while (temperature > 1000 && gen <= gen_thres) {
		sort(population.begin(), population.end(), morethen);
		//cout << "\nCurrent temp: " << temperature << "\n";
		vector<struct individual> new_population;

            int rand_spice1 = rand_num(0, POP_SIZE - 1);
            int rand_spice2 = rand_num(0, POP_SIZE - 1);
            while (rand_spice1 == rand_spice2) { rand_spice2 = rand_num(0, POP_SIZE - 1); }
            pair<string, string> par = selection(population[rand_spice1].gnome, population[rand_spice2].gnome);

            for (int k = 0; k < 2; ++k) {

			struct individual p1;
            if(k == 0) {
                p1.gnome = par.first;
                p1.fitness = cal_fitness(par.first, map);
                p1.rating = fitness_rating(par.first);
            }
            else {
                p1.gnome = par.second;
                p1.fitness = cal_fitness(par.second, map);
                p1.rating = fitness_rating(par.second);
            }


            int max_iterations = 0;
                max_iterations++;
				string new_g = mutatedGene(p1.gnome);
				struct individual new_gnome;
				new_gnome.gnome = new_g;
				new_gnome.fitness = cal_fitness(new_gnome.gnome, map);
                new_gnome.rating = fitness_rating(new_gnome.gnome);

                if(new_gnome.rating > maximum_rating && new_gnome.fitness < TimeLimit) {
                    maximum_rating = new_gnome.rating;
                    best_result = new_gnome.gnome;
                    best_fitness = new_gnome.fitness;
                }

                bool found_flag = false;

                for(int i = 0; i < POP_SIZE; ++i) {
                    if (new_gnome.fitness < population[i].fitness && new_gnome.fitness <= TimeLimit && new_gnome.rating > population[i].rating
                        || (new_gnome.fitness < population[i].fitness && new_gnome.fitness >= TimeLimit && new_gnome.rating > population[i].rating)
                        || (new_gnome.fitness > population[i].fitness) && new_gnome.fitness <= TimeLimit && new_gnome.rating > population[i].rating) {
                        population.erase(population.begin() + i);
                        population.insert(population.begin() + i, new_gnome);
                        found_flag = true;
                        break;
                    }
                }
                if (!found_flag) {
                    for(int i = 0; i < POP_SIZE; ++i) {
                        if(new_gnome.fitness <= 2*TimeLimit && 0.9*maximum_rating > population[i].rating &&
                           (new_gnome.fitness < population[i].fitness && new_gnome.rating <= population[i].rating
                            || new_gnome.fitness > population[i].fitness && new_gnome.rating >= population[i].rating)) {
                            int rand = rand_num(1,100);
                            found_flag = true;
                            if(rand >= 75) {
                                population.erase(population.begin() + i);
                                population.insert(population.begin() + i, new_gnome);
                            }
                            break;
                        }
                    }
                }
                if(!found_flag) {
                    for(int i = 0; i < POP_SIZE; ++i) {
                        if(population[i].fitness > TimeLimit ) {
                            struct individual g;
                            g.gnome = create_gnome();
                            g.fitness = cal_fitness(g.gnome, map);
                            g.rating = fitness_rating(g.gnome);
                            population.erase(population.begin() + i);
                            population.insert(population.begin() + i, g);
                            break;
                        }
                    }
                }
		}

        for(int t = 0; t < POP_SIZE; ++t) {
            if (maximum_rating < population[t].rating && population[t].fitness < TimeLimit) {
                maximum_rating = population[t].rating;
                best_result = population[t].gnome;
                best_fitness = population[t].fitness;
            }
        }


        cout << endl;
        cout << "Generation " << gen << " \n";
        cout << "Best Result " << best_result << endl;
        cout << "Max Rating " << maximum_rating << endl;
        cout << "Fitness " << best_fitness << endl;
        cout << "Time Limit " << TimeLimit << endl;

		temperature = cooldown(temperature);
		//population = new_population;

		cout << "GNOME	 FITNESS VALUE    RATING\n";

		for (int i = 0; i < POP_SIZE; i++)
			cout << population[i].gnome << "         "
				<< population[i].fitness << "         " << population[i].rating << endl;
		gen++;
	}
}

int main()
{

//	int map[V][V] = { { 0, 2, INT_MAX, 12, 5 },
//					{ 2, 0, 4, 8, INT_MAX },
//					{ INT_MAX, 4, 0, 3, 3 },
//					{ 12, 8, 3, 0, 10 },
//					{ 5, INT_MAX, 3, 10, 0 } };

    int    map[V][V];
    Source s1(R"(C:\Users\zolin\University\Optimization_methods\coursework\gen_algorithm\outfile.txt)");
    s1.Read();
    Source s2(R"(C:\Users\zolin\University\Optimization_methods\coursework\gen_algorithm\rating.txt)");
    s2.Read();

    for(int k = 0; k < V; ++k) {
        RatingVector[k] = s2.vec[k];
    }

    int count = 0;
    for(int k = 0; k < V; ++k) {
        for(int j = 0; j < V; ++j) {
            map[k][j] = s1.vec[count];
            count++;
        }
    }

	TSPUtil(map);
}


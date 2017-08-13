#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <utility>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>

using namespace std;

#define P 7 //number of partitions loaded 
#define VALENCE 3
#define MAX_V 26
#define USAGE "Usage: ./gonality_check n infile outfile\n"
#define FILE_ERROR "Error reading file: "
#define PARTITION_ERROR "Not enough partitions loaded\n"

// adjacency list (abusing trivalence)
int graph[MAX_V][VALENCE];
int divisor[MAX_V];
int divisor_temp[MAX_V];

int V; // vertices per graph
int N; // number of graphs

int load_next(ifstream& fin)
{
    int graph_n;
    int cur_vertex;
    fin >> graph_n >> V;
    for (int j = 0; j < V; j++)
    {
        fin >> cur_vertex;
        for (int k = 0; k < VALENCE; k++)
        {
            fin >> graph[cur_vertex][k];
        }
    }
    return graph_n;
}


void fire(int v)
{
    divisor[v] -= VALENCE;
    for (int w = 0; w < VALENCE; w++)
    {
        divisor[graph[v][w]]++;
    }
}

//dhar's burning algorithm from v
void dhar(int v)
{
    set<int> burned;
    burned.insert(v);
    while (burned.size() < V)
    {
        for (int w = 0; w < V; w++)
        {
            if (burned.count(w) == 0)
            {   
                int fire = 0;
                for (int u = 0; u < VALENCE; u++)
                {
                    fire += burned.count(graph[w][u]);
                }
                if (fire > divisor[w])
                {
                    burned.insert(w);
                    w = -1;
                }
            }
        }
        // nothing left to burn - fire every unburnt vertex and start over
        for (int w = 0; w < V; w++)
        {
            if (burned.count(w) == 0)
            {
                fire(w);
            }
        }
        if (burned.size() < V)
        {
            burned.erase(burned.begin(), burned.end());
            burned.insert(v);
        }
    }
}

// check if current (effective) divisor has positive rank
bool divisor_posrank(void)
{
    memcpy(divisor_temp, divisor, V * sizeof(int));
    for (int v = 0; v < V; v++)
    {
        if (divisor[v] == 0)
        {
            // remove chip and v-reduce with Dhar's burning algorithm
            divisor[v] = -1;
            dhar(v);
            // if the divisor is not equivalent to an effective one then the rank is 0
            if (divisor[v] < 0)
            {
                memcpy(divisor, divisor_temp, V * sizeof(int));
                return false;
            }
            memcpy(divisor, divisor_temp, V * sizeof(int));
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    // argument checking
    if (argc != 4)
    {
        cout << USAGE;
        return 1;
    }
    const char* infile = argv[2];
    const char* outfile = argv[3];
    ifstream fin(infile);
    if (!fin.good())
    {
        cout << FILE_ERROR << infile;
        return 1;
    }
    N = atoi(argv[1]);
    if (N <= 0)
    {
        cout << USAGE;
        return 1;
    }

    load_next(fin);
    int genus = V / 2 + 1;
    int degree = (genus + 3) / 2;
    cout << "GENUS: " << genus << "\n";
    cout << "CONJECTURED DEGREE: " << degree << "\n";

    if (degree > P)
    {
        cout << PARTITION_ERROR;
        return 1;
    }

    if (V > MAX_V)
    {
        cout << USAGE;
        return 1;
    }
    //fill hard-coded partitions
    vector<vector<vector<int> > > partitions;
    for (int i = 0; i < P; i++)
    {
        char filename[10];
        sprintf(filename, "p%i.txt", i + 1);
        ifstream pfile((const char*) filename);
        int n_p;
        pfile >> n_p;
        vector<vector<int> > ipartitions;
        for (int j = 0; j < n_p; j++)
        {
            int m;
            pfile >> m;
            vector<int> partition;
            for (int k = 0; k < m; k++)
            {
                int p;
                pfile >> p;
                partition.push_back(p);
            }
            ipartitions.push_back(partition);
        }
        partitions.push_back(ipartitions);
        pfile.close();
    }

    ofstream fout(outfile);
    int fail = 0;
    for (int k = 1; k <= N; k++)
    {
        // partition the degree
        bool gonality_holds = false;
        for (int pi = 0; pi < partitions[degree - 1].size(); pi++)
        {
            int idx = V - partitions[degree - 1][pi].size();
            for (int i = idx; i < V; i++)
            {
                divisor[i] = partitions[degree - 1][pi][i - idx];
            }
            for (int i = 0; i < idx; i++)
            {
                divisor[i] = 0;
            }
            do
            {
                if (divisor_posrank())
                {
                    gonality_holds = true;
                    cout << "\033[1;32mGRAPH #" << k << "; DIVISOR OF CONJECTURED DEGREE: ";
                    for (int j = 0; j < V; j++)
                    {
                        cout << divisor[j] << " ";
                        fout << divisor[j] << " ";
                    }
                    cout << "\033[0m\n";
                    fout << "\n";
                    break;
                }
            }
            while(next_permutation(divisor, divisor + V));
            if (gonality_holds)
                break;
        }
        if (!gonality_holds)
        {
            fail++;
            cout << "\033[1;31mGonality conjecture does not hold for graph #" << k << "\033[0m\n";
            for (int j = 0; j < V; j++)
            {
                fout << "-1 ";
            }
            fout << "\n";
        }
        if (k < N)
            load_next(fin);
    }
    cout << fail << "/" << N << " fail the gonality conjecture\n";
    fin.close();
    fout.close();
}


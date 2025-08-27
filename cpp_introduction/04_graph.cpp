#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <deque> // double ended queue (push at one end, pop at other end)


std::vector<std::string> Names; 
std::vector<std::vector<unsigned int> > V; // vector of the nodes and what they are adjacent to (adjacency list)
std::map<std::string, unsigned int> V_idx; // map from string to position

unsigned id_of_vertex(const std::string & name) {
    unsigned u;
    auto itr = V_idx.find(name); // itr will point to a key(string)-value(unsigned) pair
    if (itr != V_idx.end()) {
        u = (*itr).second; // the second thing in the pair is the value(unsigned)... could also do `itr->second`...
    } else { // not there
        u = V.size();
        Names.push_back(name);
        std::vector<unsigned int> u_adv;
        V.push_back(u_adv);
        V_idx[name] = u;
    }
    return u;
}

int read_graph(const char * fname) {
    std::ifstream in(fname); // in is an object of class std::ifstream. we construct it with the constructor, which takes the filename and opens the file (and C++ is smart enough to work with both C strings (char *) and C++ strings). alternatively could be opened explicitly using the .open() member function (which also takes the filename as parameter)
    if (!in) { // also works in C, vecxause in C the file is a pointer. here it is an object that has a member function for the `!` operator.
        return 0; // error
    }
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream line_input;
        line_input.str(line); // alternatively we could directly use the constructor to put the string into the istringstream: `std::istringstream line_input(line);`
        // `line_input` now is an input stream => now we can use the same syntax that we used to read integers, floats, strings, etc

        std::string u_name, v_name;
        unsigned u, v;

        // ignore empty lines
        if (!(line_input >> u_name)) continue;
        
        // 1. create u, if not already there
        u = id_of_vertex(u_name);

        // 2. read v1, v2, ... and create (u,v1), (u,v2), ...
        while (line_input >> v_name) {
            v = id_of_vertex(v_name);
            V[u].push_back(v);
        }
    }
    return 1;
}

void bfs(unsigned s) {
    std::vector<bool> Visited(V.size()); // using the constructor for vectors... default value for bool is false... if we wanted a different value, we could specify it in the constructor
    std::deque<unsigned> Q;
    Q.push_back(s);
    Visited[s] = true;
    while (!Q.empty()) {
        unsigned u = Q.front();
        Q.pop_front();
        for (auto v : V[u]) {
            if (!Visited[v]) {
                Visited[v] = true;
                Q.push_back(v);
                std::cout << Names[v] << " ";
            }
        }
    }
    std::cout << std::endl;
}

int main(int argc, char * argv[]) {

    const char * g_fname = "GRAPH";
    if (argc > 1) {
        g_fname = argv[1];
    }

    if (!read_graph(g_fname)) {
        std::cerr << "could not read graph from file `" << g_fname << "`" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Graph Adjacency List:" << std::endl;
    for (auto u_name : Names) {
        std::cout << u_name << ": ";
        auto u = V_idx[u_name];
        for (auto v : V[u]) {
            std::cout << " " << Names[v];
        }
        std::cout << std::endl;
    }

    std::string u_name;
    std::cout << "Write the vertex you want to know the dependencies:" << std::endl;
    std::cin >> u_name;
    // unsigned u = V_idx[u_name]; // not a good idea! creates a new key-value pair in the case u_name is not a vertex name!!!!!! do not want to add vertex from a user input...!!!!!
    auto itr = V_idx.find(u_name);
    if (itr == V_idx.end()) {
        std::cout << "unknown vertex `" << u_name << "`" << std::endl;
    } else {
        std::cout << "Dependencies of vertex:" << std::endl;
        bfs(itr->second);
    }
}

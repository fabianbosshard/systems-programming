#include <iostream>
#include <string>
#include <set>
#include <map>

#include "rowing.h"


struct boat {
    unsigned num_of_excursions;
    unsigned km_travelled;
    unsigned max_crew;
    struct excursion * e;
};

struct member {
    unsigned num_of_excursions;
    unsigned km_travelled;
    struct excursion * e;
};


struct excursion {
    struct club * c;
    std::set<std::string> crew;
    std::string boat;
};

struct club {
    std::map<std::string, struct member> Members;
    std::map<std::string, struct boat> Boats;
};


std::set<struct excursion *> current_excursions; // some tests use an excursion pointer after the corresponding excursion has been deleted due to cancel / close. thus we need to keep track of the excursion objects that are alive.


struct club * create_club () {
    struct club * c = new struct club;
    return c;
}

void destroy_club (struct club * c) {
    if (c) delete c;
}

int add_boat (struct club * c, const char * name, unsigned int max_crew) {
    if (!c) return 0;
    if (c->Boats.find(name) != c->Boats.end() || max_crew < 1) return 0;
    struct boat bv;
    bv.e = NULL;
    bv.max_crew = max_crew;
    bv.km_travelled = 0;
    bv.num_of_excursions = 0;
    c->Boats[name] = bv;
    return 1;
}

int add_person (struct club * c, const char * name) {
    if (!c) return 0;
    if (c->Members.find(name) != c->Members.end()) return 0;
    struct member mv;
    mv.e = NULL;
    mv.km_travelled = 0;
    mv.num_of_excursions = 0;
    c->Members[name] = mv;
    return 1;
}

struct excursion * create_excursion (struct club * c) {
    if (!c) return NULL;
    struct excursion * e = new struct excursion;
    e->c = c;
    current_excursions.insert(e);
    return e;
}

int use_boat (struct excursion * e, const char * name) {
    if (!e || current_excursions.find(e) == current_excursions.end()) return 0;
    if (e->c->Boats.find(name) == e->c->Boats.end()) return 0;
    if (e->c->Boats[name].e) return 0;
    if (e->boat != "") return 0;
    e->boat = name;
    e->c->Boats[name].e = e;
    return 1;
}

int add_crew (struct excursion * e, const char * name) {
    if (!e || current_excursions.find(e) == current_excursions.end()) return 0;
    if (e->boat == "") return 0; // error if excursion does not yet have an assigned boat
    if (e->c->Members.find(name) == e->c->Members.end()) return 0; // person does not exist
    if (e->c->Members[name].e) return 0; // person already on different excursion
    if (e->crew.size() >= e->c->Boats[e->boat].max_crew) return 0;
    e->crew.insert(name);
    e->c->Members[name].e = e;
    return 1;
}

int close_excursion (struct excursion * e, unsigned int k) {
    if (!e || current_excursions.find(e) == current_excursions.end()) return 0;
    if (e->boat == "") return 0;
    if (e->crew.empty()) return 0;

    // update boat stats
    e->c->Boats[e->boat].e = NULL;
    e->c->Boats[e->boat].km_travelled += k;
    e->c->Boats[e->boat].num_of_excursions++;
    
    // update crew members stats
    for (auto & name : e->crew) {
        e->c->Members[name].e = NULL;
        e->c->Members[name].km_travelled += k;
        e->c->Members[name].num_of_excursions++;
    }

    current_excursions.erase(e);
    delete e;
    return 1;
}

void cancel_excursion (struct excursion * e) {
    if (!e || current_excursions.find(e) == current_excursions.end()) return;

    // free boat
    if (e->boat != "") {
        e->c->Boats[e->boat].e = NULL;
    }
    
    // free crew members
    for (auto & name : e->crew) {
        e->c->Members[name].e = NULL;
    }

    current_excursions.erase(e);
    delete e;
}

int get_crew_excursions (struct club * c, const char * name) {
    if (!c) return -1;
    if (c->Members.find(name) == c->Members.end()) return -1;
    return c->Members[name].num_of_excursions;
}

int get_crew_kilometers (struct club * c, const char * name) {
    if (!c) return -1;
    if (c->Members.find(name) == c->Members.end()) return -1;
    return c->Members[name].km_travelled;
}

int boat_available (struct club * c, const char * name) {
    if (!c) return -1;
    if (c->Boats.find(name) == c->Boats.end()) return -1;
    if (c->Boats[name].e) return 0;
    return 1;
}

int get_boat_excursions (struct club * c, const char * name) {
    if (!c) return -1;
    if (c->Boats.find(name) == c->Boats.end()) return -1;
    return c->Boats[name].num_of_excursions;
}

int get_boat_kilometers (struct club * c, const char * name) {
    if (!c) return -1;
    if (c->Boats.find(name) == c->Boats.end()) return -1;
    return c->Boats[name].km_travelled;
}

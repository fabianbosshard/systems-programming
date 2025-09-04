

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

#include "rooms.h"

struct Event {
    std::string name;
    int start;
    int finish;
};

struct Room {
    int floor;
    int number;
    int capacity;
    std::list<struct Event> E;
};

std::vector<struct Room> R;

void clear() {
    R.clear();
}

struct Room * find_room(int floor, int number) {
    for (auto & r : R) {
        if (r.floor == floor && r.number == number) {
            return &r;
        }
    }
    return NULL;
}

void add_rooms(const room * begin, const room * end) {
    for (const room * i = begin; i != end; i++) {
        struct Room * r;
        if ((r = find_room(i->floor, i->number))) {
            r->capacity = i->capacity;
        } else {
            struct Room r_;
            r = &r_;
            r->capacity = i->capacity;
            r->floor = i->floor;
            r->number = i->number;
            R.push_back(*r);
        }
    }
}

bool compare_events(struct Event a, struct Event b) {
    return a.start < b.start;
}
bool compare_rooms(struct Room a, struct Room b) {
    if (a.capacity != b.capacity) return a.capacity < b.capacity;
    if (a.floor != b.floor) return a.floor < b.floor;
    return a.number < b.number;
}
bool compare_rooms_print(struct Room a, struct Room b) {
    if (a.floor != b.floor) return a.floor < b.floor;
    return a.number < b.number;
}
void sort_rooms(std::vector<struct Room> & R, bool print) {
    for (auto & r : R) {
        r.E.sort(compare_events);
    }
    std::sort(R.begin(), R.end(), print ? compare_rooms_print : compare_rooms);
}

void insert_event(std::list<struct Event>::iterator itr, int starting, struct Room & r_, struct room * r, struct schedule * t, const char * event_name) {
    t->start = starting;
    t->finish = starting + t->duration;
    struct Event e_;
    e_.name = std::string(event_name);
    e_.start = t->start;
    e_.finish = t->finish;
    r_.E.insert(itr, e_); // same as next(r_.E.begin(), i) if itr points to the i'th node in the list
    r->capacity = r_.capacity;
    r->floor = r_.floor;
    r->number = r_.number;
}

bool try_reservation(struct Room & r_, room * r, schedule * t, const char * event_name) {
    if (r->capacity > r_.capacity && r->capacity != ANY_CAPACITY) return false;
    if (r->floor != r_.floor && r->floor != ANY_FLOOR) return false;
    if (r->number != r_.number && r->number != ANY_ROOM_NUMBER) return false;

    if (r_.E.empty()) {
        insert_event(r_.E.begin(), t->start, r_, r, t, event_name);
        return true;
    }
    int prev_finish = 0;
    for (auto i = r_.E.begin(); i != r_.E.end(); i++) {
        if (std::max(prev_finish, t->start) + t->duration <= std::min(i->start, t->finish)) {
            insert_event(i, std::max(prev_finish, t->start),  r_, r, t, event_name);
            return true;
        }
        prev_finish = i->finish;
    }
    if (std::max(prev_finish, t->start) + t->duration <= t->finish) {
        insert_event(r_.E.end(), std::max(prev_finish, t->start),  r_, r, t, event_name);
        return true;
    }
    return false;
}

int make_reservation(room * r, schedule * t, const char * event) {
    sort_rooms(R, false);
    for (auto & r_ : R) {
        if (try_reservation(r_, r, t, event)) {
            return 1;
        }
    }
    return 0;
}

int cancel_reservation(int floor, int number, int start) {
    for (auto & r_ : R) {
        if (r_.floor == floor && r_.number == number) {
            for (auto e = r_.E.begin(); e != r_.E.end(); e++) {
                if (e->start == start) {
                    r_.E.erase(e);
                    return 1;
                }
            }
        }
    }
    return 0;
}

void print_schedule(std::ostream & output, int floor, int number, int start, int finish) {
    sort_rooms(R, true);
     for (auto & r_ : R) {
        if ((floor == ANY_FLOOR || r_.floor == floor) && (number == ANY_ROOM_NUMBER || r_.number == number)) {
            output << "room " << r_.number << " floor " << r_.floor << ":" << std::endl;
            for (auto & e_ : r_.E) {
                if (e_.start >= start && e_.start < finish) {
                    output << "event " << e_.start << " " << e_.finish << " " << e_.name << std::endl;
                }
            }
        }
    }
}

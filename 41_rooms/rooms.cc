

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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
    std::vector<struct Event> E;
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
            struct Room r;
            r.capacity = i->capacity;
            r.floor = i->floor;
            r.number = i->number;
            R.push_back(r);
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
        std::sort(r.E.begin(), r.E.end(), compare_events);
    }
    std::sort(R.begin(), R.end(), print ? compare_rooms_print : compare_rooms);
}

void insert_event(int i, int starting, struct Room & r_, struct room * r, struct schedule * t, const char * event_name) {
    t->start = starting;
    t->finish = starting + t->duration;
    struct Event e_;
    e_.name = std::string(event_name);
    e_.start = t->start;
    e_.finish = t->finish;
    r_.E.insert(r_.E.begin() + i, e_);
    r->capacity = r_.capacity;
    r->floor = r_.floor;
    r->number = r_.number;
}

bool try_reservation(struct Room & r_, room * r, schedule * t, const char * event_name) {
    if (r->capacity > r_.capacity && r->capacity != ANY_CAPACITY) {
        return false;
    }
    if (r->floor != r_.floor && r->floor != ANY_FLOOR) {
        return false;
    }
    if (r->number != r_.number && r->number != ANY_ROOM_NUMBER) {
        return false;
    }
    if (r_.E.empty()) {
        insert_event(0, (t->start > 0) ? t->start : 0, r_, r, t, event_name);
        return true;
    }
    int i; // try putting the new event before the i'th event in the event list
    for (i = 0; i <= r_.E.size(); i++) {
        if (i == 0) { 
            if ((t->duration <= r_.E[i].start - 0) && (0 <= t->finish - t->duration) && (t->start + t->duration <= r_.E[i].start)) {
                insert_event(i, (t->start > 0) ? t->start : 0,  r_, r, t, event_name);
                return true;
            }
        } else if (i == r_.E.size()) { 
            if (r_.E[i-1].finish <= t->finish - t->duration) {
                insert_event(i, (t->start > r_.E[i-1].finish) ? t->start : r_.E[i-1].finish, r_, r, t, event_name);
                return true;
            }
        } else { 
            if ((t->duration <= r_.E[i].start - r_.E[i-1].finish) && (r_.E[i-1].finish <= t->finish - t->duration) && (t->start + t->duration <= r_.E[i].start)) {
                insert_event(i, (t->start > r_.E[i-1].finish) ? t->start : r_.E[i-1].finish, r_, r, t, event_name);
                return true;
            }
        }
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
            for (int i = 0; i < r_.E.size(); i++) {
                if (r_.E[i].start == start) {
                    r_.E.erase(r_.E.begin() + i);
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

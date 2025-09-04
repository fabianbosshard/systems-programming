#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <list>
#include <vector>

#include "rooms.h"

using namespace std;

struct Event
{
    int start;
    int finish;
    string name;
    Event(int s, int f, const char *d) : start(s), finish(f), name(d) {};
};

struct Room
{
    int floor;
    int number;
    int capacity;
    list<Event> E;
    Room(int f, int n, int c) : floor(f), number(n), capacity(c) {};
    bool make_reservation(room *r, schedule *t, const char *name);
    int cancel_reservation(int start);
    void print_schedule(ostream &output, int start, int finish) const;
};

bool operator<(const Room &r1, const Room &r2)
{
    if (r1.capacity < r2.capacity)
        return true;
    if (r1.capacity > r2.capacity)
        return false;
    if (r1.floor < r2.floor)
        return true;
    if (r1.floor > r2.floor)
        return false;
    if (r1.number < r2.number)
        return true;
    return false;
}

bool Room::make_reservation(room *r, schedule *t, const char *name)
{
    int start = t->start;
    auto itr = E.begin();
    for (; itr != E.end(); ++itr)
    {
        if (start + t->duration <= itr->start)
        {
            break;
        }
        else if (itr->finish > start)
        {
            start = itr->finish;
            if (start + t->duration > t->finish)
                return false;
        }
    }
    r->floor = floor;
    r->number = number;
    r->capacity = capacity;
    t->start = start;
    t->finish = start + t->duration;
    E.insert(itr, Event(start, start + t->duration, name));
    return true;
}

int Room::cancel_reservation(int start)
{
    for (auto itr = E.begin(); itr != E.end(); ++itr)
    {
        if (itr->start == start)
        {
            E.erase(itr);
            return 1;
        }
    }
    return 0;
};

void Room::print_schedule(ostream &output, int start, int finish) const
{
    output << "room " << number << " floor " << floor << ":\n";
    for (auto & e_ : E)
        if (e_.finish > start && e_.start < finish)
            output << "event " << e_.start << " " << e_.finish << " " << e_.name << "\n";
}

vector<Room> R;

struct room_id
{
    int floor;
    int number;

    room_id(int f, int n) : floor(f), number(n) {};
};

bool operator<(const room_id &r1, const room_id &r2)
{
    return r1.floor < r2.floor || (r1.floor == r2.floor && r1.number < r2.number);
}

map<room_id, unsigned int> R_idx;

void clear()
{
    R.clear();
    R_idx.clear();
}

void add_rooms(const room *begin, const room *end)
{
    for (const room *r = begin; r != end; ++r)
    {
        auto idx_itr_bool = R_idx.insert(make_pair(room_id(r->floor, r->number), R.size()));
        if (idx_itr_bool.second)
        { // new room
            R.push_back(Room(r->floor, r->number, r->capacity));
        }
        else
        {
            R[idx_itr_bool.second].capacity = r->capacity;
        }
    }
    sort(R.begin(), R.end());
    for (unsigned int i = 0; i < R.size(); ++i)
        R_idx[room_id(R[i].floor, R[i].number)] = i;
}

int make_reservation(room *r, schedule *t, const char *name)
{
    for (auto & r_ : R)
        if ((r->floor == ANY_FLOOR || r_.floor == r->floor) && (r->number == ANY_ROOM_NUMBER || r_.number == r->number) && (r->capacity == ANY_CAPACITY || r->capacity <= r_.capacity))
            if (r_.make_reservation(r, t, name))
                return 1;
    return 0;
}

int cancel_reservation(int floor, int number, int start)
{
    auto idx_itr = R_idx.find(room_id(floor, number));
    if (idx_itr != R_idx.end())
        return R[idx_itr->second].cancel_reservation(start);
    return 0;
}

void print_schedule(ostream &output, int floor, int number, int start, int finish)
{
    for (auto &idx_pair : R_idx)
    {
        const Room &d = R[idx_pair.second];
        if ((floor == ANY_FLOOR || d.floor == floor) && (number == ANY_ROOM_NUMBER || d.number == number))
            d.print_schedule(output, start, finish);
    }
}

#include "tank_control.h"

struct tank {
    unsigned cap;
    unsigned liq;
    unsigned was;
    unsigned bot;
};

struct tank t = {1000000, 0, 0, 750};

void clear() {
    t.liq = 0;
    t.was = 0;
}

void change_bottle_capacity(unsigned int c) {
    t.bot = c;
}

void change_tank(unsigned int c) {
    if (c < t.liq) {
        t.was += t.liq - c;
        t.liq = c;
    }
    t.cap = c;
}

void add(unsigned int c) {
    if (c + t.liq > t.cap) {
        t.was += c + t.liq - t.cap;
        t.liq = t.cap;
    } else {
        t.liq += c;
    }
}

unsigned int ship_out_bottles() {
    unsigned num = t.liq / t.bot;
    t.liq -= num * t.bot;
    return num;
}

unsigned int get_wastes() {
    return t.was;
}

unsigned int get_tank_level() {
    return t.liq;
}

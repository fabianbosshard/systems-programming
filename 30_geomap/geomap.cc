#include <iostream>
#include <string>
#include <list>

#include "geomap.h"



struct point {
    double x;
    double y;
    std::string descr;
};
struct rectangle {
    double left_x;
    double bottom_y;
    double right_x;
    double top_y;
    std::string descr;
};

struct geomap {
    std::list<struct point> P;
    std::list<struct rectangle> R;
};

/* Create and return a new geomap object.  Return 0 on failure. */
struct geomap * geomap_new() {
    struct geomap * m = new struct geomap;
    return m;
}

/* Destroy the given geomap object and release all memory allocated by it. */
void geomap_delete(struct geomap * m) {
    delete m;
}

void geomap_clear(struct geomap * m) {
    m->P.clear();
    m->R.clear();
}

/* Add a point with the given coordinates and description.
 * Return 0 on failure, 1 on success. */
int geomap_add_point(struct geomap * m, double x, double y, const char * descr) {
    // for (auto & p : m->P) {
    //     if (p.x == x && p.y == y && p.descr == descr) return 0;
    // }
    struct point p = {x, y, std::string(descr)};
    m->P.push_back(p);
    return 1;
}

/* Add a rectangle with the given coordinates and description.
 * Return 0 on failure, 1 on success */
int geomap_add_rectangle(struct geomap * m, double left_x, double bottom_y, double right_x, double top_y, const char * descr) {
    // for (auto & r : m->R) {
    //     if (r.left_x == left_x && r.bottom_y == bottom_y && r.right_x == right_x && r.top_y == top_y && r.descr == descr) return 0;
    // }
    struct rectangle r = {left_x, bottom_y, right_x, top_y, std::string(descr)};
    m->R.push_back(r);
    return 1;
}

/* Remove a point with the given coordinates and description.
 * Return 0 if not found, 1 if found (and removed). */
int geomap_remove_point(struct geomap * m, double x, double y, const char * descr) {
    for (auto p = m->P.begin(); p != m->P.end(); p++) {
        if (p->x == x && p->y == y && p->descr == descr) {
            m->P.erase(p);
            return 1;
        }
    }
    return 0;
}

/* Remove a rectangle with the given coordinates and description.
 * Return 0 if not found, 1 if found (and removed). */
int geomap_remove_rectangle(struct geomap * m, double left_x, double bottom_y, double right_x, double top_y, const char * descr) {
    for (auto r = m->R.begin(); r != m->R.end(); r++) {
        if (r->left_x == left_x && r->bottom_y == bottom_y && r->right_x == right_x && r->top_y == top_y && r->descr == descr) {
            m->R.erase(r);
            return 1;
        }
    }
    return 0;
}

typedef void (*point_callback)(double x, double y, const char * descr);
typedef void (*rectangle_callback)(double left_x, double bottom_y, double right_x, double top_y, const char * descr);




/* Iterate over all the elements in the geomap.
 * If the point callback is not null, calls that function with each point.
 * If the rectangle callback is not null, calls that function with each rectangle.
 * Return the number of elements in the iteration. */
size_t geomap_iterate_all(struct geomap * m, point_callback pcb, rectangle_callback rcb) {
    size_t N = m->P.size() + m->R.size();
    if (pcb) {
        for (auto & p : m->P) {
            pcb(p.x, p.y, p.descr.c_str());
        }
    }
    if (rcb) {
        for (auto & r : m->R) {
            rcb(r.left_x, r.bottom_y, r.right_x, r.top_y, r.descr.c_str());
        }
    }
    return N;
}

bool contains(const struct point & p, const struct rectangle & r) {
    return (r.left_x <= p.x && p.x <= r.right_x && r.bottom_y <= p.y && p.y <= r.top_y);
}
bool intersect(const struct rectangle & r1, const struct rectangle & r2) {

    if (contains({r1.left_x, r1.bottom_y, "bottomleft"}, r2)) return true;
    if (contains({r1.right_x, r1.bottom_y, "bottomright"}, r2)) return true;
    if (contains({r1.right_x, r1.top_y, "topright"}, r2)) return true;
    if (contains({r1.left_x, r1.top_y, "topleft"}, r2)) return true;

    if (contains({r2.left_x, r2.bottom_y, "bottomleft"}, r1)) return true;
    if (contains({r2.right_x, r2.bottom_y, "bottomright"}, r1)) return true;
    if (contains({r2.right_x, r2.top_y, "topright"}, r1)) return true;
    if (contains({r2.left_x, r2.top_y, "topleft"}, r1)) return true;

    return false;
}

/* Iterate over the element in the geomap that appear in a given region.
 * If the point callback is not null, calls that function with each point.
 * If the rectangle callback is not null, calls that function with each rectangle.
 * Return the number of elements in the iteration. */
size_t geomap_iterate_in_region(struct geomap * m, point_callback pcb, rectangle_callback rcb, double left_x, double bottom_y, double right_x, double top_y) {
    size_t N = 0;
    struct rectangle A = {left_x, bottom_y, right_x, top_y, "area"};

    for (auto & p : m->P) {
        if (contains(p, A))
        if (left_x <= p.x && p.x <= right_x && bottom_y <= p.y && p.y <= top_y) {
            if (pcb) pcb(p.x, p.y, p.descr.c_str());
            N++;
        }
    }

    for (auto & r : m->R) {
        if (intersect(r, A)) {
            if (rcb) rcb(r.left_x, r.bottom_y, r.right_x, r.top_y, r.descr.c_str());
            N++;
        }
    }

    return N;
}



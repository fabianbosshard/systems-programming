#ifndef GEOMAP_H_INCLUDED
#define GEOMAP_H_INCLUDED

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


struct geomap;

/* Create and return a new geomap object.  Return 0 on failure. */
struct geomap * geomap_new();

/* Destroy the given geomap object and release all memory allocated by it. */
void geomap_delete(struct geomap * m);

void geomap_clear(struct geomap * m);

/* Add a point with the given coordinates and description.
 * Return 0 on failure, 1 on success. */
int geomap_add_point(struct geomap * m, double x, double y, const char * descr);

/* Add a rectangle with the given coordinates and description.
 * Return 0 on failure, 1 on success */
int geomap_add_rectangle(struct geomap * m,
                         double left_x, double bottom_y, double right_x, double top_y,
                         const char * descr);

/* Remove a point with the given coordinates and description.
 * Return 0 if not found, 1 if found (and removed). */
int geomap_remove_point(struct geomap * m, double x, double y, const char * descr);

/* Remove a rectangle with the given coordinates and description.
 * Return 0 if not found, 1 if found (and removed). */
int geomap_remove_rectangle(struct geomap * m,
			    double left_x, double bottom_y, double right_x, double top_y,
			    const char * descr);

typedef void (*point_callback)(double x, double y, const char * descr);
typedef void (*rectangle_callback)(double left_x, double bottom_y,
                                   double right_x, double top_y,
                                   const char * descr);

/* Iterate over all the elements in the geomap.
 * If the point callback is not null, calls that function with each point.
 * If the rectangle callback is not null, calls that function with each rectangle.
 * Return the number of elements in the iteration. */
size_t geomap_iterate_all(struct geomap * m,
                          point_callback pcb, rectangle_callback rcb);

/* Iterate over the element in the geomap that appear in a given region.
 * If the point callback is not null, calls that function with each point.
 * If the rectangle callback is not null, calls that function with each rectangle.
 * Return the number of elements in the iteration. */
size_t geomap_iterate_in_region(struct geomap * m,
                                point_callback pcb, rectangle_callback rcb,
                                double left_x, double bottom_y, double right_x, double top_y);

                                #ifdef __cplusplus
}
#endif

#endif

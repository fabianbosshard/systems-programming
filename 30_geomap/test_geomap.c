#include <assert.h>
#include <stdio.h>

#include "geomap.h"

void point_check1(double x, double y, const char * descr) {

}

static const char * CIAO = "ciao";
static const char * MIAO = "miao";
static const char * BAO = "bao";
static const char * XYZ = "xyz";

int main () {
    struct geomap * m = geomap_new();
    assert(m);
    geomap_delete(m);
    struct geomap * m2 = geomap_new();
    assert(m2);
    m = geomap_new();
    assert(m);
    geomap_delete(m2);
    assert(geomap_add_point(m, 1.1, 2.2, CIAO) != 0);
    assert(geomap_iterate_all(m, 0, 0) == 1);
    assert(geomap_add_rectangle(m, 1.1, 2.2, 3.3, 4.4, MIAO) != 0);
    assert(geomap_iterate_all(m, 0, 0) == 2);
    assert(geomap_add_point(m, 1.2, 3.4, BAO) != 0);
    assert(geomap_iterate_all(m, 0, 0) == 3);
    geomap_clear(m);
    assert(geomap_iterate_all(m, 0, 0) == 0);

    assert(geomap_add_point(m, 1.1, 2.2, CIAO) != 0);
    assert(geomap_iterate_all(m, 0, 0) == 1);
    assert(geomap_iterate_in_region(m, 0, 0, -1, -1, 3, 3) == 1);
    assert(geomap_add_point(m, 2.2, 3.3, XYZ) != 0);
    assert(geomap_iterate_all(m, 0, 0) == 2);
    assert(geomap_iterate_in_region(m, 0, 0, -1, -1, 3, 3) == 1);
    assert(geomap_add_rectangle(m, 1.1, 2.2, 3.3, 4.4, MIAO) != 0);
    assert(geomap_iterate_all(m, 0, 0) == 3);
    assert(geomap_add_rectangle(m, 1.2, 3.4, 5.6, 7.8, BAO) != 0);
    assert(geomap_iterate_all(m, 0, 0) == 4);
    for (int i = 0; i < 10000; ++i) {
	assert(geomap_add_point(m, 2.2*i, 1.1*i, CIAO) != 0);
	assert(geomap_add_rectangle(m, 1.2 + 10*i, 3.4, 5.6 + 10*i, 7.8, BAO) != 0);
    }
    assert(geomap_iterate_all(m, 0, 0) == 20004);
    assert(geomap_iterate_in_region(m, 0, 0, -1, -1, 3, 3) == 4);

    assert(geomap_remove_point(m, 2.2*1, 1.1*1, CIAO) != 0);
    assert(geomap_remove_rectangle(m, 1.2 + 10*1, 3.4, 5.6 + 10*1, 7.8, BAO) != 0);

    assert(geomap_remove_point(m, 2.2*1, 1.1*1, CIAO) == 0);
    assert(geomap_remove_rectangle(m, 1.2 + 10*1, 3.4, 5.6 + 10*1, 7.8, BAO) == 0);

    assert(geomap_add_point(m, 2.2, 3.3, XYZ) != 0);
    assert(geomap_add_rectangle(m, 1.1, 2.2, 3.3, 4.4, MIAO) != 0);

    assert(geomap_remove_point(m, 2.2, 3.3, XYZ) != 0);
    assert(geomap_remove_rectangle(m, 1.1, 2.2, 3.3, 4.4, MIAO) != 0);
    assert(geomap_remove_rectangle(m, 1.1, 2.2, 3.3, 4.4, MIAO) != 0);
    assert(geomap_remove_point(m, 2.2, 3.3, XYZ) != 0);

    assert(geomap_remove_point(m, 2.2, 3.3, XYZ) == 0);
    assert(geomap_remove_rectangle(m, 1.1, 2.2, 3.3, 4.4, MIAO) == 0);

    geomap_clear(m);
    assert(geomap_iterate_all(m, 0, 0) == 0);

    assert(geomap_remove_point(m, 2.2, 3.3, XYZ) == 0);
    assert(geomap_remove_rectangle(m, 1.1, 2.2, 3.3, 4.4, MIAO) == 0);
    
    assert(geomap_add_point(m, 2.2, 3.3, XYZ) != 0);
    assert(geomap_add_rectangle(m, 1.1, 2.2, 3.3, 4.4, MIAO) != 0);

    assert(geomap_remove_point(m, 2.2, 3.3, XYZ) != 0);
    assert(geomap_remove_rectangle(m, 1.1, 2.2, 3.3, 4.4, MIAO) != 0);
    
    assert(geomap_remove_point(m, 2.2, 3.3, XYZ) == 0);
    assert(geomap_remove_rectangle(m, 1.1, 2.2, 3.3, 4.4, MIAO) == 0);
    
    assert(geomap_iterate_all(m, 0, 0) == 0);

    for (int i = 0; i < 10000; ++i) 
	assert(geomap_add_point(m, 10, 20, XYZ) != 0);

    assert(geomap_remove_point(m, 11, 20, XYZ) == 0);
    assert(geomap_remove_point(m, 10, 21, XYZ) == 0);
    assert(geomap_remove_point(m, 10, 20, CIAO) == 0);

    for (int i = 0; i < 100; ++i) 
	assert(geomap_remove_point(m, 10, 20, XYZ) != 0);

    assert(geomap_iterate_all(m, 0, 0) == 9900);

    geomap_delete(m);
    
    printf("PASSED.\n");
}

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <climits>

#include <iostream>
#include <sstream>
#include <string>
#include <array> 
#include <vector> 
#include <list>
#include <map>
#include <algorithm>
using namespace std; 

#include "s_table.h"

struct s_table {
    list<string> T;
};

string get_str(const char * begin, const char * end) {
    string str;
    for (const char * p = begin; p != end; p++) {
        str.push_back(*p);
    }
    return str;
}

/* Create and return a new s_table object.  Return 0 on failure. */
struct s_table * s_table_new() {
    // struct s_table * t = (struct s_table *) malloc(sizeof(struct s_table)); // WRONG
    struct s_table * t = new s_table;
    return t;
}

/* Destroy the given s_table object and release all memory allocated by it. */
void s_table_delete(struct s_table * t) {
    s_table_clear(t);
    delete t;
}

void s_table_clear(struct s_table * t) {
    t->T.clear();
}

/* Add a record given by the given string. Return 0 on failure, 1 on success. */
int s_table_add_record(struct s_table * t, const char * begin, const char * end) {
    string record = get_str(begin, end);
    try {
        t->T.push_back(record);
    } catch (...) {
        return 0;
    }
    return 1;
}

/* Remove a record with the given string. Return 0 if not found, 1 if found (and removed). */
int s_table_remove_record(struct s_table * t, const char * begin, const char * end) {
    string record = get_str(begin, end);
    for (auto node = t->T.begin(); node != t->T.end();) {
        if (*node == record) {
            auto node_to_delete = node;
            node++;
            t->T.erase(node_to_delete);
            return 1;
        } else {
            node++;
        }
    }
    return 0;
}

/* Record processor: takes a record and returns an int */
typedef int (*feature_extractor)(const char * begin, const char * end);

/* Remove all the records that are selected by the given callback function. A record is selected if the selector callback returns an integer value that compares TRUE (i.e., != 0). Return the number of records that were removed. */
size_t s_table_remove_records(struct s_table * t, feature_extractor selector_callback) {
    int num = 0;
    for (auto node = t->T.begin(); node != t->T.end();) {
        if (selector_callback((*node).c_str(), (*node).c_str() + (*node).length())) { // same as selector_callback(&*((*node).begin()), &*((*node).begin() + (*node).length()))
            auto node_to_delete = node;
            node++;
            t->T.erase(node_to_delete);
            num++;
        } else {
            node++;
        }
    }
    return num;
}

/* Find a record with the maximal feature extracted by the given callback function.  Copies the corresponding record in the given buffer, defined by the record char pointer and the given max buffer length.  Never copies more than record_max_len characters. Return the number of characters copied in the record buffer, or 0  if there are no records in the table. */
size_t s_table_max_feature(struct s_table * t, feature_extractor feature_callback, char * record, size_t record_max_len) {
    int max = INT_MIN;
    int cur;
    string max_record;
    for (auto node = t->T.begin(); node != t->T.end(); node++) {
        if ((cur = feature_callback(&*((*node).begin()), &*((*node).begin() + (*node).length()))) > max) {
            max_record = *node;
            max = cur;
        }
    }
    int i;
    for (i = 0; i < record_max_len && i < max_record.length(); i++) {
        record[i] = max_record[i];
    }
    return i;
}

/* Print the table on the given FILE stream, one record per line, with the records sorted in increasing order of the feature extracted by the given callback function. */
void s_table_print_sorted(struct s_table * t, FILE * f, feature_extractor feature_callback) {
    t->T.sort([&](const string& record1, const string& record2) {
        if (feature_callback(&*(record1.begin()), &*(record1.begin() + record1.length())) > feature_callback(&*(record2.begin()), &*(record2.begin() + record2.length()))) {
            return 0;
        } else {
            return 1;
        }
    });
    for (auto node = t->T.begin(); node != t->T.end(); node++) {
        fputs((*node).c_str(), f);
        putc('\n', f);
    }
}



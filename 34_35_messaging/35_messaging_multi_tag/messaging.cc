#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <string>
#include <set>
#include <map>


struct receiver {
    void (*deliver)(struct receiver * r, const char * message); // this does not declare a function, but a pointer (to a function)!
};

struct server {
    std::map<struct receiver *, std::set<std::set<std::string>>> M;
};

struct server * server_new(){
    struct server * s = new struct server; // constructor (?)
    return s;
}

void server_delete(struct server * s) {
    delete s;
}


/** BEGIN: parse tags: create set of tags from C-string */
int is_start_of_tag(const char * p) {
    if ((*p == '@' || *p == '#') && isalpha(*(p + 1))) {
        return 1;
    }
    if (isalpha(*p)) {
        return 1;
    }
    return 0;
}
const char * get_end_of_tag(const char * p) {
    while (isalpha(*p)) {
        p++;
    }
    return p;
}
std::set<std::string> parse_tags(const char * C_str) {
    std::set<std::string> tags;
    const char * p = C_str;
    while (*p != '\0') {
        if (is_start_of_tag(p)) {
            const char * begin = p;
            const char * end = get_end_of_tag(begin + 1);
            std::string tag(begin, end);
            tags.insert(tag);
            p = end;
        } else {
            p++;
        }
    }
    return tags;
}
/** END: parse */


int add_interest(struct server * srv, struct receiver * r, const char * interest) {
    std::set<std::string> inter = parse_tags(interest); // parse tags from C string (char *) into set of C++ strings
    srv->M[r].insert(inter); // add interest (set of tags) to set of interests of receiver r
    return 1;
}

void remove_interest(struct server * srv, struct receiver * r, const char * interest) {
    std::set<std::string> inter = parse_tags(interest);
    srv->M[r].erase(inter); // remove interest (set of tags) from set of interests of receiver r
}

void clear_receiver(struct server * srv, struct receiver * r) {
    srv->M[r].clear();
}

void clear_all(struct server * srv) {
    srv->M.clear();
}

void send(const struct server * srv, const char * message) {
    std::set<std::string> tags = parse_tags(message);
    for (auto P : srv->M) {
        for (auto inter : P.second) {
            for (auto tag : inter) {
                if (tags.find(tag) == tags.end()) { // if one tag of the current interest is not present in the message ...
                    goto next_interest;             // ... we move on to the next interest
                }
            }                                       // if all tags of the current interest are present in the message ...
            P.first->deliver(P.first, message);     // ... we deliver it ... 
            break;                                  // ... and move on with the next receiver
            next_interest:
        }
    }
};

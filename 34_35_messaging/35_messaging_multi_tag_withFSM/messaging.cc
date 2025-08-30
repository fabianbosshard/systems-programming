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


std::set<std::string> parse_tags(const char * C_str) {
    std::set<std::string> tags;
    std::string tag;
    const char * p = C_str;

    no_tag:
    if (*p == '@' || *p == '#') {
        tag.push_back(*p);
        p++; 
        goto prefix_tag;
    } else if (isalpha(*p)) {
        tag.push_back(*p);
        p++; 
        goto is_tag;
    } else if (*p == '\0') {
        goto end_tags;
    } else {
        p++;
        goto no_tag;
    }
        

    prefix_tag:
    if (*p == '@' || *p == '#') {
        tag.clear();
        tag.push_back(*p);
        p++; 
        goto prefix_tag;
    } else if (isalpha(*p)) {
        tag.push_back(*p);
        p++; 
        goto is_tag;
    } else if (*p == '\0') {
        goto end_tags;
    } else {
        p++;
        tag.clear();
        goto no_tag;
    }

    is_tag:
    if (*p == '@' || *p == '#') {
        tags.insert(tag);
        tag.clear();
        p++; 
        goto prefix_tag;
    } else if (isalpha(*p)) {
        tag.push_back(*p);
        p++; 
        goto is_tag;
    } else if (*p == '\0') {
        tags.insert(tag);
        goto end_tags;
    } else {
        tags.insert(tag);
        tag.clear();
        p++;
        goto no_tag;
    }

    end_tags:
    return tags;
}


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

bool is_subset(const std::set<std::string> & A, const std::set<std::string> & B) {
    for (const auto & a : A) {
        if (B.find(a) == B.end()) {
            return false;           
        }
    }  
    return true;                
}

void send(const struct server * srv, const char * message) {
    std::set<std::string> tags = parse_tags(message);
    for (const auto & P : srv->M) {
        struct receiver * receiver = P.first;
        std::set<std::set<std::string>> interests = P.second;
        for (const auto & interest : interests) {
            if (is_subset(interest, tags)) {
                receiver->deliver(receiver, message);  
                break;                     
            }
        }
    }
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processes.h"


struct process {
    char user[9], command[16];
    int pid, ppid, prio;
    float cpuu;
    long int rss, size, vsize;
};

struct processes {
    struct process * data;
    int N;
};


struct processes * new_processes() {
    struct processes * p = malloc(sizeof(struct processes));
    p->data = NULL;
    p->N = 0;
    return p;
}

void delete(struct processes * p) {
    if (!p) return;
    clear(p);
    free(p);
}

int add_from_file(struct processes * p, const char * filename) {
    FILE * fp = fopen(filename, "r"); if (!fp) return 0;

    char user[9], command[16];
    int pid, ppid, prio;
    float cpuu;
    long int rss, size, vsize;
    
    while (fscanf(fp, "%d %d %8s %d %f %ld %ld %ld %15s\n", &pid, &ppid, user, &prio, &cpuu, &rss, &size, &vsize, command) == 9) {
        int i = p->N++;
        p->data = realloc(p->data, (p->N * sizeof(struct process)));

        struct process pr;

        pr.pid = pid;
        pr.ppid = ppid;
        pr.prio = prio;
        pr.cpuu = cpuu;
        pr.rss = rss;
        pr.size = size;
        pr.vsize = vsize;
        strcpy(pr.user, user);
        strcpy(pr.command, command);

        p->data[i] = pr;
    }

    return 1;
}

void clear(struct processes * p) {
    if (!p) return;
    if (p->data) free(p->data);
    p->data = NULL;
    p->N = 0;
}


struct query_result {
    struct process * p_p;
    struct query_result * next;
};

int matches(struct process * p_p, struct query * q) {

    if (q->priority < 0 && p_p->prio + q->priority <= 0) return 0;
    if (q->priority > 0 && p_p->prio != q->priority) return 0;

    if (q->rss < 0 && p_p->rss + q->rss <= 0) return 0;
    if (q->rss > 0 && p_p->rss != q->rss) return 0;

    if (q->size < 0 && p_p->size + q->size <= 0) return 0;
    if (q->size > 0 && p_p->size != q->size) return 0;

    if (q->vsize < 0 && p_p->vsize + q->vsize <= 0) return 0;
    if (q->vsize > 0 && p_p->vsize != q->vsize) return 0;

    if (q->cpu_usage < 0 && p_p->cpuu + q->cpu_usage <= 0) return 0;
    if (q->cpu_usage > 0 && p_p->cpuu != q->cpu_usage) return 0;

    return 1;
}

struct query_result * search(struct processes * p, const struct query * q) {

    struct query_result * l = NULL;

    for (int i = p->N - 1; i >= 0; i--) {
        if (matches(&(p->data[i]), q)) {
            struct query_result * cur = malloc(sizeof(struct query_result));
            cur->p_p = &p->data[i];
            cur->next = l;
            l = cur;
        }
    }

    return l;
}


struct query_result * next(struct query_result * cur) {
    struct query_result * next = cur->next;
    free(cur);
    return next; 
}

void terminate_query(struct query_result * cur) {
    while(cur) {
        cur = next(cur);
    }
}

int get_pid(struct query_result *r) {
    return r->p_p->pid;
}
int get_ppid(struct query_result *r) {
    return r->p_p->ppid;
}
const char * get_user(struct query_result *r) {
    return r->p_p->user;
}
int get_priority(struct query_result *r) {
    return r->p_p->prio;
}
float get_cpu_usage(struct query_result *r) {
    return r->p_p->cpuu;
}
long int get_rss(struct query_result *r) {
    return r->p_p->rss;
}
long int get_size(struct query_result *r) {
    return r->p_p->size;
}
long int get_vsize(struct query_result *r) {
    return r->p_p->vsize;
}
const char * get_command(struct query_result *r) {
    return r->p_p->command;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "videos.h"


struct video {
    char * title;
    int id;
    int length;

    int full_views;
    int view_minutes;
};

struct platform {
    struct video * videos;
    int num_of_videos;
};

struct platform * create () {
    struct platform * p = malloc(sizeof(struct platform));
    p->videos = NULL;
    p->num_of_videos = 0;
    return p;
}

void clear (struct platform * p) {
    if (!p) return;
    for (int i = 0; i < p->num_of_videos; i++) {
        free(p->videos[i].title);
    }
    if (p->videos) free(p->videos);
    p->videos = NULL;
    p->num_of_videos = 0;
}   

void destroy (struct platform * p) {
    if (!p) return;
    clear(p);
    free(p);
}  

int add_video (struct platform * p, const char * title, int minutes) {
    if (strlen(title) > 100) return -1;
    int id = p->num_of_videos;
    p->num_of_videos++;
    p->videos = realloc(p->videos, p->num_of_videos * sizeof(struct video));
    struct video v;
    v.title = malloc(sizeof(char) * (strlen(title) + 1)); // +1 for terminating '\0'
    strcpy(v.title, title);
    v.id = id;
    v.full_views = 0;
    v.length = minutes;
    v.view_minutes = 0;
    p->videos[id] = v;
    return v.id;
}

int add_view (struct platform * p, int video_id, int minutes) {
    if (video_id < 0 || video_id >= p->num_of_videos) return 0;
    if (minutes >= p->videos[video_id].length) {
        p->videos[video_id].full_views++;
    }
    p->videos[video_id].view_minutes += minutes;
    return 1;
}

int total_view_minutes (struct platform * p) {
    int tot = 0;
    for (int i = 0; i < p->num_of_videos; i++) {
        tot += p->videos[i].view_minutes;
    }
    return tot;
}
int total_full_views (struct platform * p) {
    int tot = 0;
    for (int i = 0; i < p->num_of_videos; i++) {
        tot += p->videos[i].full_views;
    }
    return tot;
}

int video_by_title (struct platform * p, const char * title) {
    for (int i = 0; i < p->num_of_videos; i++) {
        if (strcmp(title, p->videos[i].title) == 0) {
            return i;
        }
    }
    return -1;
}

#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, const char * argv[]) {
    const char * in_fname = 0;
    const char * out_fname = 0;
    const char * key_fname = "key";
    FILE *in = stdin, *out = stdout, *key = 0, *key_counter = 0;

    for (int i = 1; i < argc; ++i) {
	if (strncmp(argv[i], "in=", 3) == 0) {
	    in_fname = argv[i] + 3;
	    continue;
	}
	if (strncmp(argv[i], "out=", 4) == 0) {
	    out_fname = argv[i] + 4;
	    continue;
	}
	if (strncmp(argv[i], "key=", 4) == 0) {
	    key_fname = argv[i] + 4;
	    continue;
	}
	fprintf(stderr, "usage: %s [in=<filename>] [out=<filename>] [key=<filename>]\n", argv[0]);
	return 1;
    }

    if (in_fname) {
	in = fopen(in_fname, "r");
	if (!in) {
	    fprintf(stderr, "could not open input file %s\n", in_fname);
	    perror("fopen");
	    goto errors;
	}
    }

    if (out_fname) {
	out = fopen(out_fname, "w");
	if (!out) {
	    fprintf(stderr, "could not open output file %s\n", out_fname);
	    perror("fopen");
	    goto errors;
	}
    }

    key = fopen(key_fname, "r");
    if (!key) {
	fprintf(stderr, "could not open key file %s\n", key_fname);
	perror("fopen");
	goto errors;
    }

    size_t key_pos = 0;
    char key_counter_fname[1024];
    snprintf(key_counter_fname, 1024, "%s.counter", key_fname);
    key_counter = fopen(key_counter_fname, "r+");
    if (!key_counter) {
	if (errno == ENOENT)
	    key_counter = fopen(key_counter_fname, "w");
	else  {
	    fprintf(stderr, "could not open key counter file %s\n", key_counter_fname);
	    perror("fopen");
	    goto errors;
	}
    } else if (fscanf(key_counter, "%zd", &key_pos) < 1) {
	fprintf(stderr, "could not read key counter from file %s\n", key_counter_fname);
	goto errors;
    }

    if (fseek(key, key_pos, SEEK_SET) != 0) {
	fprintf(stderr, "could not move to key position in file %s\n", key_counter_fname);
	goto errors;
    }

    char buf[1024];
    char key_buf[1024];

    int res;
    while ((res = fread(buf, 1, 1024, in)) > 0) {
	int key_res;
	int i = 0;
	do {
	    key_res = fread(key_buf, 1, res - i, key);
	    if (key_res < 1) {
		fprintf(stderr, "could not read enough key bytes\n");
		perror("fread:");
		goto errors;
	    }
	    for(int j = 0; j < key_res; ++j) {
		buf[i + j] ^= key_buf[j];
	    }
	    i += key_res;
	    key_pos += key_res;
	} while (i < res);
	int out_res;
	i = 0;
	do {
	    out_res = fwrite(buf + i, 1, res, out);
	    if (out_res < 1) {
		fprintf(stderr, "could not write on output file\n");
		perror("fwrite:");
		goto errors;
	    }
	    i += out_res;
	} while (i < res);
    }

    rewind(key_counter);
    fprintf(key_counter, "%zd\n", key_pos);

    if (in && in != stdin)
	fclose(in);

    if (out && out != stdout)
	fclose(out);

    if (key)
	fclose(key);
    
    if (key_counter)
	fclose(key_counter);

    return 0;

 errors:
    if (in && in != stdin)
	fclose(in);

    if (out && out != stdout)
	fclose(out);

    if (key)
	fclose(key);
    
    if (key_counter)
	fclose(key_counter);

    return 1;
}
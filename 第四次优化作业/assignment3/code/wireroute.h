#ifndef __WIREOPT_H__
#define __WIREOPT_H__

#include <omp.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

using std::vector;

using cost_t = int;

using point_t = struct {
    int _x;
    int _y;
};

using wire_t = struct {
    cost_t _cost;

    point_t _start;
    point_t _end;

    int _offset;
    enum directon_t { horizon, vertical } _type;
};

using direction_t = wire_t::directon_t;

static int _argc;
static const char **_argv;

auto get_option_string(const char *option_name, const char *default_value) -> const char * {
    for (int i = _argc - 2; i >= 0; i -= 2)
        if (strcmp(_argv[i], option_name) == 0) return _argv[i + 1];
    return default_value;
}

auto get_option_int(const char *option_name, int default_value) -> int {
    for (int i = _argc - 2; i >= 0; i -= 2)
        if (strcmp(_argv[i], option_name) == 0) return atoi(_argv[i + 1]);
    return default_value;
}

auto get_option_float(const char *option_name, float default_value) -> float {
    for (int i = _argc - 2; i >= 0; i -= 2)
        if (strcmp(_argv[i], option_name) == 0) return (float)atof(_argv[i + 1]);
    return default_value;
}

static void show_help(const char *program_path) {
    printf("Usage: %s OPTIONS\n", program_path);
    printf("\n");
    printf("OPTIONS:\n");
    printf("\t-f <input_filename> (required)\n");
    printf("\t-n <num_of_threads> (required)\n");
    printf("\t-p <SA_prob>\n");
    printf("\t-i <SA_iters>\n");
}

#endif

#include "wireroute.h"

#include <bits/utility.h>
#include <omp.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <tuple>
#include <vector>

struct cost_table_t {
    cost_t *costs;
    int dim_x;
    int dim_y;

    [[nodiscard]] inline auto at(int x, int y) const -> cost_t & { return costs[y * dim_x + x]; }
};

template <typename func_t>
auto apply_action(int x0, int y0, int x1, int y1, cost_table_t &costs, func_t action) {
    if (x0 == x1) {
        int start = std::min(y0, y1);
        int end = std::max(y0, y1);
        for (int i = start; i <= end; i++) {
            action(costs.at(x0, i));
        }
    } else {
        int start = std::min(x0, x1);
        int end = std::max(x0, x1);
        for (int i = start; i <= end; i++) {
            action(costs.at(i, y0));
        }
    }
}

template <typename func_t>
auto apply_action(wire_t &wire, cost_table_t &costs, func_t action) {
    if (wire._type == wire_t::horizon) {
        auto mid = wire._start._x + wire._offset;
        apply_action(wire._start._x, wire._start._y, mid, wire._start._y, costs, action);
        apply_action(mid, wire._start._y, mid, wire._end._y, costs, action);
        apply_action(mid, wire._end._y, wire._end._x, wire._end._y, costs, action);
    } else {
        auto mid = wire._start._y + wire._offset;
        apply_action(wire._start._x, wire._start._y, wire._start._x, mid, costs, action);
        apply_action(wire._start._x, mid, wire._end._x, mid, costs, action);
        apply_action(wire._end._x, mid, wire._end._x, wire._end._y, costs, action);
    }
}

auto calc_best_path(const wire_t &wire, cost_table_t &costs) -> wire_t {
    cost_t cost = 0;
    auto accumulate = [&cost](auto val) { cost += val; };

    wire_t best_wire = wire;
    best_wire._cost = INT32_MAX;

    auto start = std::min(wire._start._x, wire._end._x);
    auto end = std::max(wire._start._x, wire._end._x);
    for (int i = start; i < end; i++) {
        cost = 0;
        auto new_wire = wire;
        new_wire._offset = i - wire._start._x;
        new_wire._type = wire_t::horizon;
        apply_action(new_wire, costs, accumulate);
        if (cost < best_wire._cost) {
            best_wire = new_wire;
        }
    }

    start = std::min(wire._start._y, wire._end._y);
    end = std::max(wire._start._y, wire._end._y);
    for (int i = start; i < end; i++) {
        cost = 0;
        auto new_wire = wire;
        new_wire._offset = i - wire._start._y;
        new_wire._type = wire_t::vertical;
        apply_action(new_wire, costs, accumulate);
        apply_action(new_wire, costs, accumulate);
        if (cost < best_wire._cost) {
            best_wire = new_wire;
        }
    }

    return best_wire;
}

void random_wire(wire_t &wire) {
    auto direction = drand48() < 0.5 ? wire_t::horizon : wire_t::vertical;
    auto offset = static_cast<int>(
        drand48() * (direction == wire_t::horizon ? wire._end._x - wire._start._x : wire._end._y - wire._start._y));
    wire._type = direction;
    wire._offset = offset;
}

auto main(int argc, const char *argv[]) -> int {
    using namespace std::chrono;
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::duration<double> dsec;

    auto init_start = Clock::now();
    double init_time = 0;

    _argc = argc - 1;
    _argv = argv + 1;

    const char *input_filename = get_option_string("-f", "../inputs/testinput/hard_1024.txt");
    int num_of_threads = get_option_int("-n", 1);
    double SA_prob = get_option_float("-p", 0.1f);
    int SA_iters = get_option_int("-i", 5);

    int error = 0;

    if (input_filename == nullptr) {
        printf("Error: You need to specify -f.\n");
        error = 1;
    }

    if (error) {
        show_help(argv[0]);
        return 1;
    }

    printf("Number of threads: %d\n", num_of_threads);
    printf("Probability parameter for simulated annealing: %lf.\n", SA_prob);
    printf("Number of simulated annealing iterations: %d\n", SA_iters);
    printf("Input file: %s\n", input_filename);

    FILE *input = fopen(input_filename, "r");

    if (!input) {
        printf("Unable to open file: %s.\n", input_filename);
        return 1;
    }

    int dim_x, dim_y;
    int num_of_wires;

    fscanf(input, "%d %d\n", &dim_x, &dim_y);
    fscanf(input, "%d\n", &num_of_wires);

    auto *wiresa = (wire_t *)calloc(num_of_wires, sizeof(wire_t));
    auto *wiresb = (wire_t *)calloc(num_of_wires, sizeof(wire_t));
    for (int i = 0; i < num_of_wires; i++) {
        fscanf(input, "%d %d", &wiresa[i]._start._x, &wiresa[i]._start._y);
        fscanf(input, "%d %d\n", &wiresa[i]._end._x, &wiresa[i]._end._y);
    }
    memcpy(wiresb, wiresa, num_of_wires * sizeof(wire_t));
    wire_t *wires[2] = {wiresa, wiresb};

    cost_table_t tables[2] = {{(cost_t *)calloc(dim_x * dim_y, sizeof(cost_t)), dim_x, dim_y},
                              {(cost_t *)calloc(dim_x * dim_y, sizeof(cost_t)), dim_x, dim_y}};

    // init the wire and calculate cost
    for (int i = 0; i < num_of_wires; i++) {
        random_wire(wiresa[i]);
        apply_action(wiresa[i], tables[0], [](auto &val) { val++; });
    }

    init_time += duration_cast<dsec>(Clock::now() - init_start).count();
    printf("Initialization Time: %lf.\n", init_time);

    auto compute_start = Clock::now();
    double compute_time = 0;

    for (int j = 0; j < SA_iters; j++) {
        // 根据迭代的次数互换 buffers
        wire_t *wires_arrange = wires[j % 2];
        wire_t *wires_apply = wires[(j + 1) % 2];
        cost_table_t *table_arrange = tables + j % 2;
        cost_table_t *table_apply = tables + (j + 1) % 2;

        memset(table_apply->costs, 0, dim_x * dim_y * sizeof(cost_t));

        // #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < num_of_wires; i++) {
            auto wire = wires_arrange[i];
            if (drand48() > SA_prob) {
                wire = calc_best_path(wire, *table_arrange);
            } else {
                random_wire(wire);
            }
            wires_apply[i] = wire;
#pragma omp critical
            apply_action(wire, *table_apply, [](auto &val) { val++; });
        }
    }
    compute_time += duration_cast<dsec>(Clock::now() - compute_start).count();
    printf("Computation Time: %lf.\n", compute_time);

    std::string filename;
    filename = input_filename;
    auto len = filename.find_last_of(".") - filename.find_last_of('/') - 1;
    filename = filename.substr(filename.find_last_of('/') + 1, len);
    filename.append("_");
    filename.append(std::to_string(num_of_threads));
    filename.append(".txt");

    auto costs = tables[0].costs;
    std::string cost_file("cost_");
    cost_file.append(filename);
    FILE *output_cost_file = fopen(cost_file.c_str(), "w+");
    fprintf(output_cost_file, "%d %d\n", dim_x, dim_y);
    for (int i = 0; i < dim_y; i++) {
        for (int j = 0; j < dim_x; j++) {
            fprintf(output_cost_file, "%d ", costs[i * dim_x + j]);
        }
        fprintf(output_cost_file, "\n");
    }
    return 0;
}

#include "potential_function.h"
#include "potential_max_heuristic.h"
#include "potential_optimizer.h"
#include "util.h"

#include "../plugins/plugin.h"
#include "../utils/rng.h"
#include "../utils/rng_options.h"

#include <memory>
#include <vector>

using namespace std;

namespace potentials {
static void filter_dead_ends(PotentialOptimizer &optimizer, vector<State> &samples) {
    assert(!optimizer.potentials_are_bounded());
    vector<State> non_dead_end_samples;
    for (const State &sample : samples) {
        optimizer.optimize_for_state(sample);
        if (optimizer.has_optimal_solution())
            non_dead_end_samples.push_back(sample);
    }
    swap(samples, non_dead_end_samples);
}

static void optimize_for_samples(
    PotentialOptimizer &optimizer,
    int num_samples,
    utils::RandomNumberGenerator &rng) {
    vector<State> samples = sample_without_dead_end_detection(
        optimizer, num_samples, rng);
    if (!optimizer.potentials_are_bounded()) {
        filter_dead_ends(optimizer, samples);
    }
    optimizer.optimize_for_samples(samples);
}

/*
  Compute multiple potential functions that are optimized for different
  sets of samples.
*/
static vector<unique_ptr<PotentialFunction>> create_sample_based_potential_functions(
    const plugins::Options &opts) {
    vector<unique_ptr<PotentialFunction>> functions;
    PotentialOptimizer optimizer(opts);
    shared_ptr<utils::RandomNumberGenerator> rng(utils::parse_rng_from_options(opts));
    for (int i = 0; i < opts.get<int>("num_heuristics"); ++i) {
        optimize_for_samples(optimizer, opts.get<int>("num_samples"), *rng);
        functions.push_back(optimizer.get_potential_function());
    }
    return functions;
}

static shared_ptr<Heuristic> _parse(OptionParser &parser) {
    {
        parser.document_synopsis(
            "Sample-based potential heuristics",
            "Maximum over multiple potential heuristics optimized for samples. " +
            get_admissible_potentials_reference());
        parser.add_option<int>(
            "num_heuristics",
            "number of potential heuristics",
            "1",
            plugins::Bounds("0", "infinity"));
        parser.add_option<int>(
            "num_samples",
            "Number of states to sample",
            "1000",
            plugins::Bounds("0", "infinity"));
        prepare_parser_for_admissible_potentials(parser);
        utils::add_rng_options(parser);
    }
    Options opts = parser.parse();
    if (parser.dry_run())
        return nullptr;
    return make_shared<PotentialMaxHeuristic>(
        opts, create_sample_based_potential_functions(opts));
}

static Plugin<Evaluator> _plugin(
    "sample_based_potentials", _parse, "heuristics_potentials");
}

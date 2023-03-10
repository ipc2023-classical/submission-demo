#include "goal_count_heuristic.h"

#include "../plugins/plugin.h"

#include "../utils/logging.h"

#include <iostream>
using namespace std;

namespace goal_count_heuristic {
GoalCountHeuristic::GoalCountHeuristic(const plugins::Options &opts)
    : Heuristic(opts) {
    if (log.is_at_least_normal()) {
        log << "Initializing goal count heuristic..." << endl;
    }
}

int GoalCountHeuristic::compute_heuristic(const State &ancestor_state) {
    State state = convert_ancestor_state(ancestor_state);
    int unsatisfied_goal_count = 0;

    for (FactProxy goal : task_proxy.get_goals()) {
        const VariableProxy var = goal.get_variable();
        if (state[var] != goal) {
            ++unsatisfied_goal_count;
        }
    }
    return unsatisfied_goal_count;
}

static shared_ptr<Heuristic> _parse(OptionParser &parser) {
    {
        parser.document_synopsis("Goal count heuristic", "");

        Heuristic::add_options_to_parser(parser);

        parser.document_language_support("action costs", "ignored by design");
        parser.document_language_support("conditional effects", "supported");
        parser.document_language_support("axioms", "supported");

        parser.document_property("admissible", "no");
        parser.document_property("consistent", "no");
        parser.document_property("safe", "yes");
        parser.document_property("preferred operators", "no");
    }
    Options opts = parser.parse();
    if (parser.dry_run())
        return nullptr;
    else
        return make_shared<GoalCountHeuristic>(opts);
}


static Plugin<Evaluator> _plugin("goalcount", _parse);
}

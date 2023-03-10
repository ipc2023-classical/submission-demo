#include "merge_strategy_factory.h"

#include "../plugins/plugin.h"

#include <iostream>

using namespace std;

namespace merge_and_shrink {
MergeStrategyFactory::MergeStrategyFactory(const plugins::Options &options)
    : log(utils::get_log_from_options(options)) {
}

void MergeStrategyFactory::dump_options() const {
    if (log.is_at_least_normal()) {
        log << "Merge strategy options:" << endl;
        log << "Type: " << name() << endl;
        dump_strategy_specific_options();
    }
}

void add_merge_strategy_options_to_parser(plugins::OptionParser &parser) {
    utils::add_log_options_to_parser(parser);
}

static plugins::PluginTypePlugin<MergeStrategyFactory> _type_plugin(
    "MergeStrategy",
    "This page describes the various merge strategies supported "
    "by the planner.");
}

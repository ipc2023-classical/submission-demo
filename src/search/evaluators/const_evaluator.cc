#include "const_evaluator.h"

#include "../plugins/plugin.h"

using namespace std;

namespace const_evaluator {
ConstEvaluator::ConstEvaluator(const plugins::Options &opts)
    : Evaluator(opts), value(opts.get<int>("value")) {
}

EvaluationResult ConstEvaluator::compute_result(EvaluationContext &) {
    EvaluationResult result;
    result.set_evaluator_value(value);
    return result;
}

static shared_ptr<Evaluator> _parse(OptionParser &parser) {
    {
        parser.document_synopsis(
            "Constant evaluator",
            "Returns a constant value.");

        parser.add_option<int>(
            "value",
            "the constant value",
            "1",
            plugins::Bounds("0", "infinity"));
        add_evaluator_options_to_parser(parser);
    }
    Options opts = parser.parse();
    if (parser.dry_run())
        return nullptr;
    else
        return make_shared<ConstEvaluator>(opts);
}

static Plugin<Evaluator> _plugin("const", _parse, "evaluators_basic");
}

#include "lazy_search.h"
#include "search_common.h"

#include "../plugins/plugin.h"

using namespace std;

namespace plugin_lazy_greedy {
static const string DEFAULT_LAZY_BOOST = "1000";

static shared_ptr<SearchEngine> _parse(OptionParser &parser) {
    {
        parser.document_synopsis("Greedy search (lazy)", "");

        parser.add_list_option<shared_ptr<Evaluator>>(
            "evals",
            "evaluators");
        parser.add_list_option<shared_ptr<Evaluator>>(
            "preferred",
            "use preferred operators of these evaluators",
            "[]");
        parser.add_option<bool>(
            "reopen_closed",
            "reopen closed nodes",
            "false");
        parser.add_option<int>(
            "boost",
            "boost value for alternation queues that are restricted "
            "to preferred operator nodes",
            DEFAULT_LAZY_BOOST);
        SearchEngine::add_succ_order_options(parser);
        SearchEngine::add_options_to_parser(parser);

        parser.document_note(
            "Open lists",
            "In most cases, lazy greedy best first search uses "
            "an alternation open list with one queue for each evaluator. "
            "If preferred operator evaluators are used, it adds an "
            "extra queue for each of these evaluators that includes "
            "only the nodes that are generated with a preferred operator. "
            "If only one evaluator and no preferred operator evaluator is used, "
            "the search does not use an alternation open list "
            "but a standard open list with only one queue.");
        parser.document_note(
            "Equivalent statements using general lazy search",
            "\n```\n--evaluator h2=eval2\n"
            "--search lazy_greedy([eval1, h2], preferred=h2, boost=100)\n```\n"
            "is equivalent to\n"
            "```\n--evaluator h1=eval1 --heuristic h2=eval2\n"
            "--search lazy(alt([single(h1), single(h1, pref_only=true), single(h2),\n"
            "                  single(h2, pref_only=true)], boost=100),\n"
            "              preferred=h2)\n```\n"
            "------------------------------------------------------------\n"
            "```\n--search lazy_greedy([eval1, eval2], boost=100)\n```\n"
            "is equivalent to\n"
            "```\n--search lazy(alt([single(eval1), single(eval2)], boost=100))\n```\n"
            "------------------------------------------------------------\n"
            "```\n--evaluator h1=eval1\n--search lazy_greedy(h1, preferred=h1)\n```\n"
            "is equivalent to\n"
            "```\n--evaluator h1=eval1\n"
            "--search lazy(alt([single(h1), single(h1, pref_only=true)], boost=1000),\n"
            "              preferred=h1)\n```\n"
            "------------------------------------------------------------\n"
            "```\n--search lazy_greedy(eval1)\n```\n"
            "is equivalent to\n"
            "```\n--search lazy(single(eval1))\n```\n",
            true);
    }
    Options opts = parser.parse();

    shared_ptr<lazy_search::LazySearch> engine;
    if (!parser.dry_run()) {
        opts.set("open", search_common::create_greedy_open_list_factory(opts));
        engine = make_shared<lazy_search::LazySearch>(opts);
        // TODO: The following two lines look fishy. See similar comment in _parse.
        vector<shared_ptr<Evaluator>> preferred_list = opts.get_list<shared_ptr<Evaluator>>("preferred");
        engine->set_preferred_operator_evaluators(preferred_list);
    }
    return engine;
}

static Plugin<SearchEngine> _plugin("lazy_greedy", _parse);
}

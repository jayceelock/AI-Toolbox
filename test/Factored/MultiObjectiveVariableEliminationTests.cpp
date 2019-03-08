#define BOOST_TEST_MODULE Factored_Bandit_MultiObjectiveVariableElimination
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <AIToolbox/Factored/Bandit/Algorithms/Utils/MultiObjectiveVariableElimination.hpp>
#include <AIToolbox/Utils/Core.hpp>
#include <AIToolbox/Utils/Prune.hpp>

namespace aif = AIToolbox::Factored;
namespace fb = AIToolbox::Factored::Bandit;
using MOVE = fb::MultiObjectiveVariableElimination;

BOOST_AUTO_TEST_CASE( simple_graph ) {
    const std::vector<fb::MOQFunctionRule> rules {
        // Actions,                     Value
        {  {{0}, {0}},            (aif::Rewards(2) << 4.0, 0.0).finished()},
        {  {{1}, {0}},            (aif::Rewards(2) << 5.0, 1.0).finished()},
        {  {{1}, {1}},            (aif::Rewards(2) << 2.0, 2.0).finished()},
        {  {{0, 1}, {1, 0}},      (aif::Rewards(2) << 2.0, 3.0).finished()},
    };

    MOVE::Results solutions{std::make_tuple(aif::PartialAction{{0, 1}, {0, 0}}, (aif::Rewards(2) << 9.0, 1.0).finished()),
                         // std::make_tuple(aif::PartialAction{{0, 1}, {0, 1}}, (aif::Rewards(2) << 6.0, 2.0).finished()),
                            std::make_tuple(aif::PartialAction{{0, 1}, {1, 0}}, (aif::Rewards(2) << 7.0, 4.0).finished())};
                         // std::make_tuple(aif::PartialAction{{0, 1}, {1, 1}}, (aif::Rewards(2) << 2.0, 2.0).finished())

    auto comparer = [](const auto & lhs, const auto & rhs) {
        return AIToolbox::veccmp(std::get<0>(lhs).second, std::get<0>(rhs).second) < 0;
    };

    const aif::Action a{2, 2};

    MOVE v;
    auto bestActions = v(a, rules);

    BOOST_CHECK_EQUAL(solutions.size(), bestActions.size());

    std::sort(std::begin(solutions), std::end(solutions), comparer);
    std::sort(std::begin(bestActions), std::end(bestActions), comparer);

    for (size_t i = 0; i < solutions.size(); ++i) {
        const auto & spa1 = std::get<0>(solutions[i]).first;
        const auto & spa2 = std::get<0>(solutions[i]).second;

        const auto & pa1 = std::get<0>(bestActions[i]).first;
        const auto & pa2 = std::get<0>(bestActions[i]).second;

        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(spa1), std::end(spa1), std::begin(pa1), std::end(pa1));
        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(spa2), std::end(spa2), std::begin(pa2), std::end(pa2));

        BOOST_CHECK_EQUAL(std::get<1>(solutions[i]), std::get<1>(bestActions[i]));
    }
}

BOOST_AUTO_TEST_CASE( simple_graph_2 ) {
    const std::vector<fb::MOQFunctionRule> rules {
        // Actions,                     Value
        {  {{0}, {0}},            (aif::Rewards(2) << 4.0, 0.0).finished()},
        {  {{0, 1}, {1, 0}},      (aif::Rewards(2) << 2.0, 3.0).finished()},
    };

    MOVE::Results solutions{std::make_tuple(aif::PartialAction{{0}, {0}},       (aif::Rewards(2) << 4.0, 0.0).finished()),
                            std::make_tuple(aif::PartialAction{{0, 1}, {1, 0}}, (aif::Rewards(2) << 2.0, 3.0).finished())};

    auto comparer = [](const auto & lhs, const auto & rhs) {
        return AIToolbox::veccmp(std::get<0>(lhs).second, std::get<0>(rhs).second) < 0;
    };

    const aif::Action a{2, 2};

    MOVE v;
    auto bestActions = v(a, rules);

    BOOST_REQUIRE(solutions.size() == bestActions.size());

    std::sort(std::begin(solutions), std::end(solutions), comparer);
    std::sort(std::begin(bestActions), std::end(bestActions), comparer);

    for (size_t i = 0; i < solutions.size(); ++i) {
        const auto & spa1 = std::get<0>(solutions[i]).first;
        const auto & spa2 = std::get<0>(solutions[i]).second;

        const auto & pa1 = std::get<0>(bestActions[i]).first;
        const auto & pa2 = std::get<0>(bestActions[i]).second;

        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(spa1), std::end(spa1), std::begin(pa1), std::end(pa1));
        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(spa2), std::end(spa2), std::begin(pa2), std::end(pa2));

        BOOST_CHECK_EQUAL(std::get<1>(solutions[i]), std::get<1>(bestActions[i]));
    }
}

BOOST_AUTO_TEST_CASE( radu_marinescu_graph ) {
    std::vector<fb::MOQFunctionRule> rules;

    const aif::Action A{2, 2, 2, 2, 2};

    // g rules for single actions
    for (size_t a = 0; a < A.size(); ++a) {
        rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{a}, {0}}, (aif::Rewards(2) << 0.0, 0.0).finished()});
        rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{a}, {1}}, (aif::Rewards(2) << 0.0, -(a + 1.0)).finished()});
    }

    // f1 rules
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 2}, {0, 0, 0}}, (aif::Rewards(2) << -5.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 2}, {0, 0, 1}}, (aif::Rewards(2) << -2.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 2}, {0, 1, 0}}, (aif::Rewards(2) << -3.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 2}, {0, 1, 1}}, (aif::Rewards(2) << -2.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 2}, {1, 0, 0}}, (aif::Rewards(2) << -2.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 2}, {1, 0, 1}}, (aif::Rewards(2) << -3.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 2}, {1, 1, 0}}, (aif::Rewards(2) << -0.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 2}, {1, 1, 1}}, (aif::Rewards(2) << -2.0, 0.0).finished()});

    // f2 rules
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 3}, {0, 0, 0}}, (aif::Rewards(2) << -1.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 3}, {0, 0, 1}}, (aif::Rewards(2) << -4.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 3}, {0, 1, 0}}, (aif::Rewards(2) << -0.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 3}, {0, 1, 1}}, (aif::Rewards(2) << -2.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 3}, {1, 0, 0}}, (aif::Rewards(2) << -6.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 3}, {1, 0, 1}}, (aif::Rewards(2) << -5.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 3}, {1, 1, 0}}, (aif::Rewards(2) << -6.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{0, 1, 3}, {1, 1, 1}}, (aif::Rewards(2) << -5.0, 0.0).finished()});

    // f3 rules
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{1, 3, 4}, {0, 0, 0}}, (aif::Rewards(2) << -1.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{1, 3, 4}, {0, 0, 1}}, (aif::Rewards(2) << -3.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{1, 3, 4}, {0, 1, 0}}, (aif::Rewards(2) << -5.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{1, 3, 4}, {0, 1, 1}}, (aif::Rewards(2) << -4.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{1, 3, 4}, {1, 0, 0}}, (aif::Rewards(2) << -1.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{1, 3, 4}, {1, 0, 1}}, (aif::Rewards(2) << -3.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{1, 3, 4}, {1, 1, 0}}, (aif::Rewards(2) << -5.0, 0.0).finished()});
    rules.emplace_back(fb::MOQFunctionRule{aif::PartialAction{{1, 3, 4}, {1, 1, 1}}, (aif::Rewards(2) << -4.0, 0.0).finished()});

    MOVE::Results solutions{std::make_tuple(aif::PartialAction{{0, 1, 2, 3, 4}, {0, 0, 0, 0, 0}}, (aif::Rewards(2) << -7.0,  0.0).finished()),
                            std::make_tuple(aif::PartialAction{{0, 1, 2, 3, 4}, {0, 1, 1, 0, 0}}, (aif::Rewards(2) << -3.0, -5.0).finished()),
                            std::make_tuple(aif::PartialAction{{0, 1, 2, 3, 4}, {0, 1, 0, 0, 0}}, (aif::Rewards(2) << -4.0, -2.0).finished())};

    auto comparer = [](const auto & lhs, const auto & rhs) {
        return AIToolbox::veccmp(std::get<0>(lhs).second, std::get<0>(rhs).second) < 0;
    };

    MOVE v;
    auto bestActions = v(A, rules);

    BOOST_REQUIRE(solutions.size() == bestActions.size());

    std::sort(std::begin(solutions), std::end(solutions), comparer);
    std::sort(std::begin(bestActions), std::end(bestActions), comparer);

    for (size_t i = 0; i < solutions.size(); ++i) {
        const auto & spa1 = std::get<0>(solutions[i]).first;
        const auto & spa2 = std::get<0>(solutions[i]).second;

        const auto & pa1 = std::get<0>(bestActions[i]).first;
        const auto & pa2 = std::get<0>(bestActions[i]).second;

        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(spa1), std::end(spa1), std::begin(pa1), std::end(pa1));
        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(spa2), std::end(spa2), std::begin(pa2), std::end(pa2));

        BOOST_CHECK_EQUAL(std::get<1>(solutions[i]), std::get<1>(bestActions[i]));
    }
}

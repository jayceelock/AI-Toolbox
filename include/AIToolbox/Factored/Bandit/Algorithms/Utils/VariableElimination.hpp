#ifndef AI_TOOLBOX_FACTORED_BANDIT_VARIABLE_ELIMINATION_HEADER_FILE
#define AI_TOOLBOX_FACTORED_BANDIT_VARIABLE_ELIMINATION_HEADER_FILE

#include "AIToolbox/Factored/Bandit/Types.hpp"
#include "AIToolbox/Factored/Utils/FactorGraph.hpp"

namespace AIToolbox::Factored::Bandit {
    /**
     * @brief This class represents the Variable Elimination process.
     *
     * This class performs variable elimination on a factor graph. It first
     * builds the graph starting from a list of QFunctionRules. These rules
     * are sorted by the agents they affect, and each group is added to a
     * single factor connected to those agents.
     *
     * Each agent is then eliminated from the graph, and all rules
     * connected to it are processed in order to find out which action the
     * agent being eliminated should take.
     *
     * When all agents have been eliminated, only the optimal rules
     * containing the best actions remain. The ones that provide the best
     * reward are joined into a single Action, which is then returned.
     *
     * This process is exponential in the maximum number of agents found
     * attached to the same factor (which could be higher than in the
     * original graph, as the elimination process can create bigger factors
     * than the initial ones). However, given that each factor is usually
     * linked to few agents, and that this process allows avoiding
     * considering the full factored Action at any one time, it is usually
     * much faster than a brute-force approach.
     *
     * WARNING: This process only considers rules that have been explicitly
     * passed to it. This may create problems if some of your values have
     * negative values in it, since the elimination process will not
     * consider unmentioned actions as giving 0 reward, and choose them
     * instead of the negative values. In order to avoid this problem
     * either all 0 rules have to be explicitly mentioned for each agent
     * subgroup containing negative rules, or the rules have to be
     * converted to an equivalent graph with positive values.
     */
    class VariableElimination {
        public:
            // action of subset of agents, tags of processed actions, value of rule
            using Entry = std::pair<double, PartialAction>;

            using Rule = std::pair<PartialValues, Entry>;
            using Rules = std::vector<Rule>;

            using Result = std::tuple<Action, double>;

            struct Factor {
                Rules rules;
            };

            using Graph = FactorGraph<Factor>;

            /**
             * @brief Basic constructor.
             *
             * This constructor initializes the internal graph with the
             * number of needed agents.
             *
             * @param a The action space.
             */
            VariableElimination(Action a);

            /**
             * @brief This function finds the best Action-value pair for the provided QFunctionRules.
             *
             * @param rules An iterable object over QFunctionRules.
             *
             * @return A tuple containing the best Action and its value over the input rules.
             */
            template <typename Iterable>
            Result operator()(const Iterable & inputRules) {
                // Should we reset the graph?
                for (const auto & rule : inputRules) {
                    auto it = graph_.getFactor(rule.action.first);
                    it->getData().rules.emplace_back(rule.action.second, Entry{rule.value, PartialAction()});
                }
                return start();
            }

        private:
            /**
             * @brief This function performs the actual agent elimination process.
             *
             * For each agent, its adjacent factors, and the agents
             * adjacent to those are found. Then all possible action
             * combinations between those other agents are tried in order
             * to find the best action response be for the agent to be
             * eliminated.
             *
             * All the best responses found are added as Rules to a
             * (possibly new) factor adjacent to the adjacent agents.
             *
             * The process is repeated until all agents are eliminated.
             *
             * What remains is then joined into a single Action, containing
             * the best possible value.
             *
             * @return The pair for best Action and its value given the internal graph.
             */
            Result start();

            /**
             * @brief This function performs the elimination of a single agent (and all factors next to it) from the internal graph.
             *
             * This function adds the resulting best rules which do not
             * depend on the eliminated action to the remaining factors.
             *
             * \sa start()
             *
             * @param agent The index of the agent to be removed from the graph.
             */
            void removeAgent(size_t agent);

            Action A;
            Graph graph_;
            std::vector<Entry> finalFactors_;
    };
}

#endif

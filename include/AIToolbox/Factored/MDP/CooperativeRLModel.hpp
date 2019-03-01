#ifndef AI_TOOLBOX_FACTORED_MDP_COOPERATIVE_RLMODEL_HEADER_FILE
#define AI_TOOLBOX_FACTORED_MDP_COOPERATIVE_RLMODEL_HEADER_FILE

#include <AIToolbox/Factored/MDP/CooperativeExperience.hpp>
#include <AIToolbox/Factored/Utils/BayesianNetwork.hpp>

namespace AIToolbox::Factored::MDP {
    class CooperativeRLModel {
        public:
            using TransitionMatrix   = FactoredDDN;
            // Same shape as the DDN, without tags and with the last dimension
            // summed over (matrix->vector).
            using RewardMatrix       = std::vector<std::vector<Vector>>;

            /**
             * @brief Constructor using previous Experience.
             *
             * This constructor stores a reference to the CooperativeExperience
             * that will be used to learn an MDP Model from the data, and
             * initializes internal Model data.
             *
             * The user can choose whether he wants to directly sync the
             * CooperativeRLModel to the underlying CooperativeExperience, or
             * delay it for later.
             *
             * In the latter case the default transition function defines a
             * transition of probability 1 for each state factor to 0, no
             * matter the action or its parents.
             *
             * In general it would be better to add some amount of bias to the
             * CooperativeExperience so that when a new state-action pair is
             * tried, the CooperativeRLModel doesn't automatically compute 100%
             * probability of transitioning to the resulting state, but smooths
             * into it. This may depend on your problem though.
             *
             * The default reward function is 0.
             *
             * @param exp The CooperativeExperience of the model.
             * @param discount The discount used in solving methods.
             * @param sync Whether to sync with the CooperativeExperience immediately or delay it.
             */
            CooperativeRLModel(const CooperativeExperience & exp, double discount = 1.0, bool sync = false);

            /**
             * @brief This function syncs the whole CooperativeRLModel to the underlying CooperativeExperience.
             *
             * Since use cases in AI are very varied, one may not want to
             * update its CooperativeRLModel for each single transition
             * experienced by the agent. To avoid this we leave to the user the
             * task of syncing between the underlying CooperativeExperience and
             * the CooperativeRLModel, as he/she sees fit.
             *
             * After this function is run the transition and reward functions
             * will accurately reflect the state of the underlying
             * CooperativeExperience.
             */
            void sync();

            /**
             * @brief This function syncs a state-action pair to the underlying CooperativeExperience.
             *
             * @param s The state that needs to be synced.
             * @param a The action that needs to be synced.
             */
            void sync(const State & s, const Action & a);

            /**
             * @brief This function syncs the given indeces to the underlying CooperativeExperience.
             *
             * This function is equivalent to sync(const State &, const Action
             * &), but it avoids recomputing the indeces of the state-action
             * pair. Instead, it uses the ones already computed by the
             * underlying CooperativeExperience during its record() call.
             *
             * This works because the CooperativeExperience and
             * CooperativeRLModel use the same factoring of their data
             * structures, and thus the indeces can be used unchanged in both
             * classes.
             *
             * @param indeces The indeces provided by the CooperativeExperience.
             */
            void sync(const CooperativeExperience::Indeces & indeces);

            /**
             * @brief This function samples the MDP for the specified state action pair.
             *
             * This function samples the model for simulate experience. The transition
             * and reward functions are used to produce, from the state action pair
             * inserted as arguments, a possible new state with respective reward.
             * The new state is picked from all possible states that the MDP allows
             * transitioning to, each with probability equal to the same probability
             * of the transition in the model. After a new state is picked, the reward
             * is the corresponding reward contained in the reward function.
             *
             * @param s The state that needs to be sampled.
             * @param a The action that needs to be sampled.
             *
             * @return A tuple containing a new state and a reward.
             */
            std::tuple<State, double> sampleSR(const State & s, const Action & a) const;

            /**
             * @brief This function samples the MDP with the specified state action pair.
             *
             * This function is equivalent to sampleSR(const State &, const Action &).
             *
             * The only difference is that it allows to output the new State
             * into a pre-allocated State, avoiding the need for an allocation
             * at every sample.
             *
             * NO CHECKS for nullptr are done.
             *
             * @param s The state that needs to be sampled.
             * @param a The action that needs to be sampled.
             * @param s1 The new state.
             *
             * @return The reward for the sampled transition.
             */
            double sampleSR(const State & s, const Action & a, State * s1) const;

            /**
             * @brief This function returns the stored transition probability for the specified transition.
             *
             * @param s The initial state of the transition.
             * @param a The action performed in the transition.
             * @param s1 The final state of the transition.
             *
             * @return The probability of the specified transition.
             */
            double getTransitionProbability(const State & s, const Action & a, const State & s1) const;

            /**
             * @brief This function returns the stored expected reward for the specified transition.
             *
             * @param s The initial state of the transition.
             * @param a The action performed in the transition.
             * @param s1 The final state of the transition.
             *
             * @return The expected reward of the specified transition.
             */
            double getExpectedReward(const State & s, const Action & a, const State & s1) const;

            /**
             * @brief This function returns the number of states of the world.
             *
             * @return The total number of states.
             */
            const State & getS() const;

            /**
             * @brief This function returns the number of available actions to the agent.
             *
             * @return The total number of actions.
             */
            const Action & getA() const;

            /**
             * @brief This function sets a new discount factor for the Model.
             *
             * @param d The new discount factor for the Model.
             */
            void setDiscount(double d);

            /**
             * @brief This function returns the currently set discount factor.
             *
             * @return The currently set discount factor.
             */
            double getDiscount() const;

            /**
             * @brief This function enables inspection of the underlying Experience of the RLModel.
             *
             * @return The underlying Experience of the RLModel.
             */
            const CooperativeExperience & getExperience() const;

            /**
             * @brief This function returns the transition matrix for inspection.
             *
             * @return The transition matrix.
             */
            const TransitionMatrix & getTransitionFunction() const;

            /**
             * @brief This function returns the rewards matrix for inspection.
             *
             * @return The rewards matrix.
             */
            const RewardMatrix & getRewardFunction()     const;

        private:
            const CooperativeExperience & experience_;
            double discount_;

            TransitionMatrix transitions_;
            RewardMatrix rewards_;

            mutable RandomEngine rand_;
    };
}

#endif

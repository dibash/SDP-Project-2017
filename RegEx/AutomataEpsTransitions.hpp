#pragma once
#include<forward_list>
#include<vector>
#include<queue>
#include"misc.hpp"

namespace RegEx
{
	
	/// A class to represent the epsilon transitions of the automata.
	/// Represented as a directed graph. Cycles are allowed. 
	/// Firts, the empty graph with no edges is created
	/// and later edges are added manually.
	class AutomataEpsTransitions
	{

	private:
		const size_t StatesCount; ///< Number of states in the automata.
		std::vector<StateList> deltaStar;  ///< deltaStar[q] is a list of all states reachable from q directly

	public:
		AutomataEpsTransitions() = delete;

		/// Constructs empty graph of epsilon transitions.
		/// \param n Number of states the automata has.
		AutomataEpsTransitions(size_t n)
			: StatesCount(n),
			deltaStar(n)
		{}

		/// Add epsilon transition from p to q. 
		/// \throws invalid_argument If p or q is not a valid state index.
		void addTransition(StateType p, StateType q)
		{
			if (isValidState(p) && isValidState(q))
				deltaStar[p].push_front(q);
			else throw std::invalid_argument("Invalid state index in addTransition(p, q).");
		}

		/// Get all epsilon transitions from a given state.
		/// \param p The state whose transitions list is returned.
		/// \return const reference to the forward_list that contains the transitions from p.
		/// \throws invalid_argument if p is not a valid state index.
		const StateList& delta(StateType p) const
		{
			if (isValidState(p))
				return deltaStar[p];
			else throw std::invalid_argument("Invalid state index in delta(p).");
		}

		/// Returns the number of states in the automata. 
		/// \return the value of StatesCount.
		size_t stateCount() const
		{
			return StatesCount;
		}

		/// Finds all states reachable by epsilon transitions from 
		/// any of the states in the StateList \c from. 
		/// Implemented as a MS-BFS (Multi-source Breadth First Seach) in the directed graph
		/// of epsilon transitions.
		/// \param from a StateList from which we start to mark reachable states.
		/// \return The list of reachable states
		/// 
		StateList reachableFrom(const StateList& from) const
		{
			std::vector<bool> visited(StatesCount, false);
			std::queue<size_t> toVisit;
			for (const StateType& s : from)
			{
				toVisit.push(s);
				visited[s] = true;
			}

			StateType currentState;
			while (!toVisit.empty())
			{
				currentState = toVisit.front();
				toVisit.pop();
				for (const StateType& state : deltaStar[currentState])
					if (!visited[state])
					{
						toVisit.push(state);
						visited[state] = true;
					}
			}
			
			// create the list
			StateList reachable;
			for (StateType s = 0; s < StatesCount; ++s)
				if(visited[s]) reachable.push_front(s);

			return reachable;
		}

	private:

		/// Validate a state index. 
		/// \param p The state to check.
		/// \return True if p is in [0, StatesCount)
		bool isValidState(StateType p) const
		{
			if (p >= StatesCount) return false;
			else return true;
		}

	}; // class AutomataEpsTransitions

} // namespace RegEx
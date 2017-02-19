#pragma once
#include "AutomataEpsTransitions.hpp"
#include "Parser.hpp"
#include "misc.hpp"
#include "Stack.hpp"
#include <string>

namespace RegEx
{

	/// A class representing a Non-deterministic Finite Automata.
	/// Simulates an NFA, constructed using 
	/// <a href="https://en.wikipedia.org/wiki/Thompson%27s_construction">Thompson's construction </a>
	/// from a regular expression of the form: <br>
	/// <pre>
	///     <b>{valid-sym}</b> := any ASCII symbol with code from 33 to 126 (printable characters)
	///     <b>{base-sym}</b>  := any <b>{valid-sym}</b> except <b>(</b> or <b>)</b> or <b>|</b> or <b>.</b> or <b>*</b> or <b>\\</b>
	///     <b>{spec-sym}</b>  := <b>\\s</b> or <b>\\d</b> or <b>\\a</b> or <b>\\e</b> or <b>\\\\</b>
	///     <b>{symbol}</b>    := <b>{base-sym}</b> or <b>{spec-sym}</b>
	///     <b>{sym-seq}</b>   := <b>{symbol}</b> or <b>{symbol}</b>
	///     <b>{reg-expr}</b>  := <b>{sym-seq}</b>               or
	///                    (<b>{reg-expr}</b>.<b>{reg-expr}</b>) or
	///                    (<b>{reg-expr}</b>|<b>{reg-expr}</b>) or
	///                    (<b>{reg-expr}</b>)*
	/// 
	/// Note: '?' added as a wildcard symbol (matches any character)
	/// </pre>
	/// <br>
	/// The states are named 0 to N, where 0 is the begin state and N is the final state.
	/// N is also the size of the regular expression after parsing. That way each symbol
	/// from the parsed expression corresponds to a state in the NFA. For this implementation
	/// the index of the symbol in the regular expression is also the name of the corresponding state.
	/// The idea for this construction comes from my enrollment in Princeton's course Algorithms pt.2 on Coursera.
	/// The structure may be similar to the one explained in the course, but the implementation is personally mine (Dimitar Bashliev).
	class NFA
	{
	private:
		static const StateType start = 0; ///< The first state is always the begin(start) state of the NFA.

		Parser regExpr;      ///< The regular expression that the NFA was constructed from. 
		StateType accept;    ///< The final state and the number of states in the automata.
		RegEx::AutomataEpsTransitions epsTrans; ///< The epsilon transitions of the automata. 

	public:

		/// Constructs the automata object from input string. 
		/// \param str The string containing the regular expression
		/// \throws runtime_error If there is a problem with the input/runtime.
		/// \throws logic_error If there is a logical problem (probably valid input, but the algorithm doesn't handle it). 
		NFA(const std::string& str)
			: regExpr(str),
			accept(regExpr.size()),      // the last state is accepting state (N-th)
			epsTrans(regExpr.size() + 1) // [0,N] has N+1 members
		{
			Stack<StateType> opstack;

			// for each state from begin to accept
			for (StateType curr = 0; curr < accept; ++curr)
			{
				// this is the state corresponding to the previous opening bracket
				// at first each symbol is enclosed in trivial brackets,
				// thats why lastPar = curr
				StateType lastPar = curr; 

				// if the corresponding symbol is ( or | add it to stack
				if (regExpr[curr] == LEFT_P || regExpr[curr] == OR)
					opstack.push(curr);

				// if we find a closing bracket,
				if (regExpr[curr] == RIGHT_P)
				{
					if (opstack.size() == 0) throw std::runtime_error("Parantheses missmatch in RE.");
					StateType lastOp = opstack.pop();

					// if the last operation was opening bracket,
					if (regExpr[lastOp] == LEFT_P)
						// the lastPar is now closed, so
						lastPar = lastOp;

					// if the last operation was OR
					else if (regExpr[lastOp] == OR)
					{
						StateList multiOR;
						
						while(regExpr[lastOp] == OR)
						{
							multiOR.push_front(lastOp);
							if (opstack.size() == 0) 
								throw std::runtime_error("Parentheses in RE do not match.");
							lastOp = opstack.pop();

						} 
						if (regExpr[lastOp] != LEFT_P)
							throw std::runtime_error("Multi-OR statement does not match parentheses.");

						lastPar = lastOp;

						for (const StateType& orState : multiOR)
						{
							epsTrans.addTransition(lastPar, orState + 1);
							epsTrans.addTransition(orState, curr);
						}

						// we get the bracket before the or (there must be one)
						//lastPar = opstack.pop();
						//epsTrans.addTransition(lastPar, lastOp + 1);
						//epsTrans.addTransition(lastOp, curr);
					}

					// there must not be anything else on the stack!
					else throw std::logic_error("Unknown symbol in opstack in NFA constructor!");
				}

				// lookahead for iteration
				if (curr < accept - 1 && regExpr[curr + 1] == ITERATION)
				{
					epsTrans.addTransition(lastPar, curr + 1);
					epsTrans.addTransition(curr + 1, lastPar);
				}

				// add the trivial epsilon transitions
				if (regExpr[curr] == LEFT_P ||
					regExpr[curr] == ITERATION ||
					regExpr[curr] == RIGHT_P ||
					regExpr[curr] == EPSILON
					)
					epsTrans.addTransition(curr, curr + 1);
			}
			if (opstack.size() > 0)
				throw std::runtime_error("Parantheses mismatch in RE.");

		} // constructor NFA(string)

		/// Checks whether the given \c text string is in the language of the automata.
		/// \param text The string to check.
		/// \return \c true if \c text is in the language of the automata, \c false otherwise.
		bool accepts(std::string text) const
		{
			// a list of all the states that can be reached by the NFA at any point
			StateList currentSet = epsTrans.reachableFrom({ start });

			// read the text symbol by symbol
			for (const auto& currChar : text)
			{
				StateList matching;
				for (const StateType& state : currentSet)
				{
					// there are no transitions from accept state
					if (state == accept) continue;

					Symbol sym = regExpr[state];
					if ((sym == WILDCARD)                              || 
						(sym == WHITESPACE &&  isSpace(currChar))      ||
						(sym == DIGIT      &&  isDigit(currChar))      ||
						(sym == ALPHA      &&  isAlpha(currChar))      ||
						(sym == DOT        &&  currChar == '.')        ||
						(sym == ASTERIX    &&  currChar == '*')        ||
						(sym == VERT_LINE  &&  currChar == '|')        ||
						(sym == BRACKET_LT &&  currChar == '(')        ||
						(sym == BRACKET_RT &&  currChar == ')')        ||
						(sym == QUESTIONM  &&  currChar == '?')        ||
						(sym == toLower(currChar) && !isMeta(currChar))
						)
						// the states that are pointed to by the recognized character are a match
						matching.push_front(state + 1);
				}

				// the new set after epsilon transitions:
				currentSet = epsTrans.reachableFrom(matching);



				// if no matches, we dont need to look at the rest of the text
				if (currentSet.empty())
					return false;
			}

			// if accept is in the current set after we read the whole text
			// then the NFA accepts the string
			for(const StateType& check : currentSet)
				if(check == accept) 
					return true;

			// otherwise, not
			return false;



		}


	}; // class NFA

} // namespace RegEx

using RegEx::NFA;
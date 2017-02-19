#pragma once
#include<string>
#include<vector>
#include"misc.hpp"

namespace RegEx
{
	/// Class to represent the regular expression after parsing.
	class Parser
	{
		std::vector<Symbol> regEx; ///< the parsed expression formated for NFA.

	public:
		/// Formats the inputed regular expression.
		/// Removes the '.' symbol, because concatenation will treated as implicit operation
		/// by the NFA. Replaces the special symbols with with single character that is not
		/// allowed in the expression otherwise. 
		/// \param inputExpr the regular expression to parse. 
		/// \throws runtime_error if there are unsupported symbols.
		Parser(const std::string& inputExpr)
		{
			regEx.reserve(inputExpr.size());
			for (unsigned i = 0; i < inputExpr.size(); ++i)
			{
				if (inputExpr[i] == BACKSLASH)
				{
					++i; // go to next symbol after backslash
					switch (toLower(inputExpr[i]))
					{
					case '\\'   : regEx.push_back(BACKSLASH);  break;
					case 's'    : regEx.push_back(WHITESPACE); break;
					case 'd'    : regEx.push_back(DIGIT);      break;
					case 'a'    : regEx.push_back(ALPHA);      break;
					case 'e'    : regEx.push_back(EPSILON);    break;
					case '.'    : regEx.push_back(DOT);        break;
					case '*'    : regEx.push_back(ASTERIX);    break;
					case '|'    : regEx.push_back(VERT_LINE);  break;
					case '('    : regEx.push_back(BRACKET_LT); break;
					case ')'    : regEx.push_back(BRACKET_RT); break;
					case '?'    : regEx.push_back(QUESTIONM);  break;


					default: throw std::runtime_error("Unsupported character in regex.");
					}
				}
				else if (inputExpr[i] == CONCAT) 
				{ 
					/* Do nothing, because we treat concatenation as implicit */
				}
				else if (isSpace(inputExpr[i]))
				{
					/* Ignore spaces in the regular expression */
				}
				else if (inputExpr[i] >= 33 && inputExpr[i] <= 126)
				{
					regEx.push_back(toLower(inputExpr[i]));
				}
				else // if character is not printable
				{
					throw std::runtime_error("Unsupported character in regex.");
				}
			}
			regEx.shrink_to_fit();
		}

		/// Access element at the \c pos position. 
		/// \param pos the position of the element to access.
		/// \throws out_of_range if \c pos is not a valid index.
		const Symbol& operator[](int pos) const
		{
			return regEx[pos];
		}

		/// Get the number of symbols in the regular expression.
		size_t size() const
		{
			return regEx.size();
		}

	}; // class Parser
} // namespace RegEx
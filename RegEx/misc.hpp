#pragma once

namespace RegEx
{
	typedef size_t StateType;
	typedef std::forward_list<StateType> StateList;

	typedef uint16_t Symbol;
	const Symbol WHITESPACE = 0xFF +  1; // \s 
	const Symbol DIGIT =      0xFF +  2; // \d 
	const Symbol ALPHA =      0xFF +  3; // \a 
	const Symbol EPSILON =    0xFF +  4; // \e 
	const Symbol DOT =        0xFF +  5; // \.
	const Symbol ASTERIX =    0xFF +  6; // \*
	const Symbol VERT_LINE =  0xFF +  7; // \|
	const Symbol BRACKET_LT = 0xFF +  8; // \(
	const Symbol BRACKET_RT = 0xFF +  9; // \)
	const Symbol QUESTIONM =  0xFF + 10; // \?
	
#ifndef USE_IMPLICIT_CONCAT_ONLY
	const Symbol CONCAT    = '.';
#else 
	const Symbol CONCAT     = ' ';
#endif
	const Symbol BACKSLASH = '\\';
	const Symbol OR        = '|';
	const Symbol ITERATION = '*';
	const Symbol LEFT_P    = '(';
	const Symbol RIGHT_P   = ')';
	const Symbol WILDCARD  = '?';

	

	inline bool isMeta (char c) { return c == OR || c == ITERATION || c == LEFT_P || c == RIGHT_P || c == WILDCARD; }
	inline bool isSpace(char c) { return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v'; }
	inline bool isAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
	inline bool isDigit(char c) { return c >= '0' && c <= '9'; }
	inline char toLower(char c) { return isAlpha(c) ? (c | (1 << 5)) : c; }

}
/* 
 *  Filename:    PasswordStrength.h
 *  Description: CPasswordStrength class implementation
 *               (adopted from http://www.passwordmeter.com/)
 *  Copyright:   Julijan Šribar, 2012
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the author(s) be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#pragma once

#include <atlstr.h>
#include <string>
#include <set>
#include <algorithm>
#include <iterator>
#include <math.h>

class CPasswordStrength
{
public:
	CPasswordStrength() { }

	int GetScore(LPCTSTR password)
	{
		m_password = password;

		int score = DoAdditions();
		score -= DoDeductions();

		if (score > 100)
			return 100;
		if (score < 0)
			return 0;
		return score;
	}

private:
	typedef std::basic_string<TCHAR> _tstring;
	_tstring m_password;

	static const int MinimumPasswordLength = 4;
	static const int SatisfactoryPasswordLength = 8;
	static const int PasswordLengthMultiplier = 4;
	static const int SequentialCharNumThreshold = 2;
	static const int SequentialCharNumMultiplier = 3;
	static const int SameCharCaseMultiplier = 2;
	static const int NumberOfDigitsMultiplier = 4;
	static const int NumberOfSymbolsMultiplier = 6;
	static const int NumberOfDigitsOrSymbolsInMiddleMultiplier = 2;
	static const int NumberOfRequirenmentsThreshold = 3;
	static const int NumberOfRequirenmentsMultiplier = 2;
	static const int ConsecutiveCharTypeMultiplier = 2;

	int DoAdditions()
	{
		int score = m_password.size() * PasswordLengthMultiplier;
		score += GetNumberOfUpperCaseScore();
		score += GetNumberOfLowerCaseScore();
		score += GetNumberOfDigitsScore();
		score += GetNumberOfSymbolsScore();
		score += GetNumberOfDigitsOrSymbolsInTheMiddleScore();
		score += GetNumberOfRequirementsScore();
		return score;
	}

	int DoDeductions()
	{
		int score = 0;
		score += GetLettersOnlyScore();
		score += GetDigitsOnlyScore();
		score += GetRepeatedCharsScore();
		score += GetNumberOfConsecutiveUpperCaseScore();
		score += GetNumberOfConsecutiveLowerCaseScore();
		score += GetNumberOfConsecutiveDigitsScore();
		score += GetSequentialCharactersScore();
		score += GetSequentialDigitsScore();
		return score;
	}
	
	int NumberOfUpperCaseCharacters()
	{
		return count_if(m_password.begin(), m_password.end(), ::_istupper);
	}

	int NumberOfLowerCaseCharacters()
	{
		return count_if(m_password.begin(), m_password.end(), ::_istlower);
	}

	int NumberOfDigits()
	{
		return count_if(m_password.begin(), m_password.end(), ::_istdigit);
	}

	int NumberOfSymbols()
	{
		return count_if(m_password.begin(), m_password.end(), IsSymbol);
	}

	int NumberOfDigitsOrSymbolsInTheMiddle()
	{
		if (m_password.size() < 2)
			return 0;
		return count_if(++m_password.begin(), --m_password.end(), IsDigitOrSymbol);
	}

	bool IsAllLetters()
	{
		if (m_password.size() == 0)
			return false;
		return m_password.size() == count_if(m_password.begin(), m_password.end(), ::_istalpha);
	}

	bool IsAllDigits()
	{
		if (m_password.size() == 0)
			return false;
		return m_password.size() == count_if(m_password.begin(), m_password.end(), ::_istdigit);
	}

	int NumberOfUniqueCharacters()
	{
		std::set<TCHAR> uniqueCharacters;
		std::for_each(m_password.begin(), m_password.end(), [&uniqueCharacters](TCHAR ch) { uniqueCharacters.insert(ch); });
		return uniqueCharacters.size();
	}

	static const int ConsecutiveCharTypeThreshold = 2;

	int NumberOfConsecutiveUpperCase()
	{
		size_t len = m_password.size();
		if (len < ConsecutiveCharTypeThreshold)
			return 0;
		int nCount = 0;
		for (_tstring::iterator found = m_password.begin(); (found = std::adjacent_find(found, m_password.end(), [](TCHAR ch1, TCHAR ch2) {return ::_istupper(ch1) && ::_istupper(ch2);})) != m_password.end(); ++found)
			++nCount;
		return nCount;
	}

	int NumberOfConsecutiveLowerCase()
	{
		size_t len = m_password.size();
		if (len < ConsecutiveCharTypeThreshold)
			return 0;
		int nCount = 0;
		for (_tstring::iterator found = m_password.begin(); (found = std::adjacent_find(found, m_password.end(), [](TCHAR ch1, TCHAR ch2) {return ::_istlower(ch1) && ::_istlower(ch2);})) != m_password.end(); ++found)
			++nCount;
		return nCount;
	}

	int NumberOfConsecutiveDigits()
	{
		size_t len = m_password.size();
		if (len < ConsecutiveCharTypeThreshold)
			return 0;
		int nCount = 0;
		for (_tstring::iterator found = m_password.begin(); (found = std::adjacent_find(found, m_password.end(), [](TCHAR ch1, TCHAR ch2) {return ::_istdigit(ch1) && ::_istdigit(ch2);})) != m_password.end(); ++found)
			++nCount;
		return nCount;
	}

	// Score addition methods

	int GetNumberOfUpperCaseScore()
	{
		int nuc = NumberOfUpperCaseCharacters();
		if (nuc == 0)
			return 0;
		return (m_password.size() - nuc) * SameCharCaseMultiplier;
	}

	int GetNumberOfLowerCaseScore()
	{
		int nlc = NumberOfLowerCaseCharacters();
		if (nlc == 0)
			return 0;
		return (m_password.size() - nlc) * SameCharCaseMultiplier;
	}

	int GetNumberOfDigitsScore()
	{
		size_t nd = NumberOfDigits();
		if (nd == m_password.size())
			return 0;
		return nd * NumberOfDigitsMultiplier;
	}

	int GetNumberOfSymbolsScore()
	{
		return NumberOfSymbols() * NumberOfSymbolsMultiplier;
	}

	int GetNumberOfDigitsOrSymbolsInTheMiddleScore()
	{
		return NumberOfDigitsOrSymbolsInTheMiddle() * NumberOfDigitsOrSymbolsInMiddleMultiplier;
	}

	int CPasswordStrength::GetNumberOfRequirementsScore()
	{
		int numRequirenments = 0;
		if (m_password.size() >= SatisfactoryPasswordLength)
			++numRequirenments;
		if (NumberOfUpperCaseCharacters() > 0)
			++numRequirenments;
		if (NumberOfLowerCaseCharacters() > 0)
			++numRequirenments;
		if (NumberOfDigits() > 0)
			++numRequirenments;
		if (NumberOfSymbols() > 0)
			++numRequirenments;

		if (numRequirenments > NumberOfRequirenmentsThreshold)
			return (numRequirenments * NumberOfRequirenmentsMultiplier);
		return 0;
	}

	// Score deduction methods

	int GetLettersOnlyScore()
	{
		return IsAllLetters() ? m_password.size() : 0;
	}

	int GetDigitsOnlyScore()
	{
		return IsAllDigits() ? m_password.size() : 0;
	}

	int GetRepeatedCharsScore()
	{
		size_t len = m_password.size();
		if (len < 2)
			return 0;

		double score = 0.;
		int nRepCount = 0;
		for (_tstring::const_iterator it1 = m_password.begin(); it1 != m_password.end(); ++it1)
		{
			bool bCharExists = false;
			for (_tstring::const_iterator it2 = m_password.begin(); it2 != m_password.end(); ++it2)
			{
				if (*it1 == *it2 && it1 != it2)
				{
					bCharExists = true;
					int dist = std::distance(it1, it2);
					score += ::abs(static_cast<double>(len) / dist);
				}
			}
			if (bCharExists)
			{
				++nRepCount;
				int nUniqueChars = len - nRepCount;
				score = (nUniqueChars > 0) ? ceil(score / nUniqueChars) : ceil(score);
			}
		}
		return static_cast<int>(score);
	}

	int GetNumberOfConsecutiveUpperCaseScore()
	{
		return NumberOfConsecutiveUpperCase() * ConsecutiveCharTypeMultiplier;
	}

	int GetNumberOfConsecutiveLowerCaseScore()
	{
		return NumberOfConsecutiveLowerCase() * ConsecutiveCharTypeMultiplier;
	}

	int GetNumberOfConsecutiveDigitsScore()
	{
		return NumberOfConsecutiveDigits() * ConsecutiveCharTypeMultiplier;
	}

	#undef max

	template<typename F>
	int GetSequentialCharScore(F f)
	{
		if (m_password.size() <= SequentialCharNumThreshold)
			return 0;
		int sequentialCharNum = 0;
		_tstring::const_iterator it = m_password.begin();
		_tstring::const_iterator end = m_password.end();
		// find beginning of sequence of specific character type
		while ((it = find_if(it, end, f)) != end)
		{
			// find end of sequence of specific character type
			_tstring::const_iterator sequenceEnd = find_if_not(it, end, f);
			while (distance(it, sequenceEnd) > SequentialCharNumThreshold)
			{
				// check for increasing
				sequentialCharNum = std::max(sequentialCharNum, GetNextSequenceLength(it, sequenceEnd, [](const TCHAR& ch1, const TCHAR& ch2) { return ch2 - ch1 == 1; } ));
				// check for decreasing
				if (distance(it, sequenceEnd) > SequentialCharNumThreshold)
					sequentialCharNum = std::max(sequentialCharNum, GetNextSequenceLength(it, sequenceEnd, [](const TCHAR& ch1, const TCHAR& ch2) { return ch1 - ch2 == 1; } ));
				else
					++it;
			}
			it = sequenceEnd;
		}
		if (sequentialCharNum > SequentialCharNumThreshold)
			return (sequentialCharNum - SequentialCharNumThreshold) * SequentialCharNumMultiplier;
		return 0;
	}

	template<typename BinaryPredicate>
	int GetNextSequenceLength(_tstring::const_iterator& begin, const _tstring::const_iterator& end, BinaryPredicate pred)
	{
		_tstring::const_iterator last = std::is_sorted_until(begin, end, pred);
		_tstring::difference_type diff = distance(begin, last);
		if (diff > 1)
			begin = last - 1; // last char may be the beginning of the next (opposite) sequence, e.g. '0' in sequence '2101234'
		return diff;
	}

	int GetSequentialDigitsScore()
	{
		return GetSequentialCharScore(_istdigit);
	}

	int GetSequentialCharactersScore()
	{
		return GetSequentialCharScore(_istalpha);
	}
	
	static bool IsSymbol(TCHAR ch)
	{
		return _istgraph(ch) && _istalnum(ch) == 0;
	}

	static bool IsDigitOrSymbol(TCHAR ch)
	{
		return IsSymbol(ch) || _istdigit(ch);
	}

};

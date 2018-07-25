#pragma once

#ifndef RUSH_LEXER_LEXER_HPP
#define RUSH_LEXER_LEXER_HPP

#include "rush/lexer/token.hpp"
#include "rush/lexer/options.hpp"
#include "rush/lexer/analysis.hpp"

#include <string>
#include <iostream>

namespace rush {
	lexical_analysis lex(char const*, lexer_options const& = {});
	lexical_analysis lex(std::string const&, lexer_options const& = {});
	lexical_analysis lex(std::istream&, lexer_options const& = {});
}


// ## THE BELOW RESIDES IN THIS HEADER TEMPMORARILY WHILE DOING TDD,
// ## WILL BE MOVED INTO THE CPP FILE WHEN READY.
#include "rush/core/charinfo.hpp"
#include "rush/core/iterator.hpp"

namespace rush {
	template <typename InIter>
	void skip_hspace(InIter& first, InIter const& last) {
		assert(first != last && "unexpected end of range.");
		advance_if(first, last, charinfo::is_hspace);
	}

	template <typename InIter>
	void skip_vspace(InIter& first, InIter const& last) {
		assert(first != last && "unexpected end of range.");
		advance_if(first, last, charinfo::is_vspace);
	}

	template <typename InIter>
	InIter scan_integer_literal(InIter first, InIter const& last) {
		assert(first != last && "unexpected end of range.");
		assert(charinfo::is_digit(*first) && "expected a leading digit while attempting to scan an integer literal.");

		if (charinfo::is_zero_digit(*first)) return ++first;
		advance_if(first, last, charinfo::is_digit);
		return first;
	}

	template <typename InIter>
	InIter scan_floating_literal(InIter first, InIter const& last) {
		assert(first != last && "unexpected end of range.");
		assert((charinfo::is_digit(*first) || *first == '.')
			&& "expected a leading digit, or dot, while attempting to scan a floating literal.");

		return first;
	}

	template <typename InIter>
	InIter scan_string_literal(InIter first, InIter const& last) {
		assert(first != last && "unexpected end of range.");
		assert(charinfo::is_quote(*first) && "expected a leading, double quotation mark, while attempting to scan a string literal.");

		auto prev = *first;
		advance_if(++first, last, [&first, &prev](auto const& cp) {
			if (prev != '\\' && charinfo::is_quote(cp))
			{ ++first; return false; }
			prev = cp; return true;
		});

		return first;
	}

	template <typename InIter>
	InIter scan_identifier(InIter first, InIter const& last) {
		assert(first != last && charinfo::is_ident_head(*first));
		advance_if(++first, last, charinfo::is_ident_body);
		return first;
	}
	}

#endif // RUSH_LEXER_LEXER_HPP

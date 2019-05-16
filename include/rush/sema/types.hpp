#pragma once

#ifndef RUSH_SEMA_TYPES_HPP
#define RUSH_SEMA_TYPES_HPP

#include "rush/sema/symbol.hpp"
#include <tuple>

namespace rush::sema {
	bool is_type_match(symbol const& lhs, symbol const& rhs);
	bool is_type_mismatch(symbol const& lhs, symbol const& rhs);
	std::tuple<symbol, bool> reduce_type_symbols(symbol const& lhs, symbol const& rhs);
}

#endif // RUSH_SEMA_TYPES_HPP

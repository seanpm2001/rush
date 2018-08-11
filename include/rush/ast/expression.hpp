#pragma once

#ifndef RUSH_AST_EXPRESSION_HPP
#define RUSH_AST_EXPRESSION_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/type.hpp"

#include <utility>
#include <memory>

namespace rush::ast {

	// Expressions are a tree-like structure with a possibly centralized pool of
	// memory, enabled via move semantics (?? possibly ??)
	class expression : public node {
	protected:
		expression(expression const&) = delete;
		void operator = (expression const&) = delete;

	public:
		expression() = default;
		virtual ast::type result_type() const {
			return ast::undefined_type;
		}
	};
}

#include "rush/ast/expr/unary.hpp"
#include "rush/ast/expr/binary.hpp"
#include "rush/ast/expr/literal.hpp"
#include "rush/ast/expr/identifier.hpp"

#endif // RUSH_AST_EXPRESSION_HPP

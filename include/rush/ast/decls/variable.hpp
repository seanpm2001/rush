/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#pragma once

#ifndef RUSH_AST_DECLS_VARIABLE_HPP
#define RUSH_AST_DECLS_VARIABLE_HPP

#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/decls/storage.hpp"
#include "rush/ast/ptrns/pattern.hpp"


namespace rush::ast {
	class variable_declaration;

	namespace decls {
		std::unique_ptr<variable_declaration>
         variable(std::unique_ptr<ast::pattern> patt);
	}

	class variable_declaration : public storage_declaration {
		struct factory_tag_t {};

		friend std::unique_ptr<variable_declaration>
			decls::variable(std::unique_ptr<ast::pattern>);

	public:
		variable_declaration(
         std::unique_ptr<ast::pattern> patt,
         factory_tag_t)
			: storage_declaration { std::move(patt) } {}

		virtual declaration_kind kind() const noexcept override {
			return declaration_kind::variable;
		}

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_variable_decl(*this);
		}
	};

	namespace decls {
      inline std::unique_ptr<variable_declaration>
         variable(std::unique_ptr<ast::pattern> patt) {
            return std::make_unique<variable_declaration>(
               std::move(patt),
               variable_declaration::factory_tag_t {});
         }
	} // rush::ast::decls
} // rush::ast

#endif // RUSH_AST_DECLS_VARIABLE_HPP

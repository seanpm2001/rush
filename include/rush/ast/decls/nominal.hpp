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

#ifndef RUSH_AST_DECLS_NOMINAL_HPP
#define RUSH_AST_DECLS_NOMINAL_HPP

#include "rush/ast/identifier.hpp"
#include "rush/ast/decls/declaration.hpp"

namespace rush::ast {
   class nominal_declaration : public typed_declaration {
   public:
      virtual std::string_view name() const = 0;

      ast::identifier identifier() const { return { this }; };
   protected:
      nominal_declaration() = default;
   };
}

#endif // RUSH_AST_DECLS_NOMINAL_HPP

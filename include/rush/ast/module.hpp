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

#ifndef RUSH_AST_MODULE_HPP
#define RUSH_AST_MODULE_HPP

#include "rush/extra/iterator_range.hpp"
#include "rush/extra/dereferencing_iterator.hpp"

#include "rush/ast/decls/import.hpp"
#include "rush/ast/decls/module.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/decls/undeclared.hpp"

#include <vector>
#include <algorithm>

namespace rush::ast {
   class module : public ast::node {
   private:
      std::vector<std::unique_ptr<ast::import_declaration>> _imports;
      std::vector<std::unique_ptr<ast::module_declaration>> _decls;
      mutable std::unique_ptr<ast::declaration> _undecl;
      std::string _id;

   public:
      module(std::string id)
         : _id { std::move(id) } {}

      std::string const& id() const noexcept {
         return _id;
      }

      void push_back(std::unique_ptr<ast::import_declaration> imp) {
         _imports.push_back(std::move(imp));
      }

      void push_back(std::unique_ptr<ast::declaration> decl, ast::module_access access) {
         _decls.push_back(std::make_unique<ast::module_declaration>(std::move(decl), access));
      }

      auto imports() const noexcept {
         return rush::make_iterator_range(
            make_deref_iterator(_imports.begin()),
            make_deref_iterator(_imports.end()));
      }

      auto declarations() const noexcept {
         return rush::make_iterator_range(
            make_deref_iterator(_decls.begin()),
            make_deref_iterator(_decls.end()));
      }

      ast::declaration const& undeclared_declaration() const {
         return _undecl == nullptr
              ? *(_undecl = std::make_unique<ast::undeclared_identifier>("$$no-name$$"))
              : *(_undecl);
      }

      ast::nominal_declaration const* undeclared_identifier(std::string name) {
         auto udid = std::make_unique<ast::undeclared_identifier>(std::move(name));
         push_back(std::move(udid), ast::module_access::internal);
         return static_cast<ast::nominal_declaration const*>(&_decls.back()->declaration());
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_module(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         std::for_each(_imports.begin(), _imports.end(), [this, &scope](auto& imp) { attach(scope, *imp); });
         std::for_each(_decls.begin(), _decls.end(), [this, &scope](auto& decl) { attach(scope, *decl); });
      }

      virtual void detached(ast::context&) override {
         std::for_each(_imports.begin(), _imports.end(), [this](auto& imp) { detach(*imp); });
         std::for_each(_decls.begin(), _decls.end(), [this](auto& decl) { detach(*decl); });
      }
   };
} // rush::ast

#endif // RUSH_AST_MODULE_HPP

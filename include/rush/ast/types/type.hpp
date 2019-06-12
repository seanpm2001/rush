#pragma once

#ifndef RUSH_AST_TYPE_HPP
#define RUSH_AST_TYPE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"

#include <memory>
#include <string>
#include <type_traits>

namespace rush::ast {
   class array_type;
   class tuple_type;
   class builtin_type;
   class builtin_void_type;
   class builtin_bool_type;
   class builtin_integral_type;
   class builtin_floating_type;
   class function_type;

   enum class type_kind {
      builtin_error,
      builtin_void,
      builtin_bool,
      builtin_integral,
      builtin_floating,
      array,
      tuple,
      function
   };

   class type : public node {
   public:
      virtual ast::type_kind kind() const = 0;

      std::string to_string() const;
   };

   //! Value object that references an actual instance of rush::ast::type.
	class type_ref {
   public:
      type_ref(ast::type const& ref)
         : _ptr { std::addressof(ref) } {}

      ast::type_kind kind() const noexcept {
         return _ptr->kind();
      }

      std::string to_string() const {
         return _ptr->to_string();
      }

      template <typename T>
      bool is() const noexcept {
         static_assert(std::is_base_of_v<ast::type, T>, "T is not a dervied rush::ast::type");
         // return type_traits<T>::kind() == _ptr->kind();
         return false;
      }

      template <typename T>
      T const* as() const noexcept {
         static_assert(std::is_base_of_v<ast::type, T>, "T is not a derived rush::ast::type");
         return reinterpret_cast<T const*>(_ptr);
      }

      void accept(ast::visitor& v) const {
         _ptr->accept(v);
      }

      void accept(ast::visitor&& v) const {
         _ptr->accept(std::move(v));
      }

      friend bool operator == (ast::type_ref const& lhs, ast::type_ref const& rhs) {
         return lhs._ptr == rhs._ptr;
      }

      friend bool operator != (ast::type_ref const& lhs, ast::type_ref const& rhs) {
         return !(operator ==(lhs, rhs));
      }

   private:
      ast::type const* _ptr;
   };
}

#endif // RUSH_AST_TYPE_HPP

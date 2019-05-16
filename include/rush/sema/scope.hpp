#pragma once

#ifndef RUSH_SEMA_SCOPE_HPP
#define RUSH_SEMA_SCOPE_HPP

#include "rush/sema/symbol.hpp"

#include <unordered_map>
#include <stack>
#include <optional>
#include <string>

namespace rush::sema {
   class scope;
   class scope_chain;

   enum class scope_kind {
      function,
      struct_,
      class_,
      enum_,
      concept,
      interface,
      extension,
      module,
      block,
   };

   /*! \brief Represents a lexical scope of a particular kind (e.g. block, function, class).
    *         Each scope maintains a table of symbols and a pointer to its parent scope which
    *         enables a chained lookup of symbols while isolating symbol lookup from
    *         unrelated scopes.
    */
	class scope final {
      friend class scope_chain;

      scope(scope const&) = delete;
		void operator = (scope const&) = delete;

		scope& operator = (scope&& other) {
         _kind = other._kind;
			_parent = other._parent;
			_symtab = std::move(other._symtab);
			return *this;
		}

   public:
      scope(scope&& other)
         : _symtab { std::move(other._symtab) }
         , _parent(other._parent)
         , _kind(other._kind) {}

      using symbol_t = sema::symbol;
      using symbol_table_t = std::unordered_map<std::string, symbol_t>;

      //! \brief Gets the depth of the scope relative to its ancestor scopes.
      std::size_t depth() const noexcept {
			return !is_global() ? parent()->depth() + 1 : 0;
		}

      //! \brief Gets the parent of this scope if it is nested; otherwise null.
		scope* const parent() const noexcept {
			return _parent;
		}

      //! \brief Returns true if this scope is the root (i.e. does not have a parent).
		bool is_global() const noexcept {
			return _parent == nullptr;
		}

      //! \brief Returns true if this scope is nested directly within the specified parent scope.
		bool is_local_to(scope const& parent) const noexcept {
         return _parent == &parent;
		}

      //! \brief Returns true if the scope is nested somewhere with the parents chain of scopes.
      bool is_descendent_of(scope const& parent) const noexcept {
         if (_parent == nullptr) return false;
         if (is_local_to(parent)) return true;
         return _parent->is_local_to(parent);
      }

      //! \brief Inserts a symbol within the scope's symbol table.
      void insert(symbol_t);

      /*! \brief Performs a lookup of the symbol with the specified name.
       *         This is a chained lookup, starting at the scope the method
       *         is invoked on and then performing lookup against its the parent
       *         scope recursively until either the root/global scope has been reached
       *         or a symbol entry for the specified name is found.
       */
      symbol_t lookup(std::string name);

   private:
      symbol_table_t _symtab;
      scope_kind _kind;
      scope* _parent;

      scope(scope_kind kind, scope* parent)
         : _symtab {}
         , _kind(kind)
         , _parent(parent) {}
	};


   /*! \brief Maintains a stack of lexical scopes during parsing and provides
    *         accessors for the current scope on top of the stack.
    */
   class scope_chain final {
   public:
      using symbol_t = sema::symbol;

      /*! \brief Returns a reference to the most deeply nested scope. */
      scope& current() const noexcept;

      //! \brief Inserts a symbol within the current scope's symbol table.
      void insert(symbol_t);

      /*! \brief Performs a lookup of the symbol with the specified name.
       *         This is a chained lookup, starting at the scope the method
       *         is invoked on and then performing lookup against its the parent
       *         scope recursively until either the root/global scope has been reached
       *         or a symbol entry for the specified name is found.
       */
      symbol_t lookup(std::string name);

      void push(scope_kind);
      void pop();

   private:
      std::stack<scope> _scopes;
   };
}

#endif // RUSH_SEMA_SCOPE_HPP

#pragma once

#ifndef RUSH_PARSER_PARSER_HPP
#define RUSH_PARSER_PARSER_HPP

#include <string>
#include <iostream>

namespace rush {

	struct parser_options { };
	class abstract_syntax_tree { }; // move to librush-ast..

	abstract_syntax_tree parse(std::string, parser_options const& = {});
	abstract_syntax_tree parse(std::istream&, parser_options const& = {});
} // rush

#endif // RUSH_PARSER_PARSER_HPP

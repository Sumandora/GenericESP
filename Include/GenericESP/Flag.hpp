#ifndef GENERICESP_FLAG_HPP
#define GENERICESP_FLAG_HPP

#include "Element/Text.hpp"
#include "Element/Element.hpp"

#include <functional>
#include <string>
#include <unordered_map>

namespace GenericESP {

	struct Flag : Text {
		GENERICESP_SETTING(std::string, format);

		std::string name;

		using Remaps = std::unordered_map<std::string, std::function<std::string(const EntityType*)>>;

		Remaps remaps;

		explicit Flag(std::string name, Remaps remaps);
		~Flag() override = default;

		[[nodiscard]] std::string compute_text(const EntityType* e) const;
	};

}

#endif

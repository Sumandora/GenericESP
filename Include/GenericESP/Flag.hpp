#ifndef GENERICESP_FLAG_HPP
#define GENERICESP_FLAG_HPP

#include "Element/Text.hpp"
#include "Element/Element.hpp"
#include "OpaqueLambda.hpp"

#include "imgui.h"

#include <string>
#include <unordered_map>

namespace GenericESP {

	struct Flag : Text {
		std::string name;

		using Provider = OpaqueLambda<std::string>;
		using Remaps = std::unordered_map<std::string, Provider>;

		Remaps remaps;

		explicit Flag(std::string name, Remaps remaps);
		~Flag() override = default;

		[[nodiscard]] std::string compute_text(const EntityType* e) const;

		float get_font_scale(const EntityType* e) const override = 0;
		ImColor get_font_color(const EntityType* e) const override = 0;
		bool get_shadow(const EntityType* e) const override = 0;
		float get_shadow_offset(const EntityType* e) const override = 0;
		ImColor get_shadow_color(const EntityType* e) const override = 0;

		GENERICESP_SETTING(std::string, format);
	};

}

#endif

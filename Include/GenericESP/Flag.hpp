#ifndef GENERICESP_FLAG_HPP
#define GENERICESP_FLAG_HPP

#include "Element/Text.hpp"

namespace GenericESP {

	struct Flag : Renderable, Serializable {
		std::string name;
		Text textElement;

		using Provider = std::function<std::string(const EntityType*)>;
		using Remaps = std::unordered_map<std::string, Provider>;

		Remaps remaps;
		std::string format;

		explicit Flag(ESP* base, std::string name, Remaps remaps, std::string defaultFormat);

		[[nodiscard]] std::string computeText(const EntityType* e) const;
		void renderGui() override;
		[[nodiscard]] SerializedTypeMap serialize() const override;
		void deserialize(const SerializedTypeMap &map) override;
	};

}

#endif

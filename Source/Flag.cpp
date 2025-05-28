#include "GenericESP/Flag.hpp"

#include "GenericESP/Element/Element.hpp"

#include <cstddef>
#include <string>
#include <utility>

using namespace GenericESP;

Flag::Flag(std::string name, Flag::Remaps remaps)
	: name(std::move(name))
	, remaps(std::move(remaps))
{
}

std::string Flag::compute_text(const EntityType* e) const
{
	std::string result = get_format(e);
	for (const auto& [varName, provider] : remaps) {
		size_t pos = 0;
		while ((pos = result.find('%' + varName + '%', pos)) != std::string::npos) {
			const std::string replacement = provider(e);
			result.replace(pos, varName.length() + 2, replacement);
			pos += replacement.length();
		}
	}
	return result;
}

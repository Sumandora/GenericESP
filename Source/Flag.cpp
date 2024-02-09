#include "GenericESP/Flag.hpp"

using namespace GenericESP;

Flag::Flag(ESP* base, std::string name, Flag::Remaps remaps, std::string defaultFormat)
	: name(std::move(name))
	, textElement(base, "Text element", false)
	, remaps(std::move(remaps))
	, format(std::move(defaultFormat))
	, formattingRenderer(rendererFactory.createTextRenderer())
{
}

std::string Flag::computeText(const EntityType* e) const
{
	std::string result = format;
	for (const auto& [varName, provider] : remaps) {
		size_t pos = 0;
		while ((pos = result.find('%' + varName + '%', pos)) != std::string::npos) {
			std::string replacement = provider(e);
			result.replace(pos, varName.length() + 2, replacement);
			pos += replacement.length();
		}
	}
	return result;
}

void Flag::renderGui()
{
	ImGui::PushID(name.c_str());
	textElement.renderGui();
	formattingRenderer("Formatting", format);
	ImGui::PopID();
}

SerializedTypeMap Flag::serialize() const
{
	SerializedTypeMap map;
	map.putSubtree(textElement.id, textElement.serialize());
	map["Format"] = format;
	return map;
}

void Flag::deserialize(const SerializedTypeMap& map)
{
	auto textOpt = map.getSubtree(textElement.id);
	if(textOpt.has_value())
		textElement.deserialize(textOpt.value());
	auto formatOpt = map.get<std::string>("Format");
	if(formatOpt.has_value())
		format = formatOpt.value();
}
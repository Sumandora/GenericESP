#include "GenericESP/Element/Flags.hpp"

using namespace GenericESP;

Flags::Flags(ESP* base, std::string id, std::initializer_list<Flag*> flags)
	: SidedElement(base, std::move(id), Side::RIGHT)
	, spacing{ StaticConfig<float>{ "Spacing", 1.0f, base->createFloatRenderer(0.0, 10.0f, "%.2f") } }
	, flagOrder{ this->flags, [](const std::unique_ptr<Flag>& flag) { return flag->name; } }
{
	this->flags.reserve(flags.size());
	for (Flag* flagPtr : flags) {
		this->flags.emplace_back(std::unique_ptr<Flag>(flagPtr));
	}
}

ImVec2 Flags::drawEntry(ImDrawList* drawList, const EntityType* e, ImRect& rect, const std::unique_ptr<Flag>& flag, float yOffset) const
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float spacing = this->spacing(e);
#pragma clang diagnostic pop
	const std::string text = flag->computeText(e);
	const Text& textElement = flag->textElement;

	switch (getSide(e)) {
	case Side::TOP: {
		auto size = textElement.draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Min.y - spacing - yOffset }, TextAlignment::CENTERED, VerticalAlignment::ABOVE_POSITION);
		if (size.has_value())
			return { yOffset + size->y, size->y };
	}
	case Side::LEFT: {
		auto size = textElement.draw(drawList, e, text, { rect.Min.x - spacing, rect.Min.y + yOffset }, TextAlignment::RIGHT_BOUNDED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			return size.value();
	}
	case Side::BOTTOM: {
		auto size = textElement.draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Max.y + spacing + yOffset }, TextAlignment::CENTERED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			return { yOffset + size->y, size->y };
	}
	case Side::RIGHT: {
		auto size = textElement.draw(drawList, e, text, { rect.Max.x + spacing, rect.Min.y + yOffset }, TextAlignment::LEFT_BOUNDED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			return size.value();
	}
	}
	return { 0.0f, 0.0f };
}

void Flags::draw(ImDrawList* drawList, const EntityType* e, UnionedRect& unionedRect) const
{
	if (!enabled(e))
		return;

	ImRect& rect = chooseRect(e, unionedRect);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float spacing = this->spacing(e);
#pragma clang diagnostic pop

	float biggestOffset = 0.0f;
	float yOffset = 0.0f;

	Side side = getSide(e);

	auto process = [&](const std::unique_ptr<Flag>& flag) {
		ImVec2 offset = drawEntry(drawList, e, rect, flag, yOffset);
		if (offset.x > biggestOffset)
			biggestOffset = offset.x;
		yOffset += offset.y;
	};

	if (side == Side::TOP) // We render from bottom to top in this case, so reverse the flags to get the same order
		for (const auto& flag : std::ranges::views::reverse(flags))
			process(flag);
	else
		for (const auto& flag : flags)
			process(flag);

	biggestOffset += spacing;

	switch (side) {
	case Side::TOP:
		rect.Min.y -= biggestOffset;
		break;
	case Side::LEFT:
		rect.Min.x -= biggestOffset;
		break;
	case Side::BOTTOM:
		rect.Max.y += biggestOffset;
		break;
	case Side::RIGHT:
		rect.Max.x += biggestOffset;
		break;
	}
}

void Flags::renderGui()
{
	ImGui::PushID(id.c_str());
	for (Renderable* r : std::initializer_list<Renderable*>{
			 &enabled, &side, &spacing })
		r->renderGui();
	if (ImGui::BeginTabBar("Flags", ImGuiTabBarFlags_Reorderable)) {
		for (std::unique_ptr<Flag>& flag : flags) {
			if (ImGui::BeginTabItem(flag->name.c_str())) {
				flag->renderGui();
				ImGui::EndTabItem();
			}
		}
		if (ImGui::BeginTabItem("Order")) {
			flagOrder.renderGui("Order");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

SerializedTypeMap Flags::serialize() const
{
	SerializedTypeMap map;
	for (const MixableBase* mixable : std::initializer_list<const MixableBase*>{
			 &enabled, &side, &spacing })
		mixable->serialize(map);
	for (const std::unique_ptr<Flag>& flag : flags)
		map.putSubtree(flag->name, flag->serialize());
	SerializedTypeMap order;
	for(std::size_t i = 0; i < flags.size(); i++) {
		order[flags[i]->name] = i;
	}
	map.putSubtree("Order", std::move(order));
	return map;
}

void Flags::deserialize(const SerializedTypeMap& map)
{
	for (MixableBase* mixable : std::initializer_list<MixableBase*>{
			 &enabled, &side, &spacing })
		mixable->deserializeFromParent(map);
	for (const std::unique_ptr<Flag>& flag : flags)
		flag->deserialize(map.getSubtree(flag->name));
	auto& order = map.getSubtree("Order");
	for (const auto& [name, pos] : order) {
		std::size_t i = std::get<std::size_t>(pos);
		auto it = std::find_if(flags.begin(), flags.end(), [&name](const std::unique_ptr<Flag>& flag) { return flag->name == name; });
		if(it != flags.end())
			std::iter_swap(it, std::next(flags.begin(), static_cast<decltype(flags)::difference_type>(i)));
	}
}

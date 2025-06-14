#define IMGUI_DEFINE_MATH_OPERATORS

#include "GenericESP/Element/Bar.hpp"
#include "GenericESP/Element/Circle.hpp"
#include "GenericESP/Element/Element.hpp"
#include "GenericESP/Element/Flags.hpp"
#include "GenericESP/Element/Line.hpp"
#include "GenericESP/Element/Rectangle.hpp"
#include "GenericESP/Element/SidedElement.hpp"
#include "GenericESP/Element/SidedText.hpp"
#include "GenericESP/Flag.hpp"
#include "GenericESP/UnionedRect.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <numbers>
#include <string>
#include <utility>
#include <vector>

struct Entity {
	int health = 69;
	int max_health = 100;
	const char* name = "Placeholder entity";
	float flag_percentage = 13.37F;
	float another_flag_percentage = 6.9F;
};

using namespace GenericESP;

static bool render_bool(const std::string& label, bool& thing)
{
	return ImGui::Checkbox(label.c_str(), &thing);
}

static bool render_color(const std::string& label, ImColor& thing)
{
	ImGui::PushID(label.c_str());
	const bool clicked = ImGui::ColorButton(label.c_str(), thing.Value, ImGuiColorEditFlags_None, ImVec2(0, 0));
	ImGui::SameLine();
	ImGui::Text("%s", label.c_str());

	if (clicked)
		ImGui::OpenPopup("##Picker");

	bool changed = false;
	if (ImGui::BeginPopup("##Picker")) {
		float color[] = { thing.Value.x, thing.Value.y, thing.Value.z, thing.Value.w };
		if (ImGui::ColorPicker4(label.c_str(), color, 0)) {
			thing.Value = ImVec4(color[0], color[1], color[2], color[3]);
			changed = true;
		}
		ImGui::EndPopup();
	}
	ImGui::PopID();

	return changed;
}

static bool render_combo(const std::string& label, std::size_t& thing, const std::vector<const char*>& localization)
{
	int i = static_cast<int>(thing);
	if (ImGui::Combo(label.c_str(), &i, localization.data(), static_cast<int>(localization.size()))) {
		thing = static_cast<std::size_t>(i);
		return true;
	}
	return false;
}

static bool render_float(const std::string& label, float& thing, float min, float max, const char* fmt = "%f")
{
	return ImGui::SliderFloat(label.c_str(), &thing, min, max, fmt);
}

static bool render_int(const std::string& label, int& thing, int min, int max)
{
	return ImGui::SliderInt(label.c_str(), &thing, min, max);
}

static bool render_text(const std::string& label, std::string& thing)
{
	return ImGui::InputText(label.c_str(), thing.data(), thing.capacity() + 1, ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) {
			auto* thing = reinterpret_cast<std::string*>(data->UserData);
			thing->resize(data->BufTextLen);
			data->Buf = thing->data();
			return 0; }, &thing); // from imgui_stdlib
}

template <typename Lambda>
static void render_popup(const std::string& label, const Lambda& lambda)
{
	const auto popup_label = label + "##Popup";

	ImGui::PushID(popup_label.c_str());
	ImGui::Text("%s", label.c_str());
	ImGui::SameLine();
	if (ImGui::Button("..."))
		ImGui::OpenPopup(popup_label.c_str());

	if (ImGui::BeginPopup(popup_label.c_str())) {
		lambda();
		ImGui::EndPopup();
	}
	ImGui::PopID();
}

#define DEFINE_BASIC_FORWARD(ty, name, ...) \
	ty name = __VA_ARGS__;                  \
	ty get_##name(const EntityType*) const override { return name; }

struct DemoRectangle : Rectangle {
	bool enabled = false;

	// Inject entirely new settings based on others
	bool health_based_color = false;
	ImColor color{ 1.0F, 1.0F, 1.0F, 1.0F };
	ImColor dead_color{ 1.0F, 0.0F, 0.0F, 1.0F };
	ImColor alive_color{ 0.0F, 1.0F, 0.0F, 1.0F };
	ImColor get_color(const EntityType* e) const override
	{
		if (health_based_color) {
			const auto* entity = static_cast<const Entity*>(e);
			const float t = static_cast<float>(entity->health) / static_cast<float>(entity->max_health);
			return ImColor{
				dead_color.Value.x + (alive_color.Value.x - dead_color.Value.x) * t,
				dead_color.Value.y + (alive_color.Value.y - dead_color.Value.y) * t,
				dead_color.Value.z + (alive_color.Value.z - dead_color.Value.z) * t,
				dead_color.Value.w + (alive_color.Value.w - dead_color.Value.w) * t
			};
		}
		return color;
	};

	DEFINE_BASIC_FORWARD(float, rounding, 0.0F);

	DEFINE_BASIC_FORWARD(bool, rounding_top_left, true);
	DEFINE_BASIC_FORWARD(bool, rounding_top_right, true);
	DEFINE_BASIC_FORWARD(bool, rounding_bottom_left, true);
	DEFINE_BASIC_FORWARD(bool, rounding_bottom_right, true);

	DEFINE_BASIC_FORWARD(float, thickness, 1.0F);
	DEFINE_BASIC_FORWARD(bool, outlined, true);
	DEFINE_BASIC_FORWARD(ImColor, outline_color, { 0.0F, 0.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(float, outline_thickness, 1.0F);
	DEFINE_BASIC_FORWARD(bool, fill, false);
	DEFINE_BASIC_FORWARD(ImColor, fill_color, { 1.0F, 1.0F, 1.0F, 1.0F });

	virtual ~DemoRectangle() = default;

	void render_gui()
	{
		render_bool("Health-based color", health_based_color);
		if (!health_based_color)
			render_color("Color", color);
		else {
			render_color("Dead color", dead_color);
			render_color("Alive color", alive_color);
		}
		render_float("Rounding", rounding, 0.0F, 10.0F);

		render_popup("Roundings", [this]() {
			render_bool("Rounding top left", rounding_top_left);
			ImGui::SameLine();
			render_bool("Rounding top right", rounding_top_right);
			render_bool("Rounding bottom left", rounding_bottom_left);
			ImGui::SameLine();
			render_bool("Rounding bottom right", rounding_bottom_right);
		});

		render_float("Thickness", thickness, 0.0F, 10.0F);
		render_bool("Outlined", outlined);
		render_color("Outline color", outline_color);
		render_float("Outline thickness", outline_thickness, 0.0F, 10.0F);
		render_bool("fill", fill);
		render_color("Fill color", fill_color);
	}
};

struct DemoBar : Bar {
	bool enabled = false;

	DEFINE_BASIC_FORWARD(Side, side, Side::LEFT);
	DEFINE_BASIC_FORWARD(ImColor, background_color, { 0.0F, 0.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(float, spacing, 1.0F);
	DEFINE_BASIC_FORWARD(float, width, 1.0F);
	DEFINE_BASIC_FORWARD(ImColor, filled_color, { 0.0F, 1.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(ImColor, empty_color, { 1.0F, 0.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(bool, gradient, false);
	DEFINE_BASIC_FORWARD(int, hue_steps, 3);
	DEFINE_BASIC_FORWARD(bool, flipped, false);
	DEFINE_BASIC_FORWARD(bool, outlined, true);
	DEFINE_BASIC_FORWARD(ImColor, outline_color, { 0.0F, 0.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(float, outline_thickness, 1.0F);

	using Bar::Bar;
	~DemoBar() override = default;

	float get_filled_percentage(const EntityType* e) const override
	{
		const auto* entity = static_cast<const Entity*>(e);
		return static_cast<float>(entity->health) / static_cast<float>(entity->max_health);
	}

	void render_gui()
	{
		render_combo("Side", *reinterpret_cast<std::size_t*>(&side),
			{
				"Top",
				"Left",
				"Bottom",
				"Right",
			});
		render_color("Background color", background_color);
		render_float("Spacing", spacing, 0.0F, 10.0F);
		render_float("Width", width, 0.0F, 10.0F);
		// Renaming settings
		render_color("Alive color", filled_color);
		render_color("Dead color", empty_color);
		render_bool("Gradient", gradient);
		render_int("Hue steps", hue_steps, 0, 10);
		render_bool("Flipped", flipped);
		render_bool("Outlined", outlined);
		render_color("Outline color", outline_color);
		render_float("Outline thickness", outline_thickness, 0.0F, 10.0F);
	}
};

struct DemoBarWithText : BarWithText {
	bool enabled = false;

	DEFINE_BASIC_FORWARD(Side, side, Side::LEFT);
	DEFINE_BASIC_FORWARD(ImColor, background_color, { 0.0F, 0.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(float, spacing, 1.0F);
	DEFINE_BASIC_FORWARD(float, width, 1.0F);
	DEFINE_BASIC_FORWARD(ImColor, filled_color, { 0.0F, 1.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(ImColor, empty_color, { 1.0F, 0.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(bool, gradient, false);
	DEFINE_BASIC_FORWARD(int, hue_steps, 3);
	DEFINE_BASIC_FORWARD(bool, flipped, false);
	DEFINE_BASIC_FORWARD(bool, outlined, true);
	DEFINE_BASIC_FORWARD(ImColor, outline_color, { 0.0F, 0.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(float, outline_thickness, 1.0F);

	DEFINE_BASIC_FORWARD(bool, text_enabled, false);

	ImFont* get_font(const EntityType* /*e*/) const override { return ImGui::GetFont(); }

	DEFINE_BASIC_FORWARD(float, font_size, 12.0F);
	DEFINE_BASIC_FORWARD(ImColor, font_color, { 1.0F, 1.0F, 1.0F, 1.0F });
	DEFINE_BASIC_FORWARD(bool, shadow, true);
	DEFINE_BASIC_FORWARD(float, shadow_offset, 1.0F);
	DEFINE_BASIC_FORWARD(ImColor, shadow_color, { 0.0F, 0.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(bool, hide_when_full, true);

	using BarWithText::BarWithText;
	~DemoBarWithText() override = default;

	float get_filled_percentage(const EntityType* e) const override
	{
		const auto* entity = static_cast<const Entity*>(e);
		return static_cast<float>(entity->health) / static_cast<float>(entity->max_health);
	}

	std::string get_number_text_content(const EntityType* e) const override
	{
		const auto* entity = static_cast<const Entity*>(e);
		return std::to_string(entity->health);
	}

	void render_gui()
	{
		render_combo("Side", *reinterpret_cast<std::size_t*>(&side),
			{
				"Top",
				"Left",
				"Bottom",
				"Right",
			});
		render_color("Background color", background_color);
		render_float("Spacing", spacing, 0.0F, 10.0F);
		render_float("Width", width, 0.0F, 10.0F);
		// Renaming settings
		render_color("Alive color", filled_color);
		render_color("Dead color", empty_color);
		render_bool("Gradient", gradient);
		render_int("Hue steps", hue_steps, 0, 10);
		render_bool("Flipped", flipped);
		render_bool("Outlined", outlined);
		render_color("Outline color", outline_color);
		render_float("Outline thickness", outline_thickness, 0.0F, 10.0F);

		render_popup("Number text", [this]() {
			render_bool("Enabled", text_enabled);
			render_float("Font size", font_size, 0.0F, 24.0F);
			render_color("Font color", font_color);
			render_bool("Shadow", shadow);
			render_float("Shadow offset", shadow_offset, 0.0F, 10.0F);
			render_color("Shadow color", shadow_color);
			render_bool("Hide when full", hide_when_full);
		});
	}
};

struct DemoLine : Line {
	bool enabled = false;

	DEFINE_BASIC_FORWARD(ImColor, color, { 1.0F, 1.0F, 1.0F, 1.0F });
	DEFINE_BASIC_FORWARD(float, thickness, 1.0F);
	DEFINE_BASIC_FORWARD(bool, outlined, true);
	DEFINE_BASIC_FORWARD(ImColor, outline_color, { 0.0F, 0.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(float, outline_thickness, 1.0F);

	void render_gui()
	{
		render_color("Color", color);
		render_float("Thickness", thickness, 0.0F, 10.0F);
		render_bool("Outlined", outlined);
		render_color("Outline color", outline_color);
		render_float("Outline thickness", outline_thickness, 0.0F, 10.0F);
	}
};

struct DemoCircle : Circle {
	bool enabled = false;

	DEFINE_BASIC_FORWARD(ImColor, color, { 1.0F, 1.0F, 1.0F, 1.0F });
	DEFINE_BASIC_FORWARD(float, radius, 1.0F);
	DEFINE_BASIC_FORWARD(bool, outlined, true);
	DEFINE_BASIC_FORWARD(ImColor, outline_color, { 0.0F, 0.0F, 0.0F, 1.0F });
	DEFINE_BASIC_FORWARD(float, outline_radius, 1.0F);

	// Injecting settings rendering for GUIs

	ImVec2 circle_offset{ 0.0F, 0.0F }; // Shout-out to that one user that wanted this feature... Cool idea though

	void render_gui()
	{
		render_color("Color", color);
		render_float("Radius", radius, 0.0F, 10.0F);
		render_bool("Outlined", outlined);
		render_color("Outline color", outline_color);
		render_float("Outline radius", outline_radius, 0.0F, 10.0F);

		if (ImGui::Button("Circle offset"))
			ImGui::OpenPopup("##Circle offset");
		if (ImGui::BeginPopup("##Circle offset")) {
			ImGui::DragFloat("Horizontal offset", &circle_offset.x, 0.1F);
			ImGui::DragFloat("Vertical offset", &circle_offset.y, 0.1F);
			ImGui::EndPopup();
		}
	}
};

struct DemoSidedText : SidedText {
	bool enabled = false;

	DEFINE_BASIC_FORWARD(Side, side, Side::TOP);
	DEFINE_BASIC_FORWARD(float, spacing, 1.0F);
	DEFINE_BASIC_FORWARD(float, font_size, 12.0F);

	ImFont* get_font(const EntityType* /*e*/) const override { return ImGui::GetFont(); }

	DEFINE_BASIC_FORWARD(ImColor, font_color, { 1.0F, 1.0F, 1.0F, 1.0F });
	DEFINE_BASIC_FORWARD(bool, shadow, true);
	DEFINE_BASIC_FORWARD(float, shadow_offset, 1.0F);
	DEFINE_BASIC_FORWARD(ImColor, shadow_color, { 0.0F, 0.0F, 0.0F, 1.0F });

	void render_gui()
	{
		render_combo("Side", *reinterpret_cast<std::size_t*>(&side),
			{
				"Top",
				"Left",
				"Bottom",
				"Right",
			});
		render_float("Spacing", spacing, 0.0F, 10.0F);
		render_float("Font size", font_size, 0.0F, 24.0F);
		render_color("Font color", font_color);
		render_bool("Shadow", shadow);
		render_float("Shadow offset", shadow_offset, 0.0F, 10.0F);
		render_color("Shadow color", shadow_color);
	}
};

struct DemoFlag : Flag {
	ImFont* get_font(const EntityType* /*e*/) const override { return ImGui::GetFont(); }

	DEFINE_BASIC_FORWARD(float, font_size, 12.0F);
	DEFINE_BASIC_FORWARD(ImColor, font_color, { 1.0F, 1.0F, 1.0F, 1.0F });
	DEFINE_BASIC_FORWARD(bool, shadow, true);
	DEFINE_BASIC_FORWARD(float, shadow_offset, 1.0F);
	DEFINE_BASIC_FORWARD(ImColor, shadow_color, { 0.0F, 0.0F, 0.0F, 1.0F });
	// NOLINTNEXTLINE(readability-redundant-member-init)
	DEFINE_BASIC_FORWARD(std::string, format, {});

	DemoFlag(std::string name, Remaps remaps, std::string default_format)
		: Flag(std::move(name), std::move(remaps))
		, format(std::move(default_format))
	{
	}
	~DemoFlag() override = default;

	void render_gui()
	{
		render_float("Font size", font_size, 0.0F, 24.0F);
		render_color("Font color", font_color);
		render_bool("Shadow", shadow);
		render_float("Shadow offset", shadow_offset, 0.0F, 10.0F);
		render_color("Shadow color", shadow_color);
		render_text("Format", format);
	}
};

struct DemoFlags : Flags {
	bool enabled = false;

	DEFINE_BASIC_FORWARD(Side, side, Side::RIGHT);

	DEFINE_BASIC_FORWARD(float, spacing, 1.0F);
	DEFINE_BASIC_FORWARD(float, line_spacing, 1.0F);

	std::vector<Flag*> flags;

	const std::vector<Flag*>& get_order(const EntityType* /*entity*/) const override
	{
		return flags;
	}

	explicit DemoFlags(std::vector<Flag*>&& flags)
		: flags(std::move(flags))
	{
	}

	std::size_t current_index = 0;
	void render_gui()
	{
		render_combo("Side", *reinterpret_cast<std::size_t*>(&side),
			{
				"Top",
				"Left",
				"Bottom",
				"Right",
			});

		render_float("Spacing", spacing, 0.0F, 10.0F);
		render_float("Line spacing", line_spacing, 0.0F, 10.0F);

		if (ImGui::BeginListBox("##Order")) {
			for (std::size_t i = 0; i < flags.size(); i++) {
				const bool selected = (current_index == i);
				if (ImGui::Selectable(flags.at(i)->name.c_str(), selected))
					current_index = i;

				if (selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::SameLine();
		ImGui::BeginDisabled(current_index == 0);
		if (ImGui::Button("Up")) {
			auto prev = std::next(flags.begin(), static_cast<decltype(flags)::difference_type>(current_index) - 1);
			auto curr = prev + 1;
			std::iter_swap(curr, prev);
			current_index--;
		}
		ImGui::EndDisabled();
		ImGui::SameLine();
		ImGui::BeginDisabled(current_index == flags.size() - 1);
		if (ImGui::Button("Down")) {
			auto curr = std::next(flags.begin(), static_cast<decltype(flags)::difference_type>(current_index));
			auto next = curr + 1;
			std::iter_swap(curr, next);
			current_index++;
		}
		ImGui::EndDisabled();

		auto* flag = static_cast<DemoFlag*>(flags.at(current_index));
		flag->render_gui();
	}
};

struct EntityESP {
	DemoRectangle box;
	DemoBar bar;
	DemoBarWithText bar2;
	DemoLine line;
	DemoCircle circle;
	DemoSidedText name;

	// Custom flags
	DemoFlags flags{
		{ new DemoFlag{
			  "My flag",
			  { { "percentage", [](const EntityType* e) { return std::to_string(static_cast<const Entity*>(e)->flag_percentage); } } },
			  "My flag: %percentage%" },
			new DemoFlag{
				"Another flag",
				{ { "percentage", [](const EntityType* e) { return std::to_string(static_cast<const Entity*>(e)->another_flag_percentage); } } },
				"Another flag: %percentage%" } }
	};

	UnionedRect draw(ImDrawList* draw_list, const Entity& e, const ImRect& rect) const
	{
		UnionedRect unioned_rect{ rect };

		// Since the trail is kinda supposed to be a 3d object we render it first so that the 2d box overlaps the trail
		std::vector<ImVec2> trail; // Let's imagine this to be a trail of previous positions
		const ImVec2 base_pos = { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5F, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.9F }; // Imagine feet here
		for (int i = 0; i < 100; i++) { // Some wobble
			const float f = static_cast<float>(i) / 100.0F;
			trail.emplace_back(base_pos.x + sinf(f * std::numbers::pi_v<float> * 2) * 30.0F + static_cast<float>(i), base_pos.y - f * 20.0F);
		}
		if (line.enabled)
			line.draw(draw_list, &e, trail);

		if (box.enabled)
			box.draw(draw_list, &e, unioned_rect);

		if (bar.enabled)
			bar.draw(draw_list, &e, unioned_rect);

		if (bar2.enabled)
			bar2.draw(draw_list, &e, unioned_rect);

		if (name.enabled)
			name.draw(draw_list, &e, e.name, unioned_rect);

		if (flags.enabled)
			flags.draw(draw_list, &e, unioned_rect);

		if (circle.enabled)
			circle.draw(draw_list, &e, circle.circle_offset + ImVec2{ rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5F, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.25F }); // Let's imagine this to be a head dot

		return unioned_rect;
	}

	void render_gui()
	{
		render_bool("Box", box.enabled);
		render_popup("Box settings", [this] { box.render_gui(); });

		render_bool("Bar", bar.enabled);
		render_popup("Bar settings", [this] { bar.render_gui(); });

		render_bool("Bar 2", bar2.enabled);
		render_popup("Bar 2 settings", [this] { bar2.render_gui(); });

		render_bool("Line", line.enabled);
		render_popup("Line settings", [this] { line.render_gui(); });

		render_bool("Circle", circle.enabled);
		render_popup("Circle settings", [this] { circle.render_gui(); });

		render_bool("Name", name.enabled);
		render_popup("Name settings", [this] { name.render_gui(); });

		render_bool("Flags", flags.enabled);
		render_popup("Flags settings", [this] { flags.render_gui(); });
	}
};

extern void render()
{
	static EntityESP e;

	if (ImGui::BeginTable("Table", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV)) {
		ImGui::TableNextColumn();
		ImGui::BeginChild("Left");
		e.render_gui();
		ImGui::EndChild();
		ImGui::TableNextColumn();
		ImGui::BeginChild("Right");

		static bool show_debug = false;
		static bool health_sine = false;
		static bool align = false;

		static Entity my_entity{};

		if (health_sine) {
			const uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			static const uint64_t BEGIN_MS = ms;
			my_entity.health = std::clamp(static_cast<int>((sinf(static_cast<float>(ms - BEGIN_MS) / 2000.0F * std::numbers::pi_v<float>) / 2.0F + 0.5F) * static_cast<float>(my_entity.max_health + 50)) - 25, 0, my_entity.max_health);
		}

		const ImVec2 center = ImGui::GetWindowPos() + ImVec2{ ImGui::GetColumnWidth(0), ImGui::GetWindowHeight() } / 2;
		constexpr ImVec2 EXPANSION{ 50, 100 };
		ImRect rect{ center - EXPANSION, center + EXPANSION };
		if (align) {
			rect = ImRect{
				{ std::roundf(rect.Min.x), std::roundf(rect.Min.y) },
				{ std::roundf(rect.Max.x), std::roundf(rect.Max.y) }
			};
		}
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->Flags |= ImDrawListFlags_AntiAliasedLines;
		draw_list->Flags |= ImDrawListFlags_AntiAliasedFill;
		const auto new_unioned_rect = e.draw(draw_list, my_entity, rect);

		if (show_debug) {
			constexpr ImColor GREEN{ 0.0F, 1.0F, 0.0F, 1.0F };
			ImGui::GetWindowDrawList()->AddRect(rect.Min, rect.Max, GREEN, 0.0F, ImDrawFlags_None, 2.0F);
			constexpr ImColor RED{ 1.0F, 0.0F, 0.0F, 1.0F };
			ImGui::GetWindowDrawList()->AddRect(new_unioned_rect.horizontal.Min, new_unioned_rect.horizontal.Max, RED, 0.0F, 0, 2.0F);
			constexpr ImColor BLUE{ 0.0F, 0.0F, 1.0F, 1.0F };
			ImGui::GetWindowDrawList()->AddRect(new_unioned_rect.vertical.Min, new_unioned_rect.vertical.Max, BLUE, 0.0F, ImDrawFlags_None, 2.0F);
		}

		ImGui::Checkbox("Show debug", &show_debug);
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Green is the beginning box\nRed is the horizontally expanded box\nBlue is the vertically expanded box");
		}
		ImGui::Checkbox("Health sine", &health_sine);
		ImGui::Checkbox("Align", &align);
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("May improve visibility through decreased anti aliasing");
		}
		// if (ImGui::Button("Print serialized state")) {
		// 	SerializedTypeMap map = e.serialize();
		// 	std::function<void(const SerializedTypeMap&, std::size_t)> iterativelyPrint = [&](const SerializedTypeMap& map, std::size_t depth) {
		// 		std::string indent(depth, '\t');

		// 		for (const auto& [name, type] : map)
		// 			std::visit([&](const auto& arg) {
		// 				if constexpr (std::is_same_v<std::remove_cvref_t<std::decay_t<decltype(arg)>>, std::unique_ptr<SerializedTypeMap>>) {
		// 					std::cout << indent << name << " {" << std::endl;
		// 					iterativelyPrint(*arg, depth + 1);
		// 					std::cout << indent << "}" << std::endl;
		// 				} else
		// 					std::cout << indent << name << ": " << arg << std::endl;
		// 			},
		// 				type);
		// 	};

		// 	iterativelyPrint(map, 0);
		// }
		// static std::optional<SerializedTypeMap> serializedState = std::nullopt;
		// if (ImGui::Button("Save serialized state")) {
		// 	serializedState = e.serialize();
		// }
		// ImGui::BeginDisabled(!serializedState.has_value());
		// if (ImGui::Button("Load serialized state")) {
		// 	e.deserialize(serializedState.value());
		// }
		// ImGui::EndDisabled();
		ImGui::EndChild();
		ImGui::EndTable();
	}
}

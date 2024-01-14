#include <chrono>

#include "GenericESP.hpp"

struct Entity {
	int health = 69;
	int maxHealth = 100;
	const char* name = "Placeholder entity";
	float flagPercentage = 13.37f;
	float anotherFlagPercentage = 6.9f;
};

using namespace GenericESP;

struct EntityESP : ESP {
	Rectangle box{ this };
	Bar bar{
		this,
		makeOpaque<float, Entity>([](const Entity* entity) { return static_cast<float>(entity->health) / static_cast<float>(entity->maxHealth); }),
		Bar::NumberText{ this, makeOpaque<std::string, Entity>([](const Entity* entity) { return std::to_string(entity->health); }) }
	};
	Bar bar2{
		this,
		makeOpaque<float, Entity>([](const Entity* entity) { return static_cast<float>(entity->health) / static_cast<float>(entity->maxHealth); }),
		Bar::NumberText{ this, makeOpaque<std::string, Entity>([](const Entity* entity) { return std::to_string(entity->health); }) }
	};
	Line line{ this };
	Circle circle{ this };
	SidedText name{ this };
	struct MyFlag : Flag {
		explicit MyFlag(ESP* base)
			: Flag{
				base,
				"My flag",
				{ { "percentage", makeOpaque<std::string, Entity>([](const Entity* e) { return std::to_string(e->flagPercentage); }) } },
				"My flag: %percentage%"
			}
		{
		}
	};
	struct AnotherFlag : Flag {
		explicit AnotherFlag(ESP* base)
			: Flag{
				base,
				"Another flag",
				{ { "percentage", makeOpaque<std::string, Entity>([](const Entity* e) { return std::to_string(e->anotherFlagPercentage); }) } },
				"Another flag: %percentage%"
			}
		{
		}
	};
	Flags flags{ this, { new MyFlag(this), new AnotherFlag(this) } };

	// Injected health-based color
	ImColor aliveColor{ 0.0f, 1.0f, 0.0f, 1.0f };
	ImColor deadColor{ 1.0f, 0.0f, 0.0f, 1.0f };

	ImVec2 circleOffset{ 0.0f, 0.0f }; // Shout-out to that one user that wanted this feature... Cool idea though

	EntityESP()
	{
		box.color.addType("Health-based", DynamicConfig<ImColor>{ makeOpaque<float, Entity>([this](const Entity* e) {
																	 const float t = static_cast<float>(e->health) / static_cast<float>(e->maxHealth);
																	 return ImColor{ ImLerp(deadColor.Value, aliveColor.Value, t) };
																 }),
											  [this](const std::string& id) {
												  static auto displayColor = createColorRenderer();
												  ImGui::PushID(id.c_str());
												  displayColor("Alive color", aliveColor);
												  displayColor("Dead color", deadColor);
												  ImGui::PopID();
											  } });
		bar.filledColor.id = "Alive color";
		bar.emptyColor.id = "Dead color";

		bar2.filledColor.id = "Alive color";
		bar2.emptyColor.id = "Dead color";

		GenericESP::StaticConfig<bool>& enabledCfg = circle.enabled.getSelected().getStaticConfig();
		auto oldRenderer = enabledCfg.render;
		enabledCfg.render = [this, oldRenderer](const std::string& id, bool& thing) {
			oldRenderer(id, thing);
			if (thing) {
				ImGui::SameLine();
				if (ImGui::Button("Circle offset"))
					ImGui::OpenPopup("##Circle offset");
				if (ImGui::BeginPopup("##Circle offset")) {
					ImGui::DragFloat("Horizontal offset", &circleOffset.x, 0.1f);
					ImGui::DragFloat("Vertical offset", &circleOffset.y, 0.1f);
					ImGui::EndPopup();
				}
			}
		};
	}

	GenericESP::UnionedRect draw(ImDrawList* drawList, const Entity& e, const ImRect& rect) const
	{
		GenericESP::UnionedRect unionedRect{ rect };

		// Since the trail is kinda supposed to be a 3d object we render it first so that the 2d box overlaps the trail
		std::vector<ImVec2> trail; // Let's imagine this to be a trail of previous positions
		ImVec2 basePos = { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.9f }; // Imagine feet here
		for (int i = 0; i < 100; i++) { // Some wobble
			float f = static_cast<float>(i) / 100.0f;
			trail.emplace_back(basePos.x + sinf(f * M_PIf * 2) * 30.0f + static_cast<float>(i), basePos.y - f * 20.0f);
		}
		line.draw(drawList, &e, trail);

		box.draw(drawList, &e, unionedRect);

		bar.draw(drawList, &e, unionedRect);

		bar2.draw(drawList, &e, unionedRect);

		name.draw(drawList, &e, e.name, unionedRect);

		flags.draw(drawList, &e, unionedRect);

		circle.draw(drawList, &e, ImVec2{ rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.25f } + circleOffset); // Let's imagine this to be a head dot

		return unionedRect;
	}

	void renderGui()
	{
		if (ImGui::BeginTabBar("Elements", ImGuiTabBarFlags_Reorderable)) {
			if (ImGui::BeginTabItem("Box")) {
				box.renderGui("Box");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Bar")) {
				bar.renderGui("Bar");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Bar 2")) {
				bar2.renderGui("Bar 2");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Line")) {
				line.renderGui("Line");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Circle")) {
				circle.renderGui("Circle");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Name")) {
				name.renderGui("Name");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Flags")) {
				flags.renderGui("Flags");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	std::function<void(const std::string&, bool&)> createBoolRenderer(const std::function<void()>& onChange = [] {}) override
	{
		return [onChange](const std::string& id, bool& thing) {
			if (ImGui::Checkbox(id.c_str(), &thing))
				onChange();
		};
	};
	std::function<void(const std::string&, ImColor&)> createColorRenderer(const std::function<void()>& onChange = [] {}) override
	{
		static ImColor colorClipboard; // You probably want to replace this with a global color clipboard
		return [onChange](const std::string& id, ImColor& thing) {
			ImGui::PushID(id.c_str());
			const bool clicked = ImGui::ColorButton(id.c_str(), thing.Value, ImGuiColorEditFlags_None, ImVec2(0, 0));
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Selectable("Copy")) {
					colorClipboard = thing;
				}
				if (ImGui::Selectable("Paste")) {
					thing = colorClipboard;
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			ImGui::Text("%s", id.c_str());

			if (clicked)
				ImGui::OpenPopup("##Picker");

			if (ImGui::BeginPopup("##Picker")) {
				float color[] = { thing.Value.x, thing.Value.y, thing.Value.z, thing.Value.w };
				if (ImGui::ColorPicker4(id.c_str(), color, 0)) {
					thing.Value = ImVec4(color[0], color[1], color[2], color[3]);
					onChange();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		};
	};
	std::function<void(const std::string&, std::size_t&)> createComboRenderer(const std::initializer_list<std::string>& localization, const std::function<void()>& onChange = [] {}) override
	{
		return [&localization, onChange](const std::string& id, std::size_t& thing) {
			const char* names[localization.size()];
			std::size_t idx = 0;
			for (const auto& local : localization) {
				names[idx] = local.c_str();
				idx++;
			}

			int i = static_cast<int>(thing);
			if (ImGui::Combo(id.c_str(), &i, names, IM_ARRAYSIZE(names))) {
				thing = static_cast<std::size_t>(i);
				onChange();
			}
		};
	}
	std::function<void(const std::string&, float&)> createFloatRenderer(float min, float max, const char* fmt, const std::function<void()>& onChange = [] {}) override
	{
		return [min, max, fmt, onChange](const std::string& id, float& thing) {
			if (ImGui::SliderFloat(id.c_str(), &thing, min, max, fmt))
				onChange();
		};
	}
	std::function<void(const std::string&, int&)> createIntRenderer(int min, int max, const std::function<void()>& onChange = [] {}) override
	{
		return [min, max, onChange](const std::string& id, int& thing) {
			if (ImGui::SliderInt(id.c_str(), &thing, min, max))
				onChange();
		};
	}
};

EntityESP e;

void render()
{
	if (ImGui::BeginTable("Table", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV)) {
		ImGui::TableNextColumn();
		ImGui::BeginChild("Left");
		e.renderGui();
		ImGui::EndChild();
		ImGui::TableNextColumn();
		ImGui::BeginChild("Right");

		static bool showDebug = false;
		static bool healthSine = false;
		static bool align = false;

		static Entity myEntity{};

		if (healthSine) {
			const unsigned long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			static unsigned long long lastMs = ms;
			myEntity.health = std::clamp(static_cast<int>((sinf(static_cast<float>(ms - lastMs) / 2000.0f * M_PIf) / 2.0f + 0.5f) * static_cast<float>(myEntity.maxHealth + 50)) - 25, 0, myEntity.maxHealth);
		}

		const ImVec2 center = ImGui::GetWindowPos() + ImVec2{ ImGui::GetColumnWidth(0), ImGui::GetWindowHeight() } / 2;
		constexpr ImVec2 expansion{ 50, 100 };
		ImRect rect{ center - expansion, center + expansion };
		if (align) {
			rect = ImRect{
				{ std::roundf(rect.Min.x), std::roundf(rect.Min.y) },
				{ std::roundf(rect.Max.x), std::roundf(rect.Max.y) }
			};
		}
		const auto newUnionedRect = e.draw(ImGui::GetWindowDrawList(), myEntity, rect);

		if (showDebug) {
			static ImColor green{ 0.0f, 1.0f, 0.0f, 1.0f };
			ImGui::GetWindowDrawList()->AddRect(rect.Min, rect.Max, green, 0.0f, ImDrawFlags_None, 2.0f);
			static ImColor red{ 1.0f, 0.0f, 0.0f, 1.0f };
			ImGui::GetWindowDrawList()->AddRect(newUnionedRect.horizontal.Min, newUnionedRect.horizontal.Max, red, 0.0f, 0, 2.0f);
			static ImColor blue{ 0.0f, 0.0f, 1.0f, 1.0f };
			ImGui::GetWindowDrawList()->AddRect(newUnionedRect.vertical.Min, newUnionedRect.vertical.Max, blue, 0.0f, ImDrawFlags_None, 2.0f);
		}

		ImGui::Checkbox("Show debug", &showDebug);
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Green is the beginning box\nRed is the horizontally expanded one\nBlue is the vertically expanded one");
		}
		ImGui::Checkbox("Health sine", &healthSine);
		ImGui::Checkbox("Align", &align);
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("May improve visibility through decreased anti aliasing");
		}
		ImGui::EndChild();
		ImGui::EndTable();
	}
}

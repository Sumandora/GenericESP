#include <chrono>

#define IMGUI_DEFINE_MATH_OPERATORS

#include "GenericESP.hpp"
#include "GenericESP/DefaultRenderers.hpp"
#include "GenericESP/RendererFactory.hpp"
#include "GenericESP/Serialization/ImColorSerialization.hpp"

struct Entity {
	int health = 69;
	int maxHealth = 100;
	const char* name = "Placeholder entity";
	float flagPercentage = 13.37f;
	float anotherFlagPercentage = 6.9f;
};

using namespace GenericESP;

DefaultRenderers defaultRenderers;
RendererFactory* const GenericESP::rendererFactory = &defaultRenderers;

struct EntityESP : ESP {
	Rectangle box{ this, "Box" };
	Bar bar{
		this,
		"Bar",
		[](const Entity* entity) { return static_cast<float>(entity->health) / static_cast<float>(entity->maxHealth); }
	};
	Bar bar2{
		this,
		"Bar 2",
		[](const Entity* entity) { return static_cast<float>(entity->health) / static_cast<float>(entity->maxHealth); },
		[](const Entity* entity) { return std::to_string(entity->health); }
	};
	Line line{ this, "Line" };
	Circle circle{ this, "Circle" };
	SidedText name{ this, "Name", Side::TOP };

	// Custom flags
	struct MyFlag : Flag {
		explicit MyFlag(ESP* base)
			: Flag{
				base,
				"My flag",
				{ { "percentage", [](const Entity* e) { return std::to_string(e->flagPercentage); } } },
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
				{ { "percentage", [](const Entity* e) { return std::to_string(e->anotherFlagPercentage); } } },
				"Another flag: %percentage%"
			}
		{
		}
	};
	Flags flags{ this, "Flags", { new MyFlag{ this }, new AnotherFlag{ this } } };

	// Injected health-based color
	ImColor aliveColor{ 0.0f, 1.0f, 0.0f, 1.0f };
	ImColor deadColor{ 1.0f, 0.0f, 0.0f, 1.0f };

	ImVec2 circleOffset{ 0.0f, 0.0f }; // Shout-out to that one user that wanted this feature... Cool idea though

	EntityESP()
	{
		// Inject entirely new settings based on others
		box.color.addType(DynamicConfig<ImColor>{ "Health-based", [this](const Entity* e) {
													 const float t = static_cast<float>(e->health) / static_cast<float>(e->maxHealth);
													 return ImColor{
														 deadColor.Value.x + (aliveColor.Value.x - deadColor.Value.x) * t,
														 deadColor.Value.y + (aliveColor.Value.y - deadColor.Value.y) * t,
														 deadColor.Value.z + (aliveColor.Value.z - deadColor.Value.z) * t,
														 deadColor.Value.w + (aliveColor.Value.w - deadColor.Value.w) * t
													 };
												 },
			[this](const std::string& id) {
				static auto displayColor = GenericESP::rendererFactory->createColorRenderer();
				ImGui::PushID(id.c_str());
				displayColor("Alive color", aliveColor);
				displayColor("Dead color", deadColor);
				ImGui::PopID();
			},
			[this] -> SerializedTypeMap {
				SerializedTypeMap map;

				map.putSubtree("Alive", serializeImColor(aliveColor));
				map.putSubtree("Dead", serializeImColor(deadColor));

				return map;
			},
			[this](const SerializedTypeMap& map) {
				{
					auto opt = map.getSubtree("Alive");
					if (opt.has_value()) {
						deserializeImColor(map, aliveColor);
					}
				}
				{
					auto opt = map.getSubtree("Dead");
					if (opt.has_value()) {
						deserializeImColor(map, deadColor);
					}
				}
			} });

		// Renaming settings
		bar.filledColor.getSelected().rename("Alive color");
		bar.emptyColor.getSelected().rename("Dead color");

		bar2.filledColor.getSelected().rename("Alive color");
		bar2.emptyColor.getSelected().rename("Dead color");

		// Injecting settings rendering for GUIs
		StaticConfig<bool>& enabledCfg = circle.enabled.getSelected().getStaticConfig();
		auto oldRenderer = enabledCfg.renderer;
		enabledCfg.renderer = [this, oldRenderer](const std::string& id, bool& thing) {
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

	UnionedRect draw(ImDrawList* drawList, const Entity& e, const ImRect& rect) const
	{
		UnionedRect unionedRect{ rect };

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
			ImGui::SetTooltip("Green is the beginning box\nRed is the horizontally expanded box\nBlue is the vertically expanded box");
		}
		ImGui::Checkbox("Health sine", &healthSine);
		ImGui::Checkbox("Align", &align);
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("May improve visibility through decreased anti aliasing");
		}
		if (ImGui::Button("Print serialized state")) {
			SerializedTypeMap map = e.serialize();
			std::function<void(const SerializedTypeMap&, std::size_t)> iterativelyPrint = [&](const SerializedTypeMap& map, std::size_t depth) {
				std::string indent(depth, '\t');

				for(const auto& [name, type] : map)
					std::visit([&](const auto& arg) {
						if constexpr (std::is_same_v<std::remove_cvref_t<std::decay_t<decltype(arg)>>, std::unique_ptr<SerializedTypeMap>>) {
							std::cout << indent << name << " {" << std::endl;
							iterativelyPrint(*arg, depth + 1);
							std::cout << indent << "}" << std::endl;
						} else
							std::cout << indent << name << ": " << arg << std::endl;
					}, type);
			};

			iterativelyPrint(map, 0);
		}
		static std::optional<SerializedTypeMap> serializedState = std::nullopt;
		if (ImGui::Button("Save serialized state")) {
			serializedState = e.serialize();
		}
		ImGui::BeginDisabled(!serializedState.has_value());
		if (ImGui::Button("Load serialized state")) {
			e.deserialize(serializedState.value());
		}
		ImGui::EndDisabled();
		ImGui::EndChild();
		ImGui::EndTable();
	}
}

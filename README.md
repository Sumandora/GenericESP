# Generic ESP
This library provides a generic implementation of an ESP (Extra Sensory Perception). The goal of this library is to provide a reusable foundation for creating custom ESP hacks with ImGui. It includes elements such as boxes, bars, text labels, circles, lines, and flags that can be easily configured and extended.

## Features
- Fully configurable boxes, bars, lines, circles, and text labels
- Preloaded with tons of features
- Allows injection of new settings
- Allows injection of setting modifiers
- Allows renaming of settings
- Preloaded with menu render routines
- Customizable renderers and serializers
- Hackable through public-only structs
- And of course: Completely generic with no links to your entity structures

## Implementation
- Include the header files: Add `#include "GenericESP.hpp"` to include the main header file in your project.
- Create a new struct that extends from `ESP`
```cpp
struct EntityESP : ESP {

};
```
- Define each element of this ESP. This example will only use one element for space reasons. For a more complete example check the `Example` folder
```cpp
struct EntityESP : ESP {
	Rectangle box{ this, "Box" };
};
```
- Creating your `draw`-method
```cpp
struct EntityESP : ESP {
	// ...
	
	void draw(ImDrawList* drawList, const Entity& e, const ImRect& rect) const
	{
		UnionedRect unionedRect{ rect };
		box.draw(drawList, &e, unionedRect);
	}
};
```
- Create your `renderGui`-method
```cpp
struct EntityESP : ESP {
	// ...
	
	void renderGui()
	{
		if (ImGui::BeginTabBar("Elements", ImGuiTabBarFlags_Reorderable)) {
			for (Element* e : std::initializer_list<Element*>{
				&box
			})
				if (ImGui::BeginTabItem(e->id.c_str())) {
					e->renderGui();
					ImGui::EndTabItem();
				}
			ImGui::EndTabBar();
		}
	}
};
```
- Create your `serialize`- and `deserialize`-method
```cpp
struct EntityESP : ESP {
	// ...
	
	SerializedTypeMap serialize() const
	{
		SerializedTypeMap map;
		for (const Element* e : std::initializer_list<const Element*>{
			&box
		})
			map.putSubtree(e->id, e->serialize());
		return map;
	}

	void deserialize(const SerializedTypeMap& map)
	{
		for (Element* e : std::initializer_list<Element*>{
			&box
		}) {
			auto opt = map.getSubtree(e->id);
			if(opt.has_value())
				e->deserialize(opt.value());
		}
	}
};
```
- Create your renderers. You should probably replace these, but if you need a point to start then checkout the Example in the `Example` folder
```cpp
struct EntityESP : ESP {
	// ...
	
	std::function<void(const std::string&, bool&)> createBoolRenderer(const std::function<void()>& onChange = [] {}) override {
		// ...
	}
	std::function<void(const std::string&, ImColor&)> createColorRenderer(const std::function<void()>& onChange = [] {}) override {
		// ...
	};
	std::function<void(const std::string&, std::size_t&)> createComboRenderer(const std::initializer_list<std::string>& localization, const std::function<void()>& onChange = [] {}) override {
		// ...
	}
	std::function<void(const std::string&, float&)> createFloatRenderer(float min, float max, const char* fmt, const std::function<void()>& onChange = [] {}) override {
		// ...
	}
	std::function<void(const std::string&, int&)> createIntRenderer(int min, int max, const std::function<void()>& onChange = [] {}) override {
		// ...
	}
};
```
- Congrats, you now have a simple ESP with only a box to call it invoke it like this:
```cpp
static EntityESP e;
e.draw(ImGui::GetWindowDrawList(), myEntity, rect);
```

### More Elements
Generic ESP offers lots of elements, you can also combine them later to make your own.

#### The Bar
The bar is typically used as a health or armor bar.
```cpp
	Bar bar{
		this,
		"Bar",
		[](const Entity* entity) {
			return static_cast<float>(entity->health) / static_cast<float>(entity->maxHealth);
		}
	};
```
If you like a text which displays the number then add it using
```cpp
	Bar bar{
		this,
		"Bar",
		[](const Entity* entity) {
			return static_cast<float>(entity->health) / static_cast<float>(entity->maxHealth); }
		),
		[](const Entity* entity) { return std::to_string(entity->health); }
	};
```

#### The Line
The line can be used as a trail
```cpp
	Line line{ this, "Line" };
```
It is then invoked using a position vector
```cpp
	std::vector<ImVec2> trail;
	// Fill the trail using lots of points
	line.draw(drawList, &e, trail);
```

#### The circle
The circle can be used as a head dot
```cpp
	Circle circle{ this, "Circle" };
```

#### Text labels and their attached-to-side counterpart
Text labels can have many purposes, for example the name of the player or the weapon they are holding.
```cpp
	SidedText name{ this, "Name" };
```

#### Flags
A flag can be defined using the following.
```cpp
	struct MyFlag : Flag {
		explicit MyFlag(ESP* base)
			: Flag{
				base,
				"My flag", // Id
				{
					// Formatting variables
					{ "percentage", [](const Entity* e) { return std::to_string(e->flagPercentage); } }
				},
				"My flag: %percentage%" // Default formatting
			}
		{
		}
	};
```
The flags element is then initialized like this:
```cpp
	Flags flags{ this, "Flags", { new MyFlag{ this } } };
```

## Terminology

When hacking around in the project you are likely going to stumble across the following terminology

| Term            | Meaning                                                                                                                                           |
|-----------------|---------------------------------------------------------------------------------------------------------------------------------------------------|
| ESP             | ESPs are usually classes intended for one game object only, like players and weapons                                                              |
| Opaque Lambdas  | In order to compress the resulting binary most lambdas are opaque                                                                                 |
| Mixable         | A type that wraps configurable types in a way that makes them easily extendable                                                                   |
| Static Configs  | Configurable type that has no relation with entities and does not change based on the entity state. These types are portable                      |
| Dynamic Configs | Configurable type that has clear relation with entities and does change based on the entity state. These types are not portable                   |
| Unioned Rects   | Combine a horizontal and a vertical rectangle to allow side-based elements to grab the necessary rectangle                                        |
| Sided elements  | Elements that are able to change the side they are on. They are attached to the box and typically have a setting that allows their side to change |


## Credits
- [ImGui](https://github.com/ocornut/imgui) by ocornut

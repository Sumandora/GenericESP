# Generic ESP
This library provides a generic implementation of an [ESP (Extra Sensory Perception)](https://en.wikipedia.org/wiki/Cheating_in_online_games#/media/File:AssaultCube_wallhacks_(cropped).png). The goal of this library is to provide a reusable foundation for creating custom ESP hacks with Dear ImGui. It includes elements such as boxes, bars, text labels, circles, lines, and flags that can be easily configured and extended.

## Features
- Fully configurable boxes, bars, lines, circles, and text labels
- Preloaded with tons of features
- Allows injection of new settings
- Allows renaming of settings
- Preloaded with menu render routines
- Customizable renderers and serializers
- Hackable through public-only structs
- And of course: Completely generic with no links to your entity structures

## Implementation
- Include the header files: Add `#include "GenericESP.hpp"` to include the main header file in your project. The following steps will expect `using namespace GenericESP;` for shorter code.
- Define your renderers. You can add default renderers to your project using the `Extensions/DefaultRenderers` submodule. You can also use them as a reference for your own renderers to add a stylized look to all menus.
```cpp
DefaultRenderers defaultRenderers;
RendererFactory* const GenericESP::rendererFactory = &defaultRenderers;
```
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
- Create your `draw`-method
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
- Congrats, you now have a simple ESP box:
  - Define it:
    ```cpp
    static EntityESP e;
    ```
  - Render it on your entity of choice:
    ```cpp
    e.draw(ImGui::GetWindowDrawList(), myEntity, rect);
    ```
  - Render it inside a Dear ImGui gui
    ```cpp
    e.renderGui();
    ```
  - or (de-)serialize it
    ```cpp
    static SerializedTypeMap serializedState = e.serialize();
    e.deserialize(serializedState);
    ```

### More Elements
Generic ESP offers lots of elements, you can also combine them to make your own.

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
    [](const Entity* entity) {
        return std::to_string(entity->health);
    }
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
Text labels can have many purposes, for example showing the name of the player or the weapon they are holding.
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

## All elements and their features listed

<details>

* Rectangles
  * Color - Color of the entire box
  * Rounding - How much edges are rounded off
  * Thickness - How thick the box is
  * Outlined - Whether the box is outlined
    * Outline color - Color of the outline
    * Outline thickness - How thick the outline is
  * Fill - Whether the box is filled
    * Fill color - The color the box is filled with
* Bars
  * Side - Side of the rectangle the bar is on 
  * Background color - Background color of the bar
  * Spacing - Distance from the box
  * Width - Width of the bar
  * Filled color - Color when the bar is filled
  * Empty color - Color when the bar is empty
  * Gradient - Whether the color is a gradient
    * Hue steps - How many steps are in between the top and the bottom
  * Flipped - Whether the bar is flipped
  * Outlined - Whether the bar is outlined
    * Outline color - Color of the outline
    * Outline thickness - How thick the outline is
  * Number Text (optional)
    * Inherits all settings from an unsided text label
    * Hide when full - Hides text when bar is fully filled up
* Text labels
  * Sided text labels
    * Inherits all settings from an unsided text label
    * Side - Side of the rectangle the bar is on
    * Spacing - Distance from the box
  * Unsided text labels
    * Font scale - Size of the text
    * Font color - Color of the text
    * Shadow - Whether the text has a drop shadow
      * Shadow offset - Distance from text
      * Shadow color - Color of the drop shadow
* Lines
  * Color - Color of the line
  * Thickness - How thick the line is
  * Outlined - Whether the line has an outline
    * Outline color - Color of the outline
    * Outline thickness - How thick the outline is
* Circles
  * Color - Color of the circle
  * Thickness - How thick the circle is
  * Outlined - Whether the line has an outline
    * Outline color - Color of the outline
    * Outline thickness - How thick the outline is
* Flags
  * Side - Side of the rectangle the bar is on
  * Spacing - Distance from box
  * Line spacing - Space between each line
  * Each flag:
    * Inherits all settings from an unsided text label
    * Format - Formatting of the flag text
  * Order - Customizable order of the flags

</details>

## Hacking around

- Note that most elements are difficult to move because the lambdas inside them capture the this-pointer. Use std::unique_ptr or similar if you want to store them dynamically.
- The following terminology may be useful:

| Term            | Meaning                                                                                                                                           |
|-----------------|---------------------------------------------------------------------------------------------------------------------------------------------------|
| ESP             | ESPs are usually classes intended for one game object only, like players and weapons                                                              |
| Opaque Lambdas  | In order to compress the resulting binary most lambdas are opaque, that means that the entity pointer is being handled as a void pointer          |
| Mixable         | A type that wraps configurable types in a way that makes them easily extendable                                                                   |
| Static Configs  | Configurable type that has no relation with entities and does not change based on the entity state. These types are portable                      |
| Dynamic Configs | Configurable type that has clear relation with entities and does change based on the entity state. These types are not portable                   |
| Unioned Rects   | Combine a horizontal and a vertical rectangle to allow side-based elements to grab the necessary rectangle                                        |
| Sided elements  | Elements that are able to change the side they are on. They are attached to the box and typically have a setting that allows their side to change |


## Credits
- [Dear ImGui](https://github.com/ocornut/imgui) by ocornut

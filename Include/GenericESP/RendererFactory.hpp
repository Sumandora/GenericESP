#ifndef GENERICESP_RENDERERFACTORY_HPP
#define GENERICESP_RENDERERFACTORY_HPP

#include <functional>
#include <string>

#include "imgui.h"

namespace GenericESP {

	using BoolRenderer	= std::function<void(const std::string&, bool&)>;
	using ColorRenderer	= std::function<void(const std::string&, ImColor&)>;
	using ComboRenderer	= std::function<void(const std::string&, std::size_t&)>;
	using FloatRenderer	= std::function<void(const std::string&, float&)>;
	using IntRenderer	= std::function<void(const std::string&, int&)>;
	using TextRenderer	= std::function<void(const std::string&, std::string&)>;
	using ButtonRenderer= std::function<bool(const std::string&)>;

	using ChangeCallback = std::function<void()>;

	struct RendererFactory {
		virtual ~RendererFactory() = default;

		virtual BoolRenderer _createBoolRenderer(const ChangeCallback& onChange) = 0;
		virtual ColorRenderer _createColorRenderer(const ChangeCallback& onChange) = 0;
		virtual ComboRenderer _createComboRenderer(const std::vector<std::string>& localization, const ChangeCallback& onChange) = 0;
		virtual FloatRenderer _createFloatRenderer(float min, float max, const char* fmt, const ChangeCallback& onChange) = 0;
		virtual IntRenderer _createIntRenderer(int min, int max, const ChangeCallback& onChange) = 0;
		virtual TextRenderer _createTextRenderer(const ChangeCallback& onChange) = 0;
		virtual ButtonRenderer _createButtonRenderer() = 0;

		// Default parameters
		BoolRenderer createBoolRenderer(const ChangeCallback& onChange = [] {});
		ColorRenderer createColorRenderer(const ChangeCallback& onChange = [] {});
		ComboRenderer createComboRenderer(const std::vector<std::string>& localization, const ChangeCallback& onChange = [] {});
		FloatRenderer createFloatRenderer(float min, float max, const char* fmt, const ChangeCallback& onChange = [] {});
		IntRenderer createIntRenderer(int min, int max, const ChangeCallback& onChange = [] {});
		TextRenderer createTextRenderer(const ChangeCallback& onChange = [] {});
		ButtonRenderer createButtonRenderer(); // syntax sugar

	};

	// Set this pointer to an instance of Renderers which will then be used to render contents
	extern RendererFactory& rendererFactory;

}

#endif

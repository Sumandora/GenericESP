#ifndef GENERICESP_DEFAULTRENDERERS_HPP
#define GENERICESP_DEFAULTRENDERERS_HPP

#include "GenericESP/RendererFactory.hpp"

namespace GenericESP {

	struct DefaultRenderers : RendererFactory {
		ImColor* colorClipboard;

		explicit DefaultRenderers(ImColor* colorClipboard = nullptr);

		BoolRenderer _createBoolRenderer(const ChangeCallback& onChange) override;
		ColorRenderer _createColorRenderer(const ChangeCallback& onChange) override;
		ComboRenderer _createComboRenderer(const std::vector<std::string>& localization, const ChangeCallback& onChange) override;
		FloatRenderer _createFloatRenderer(float min, float max, const char* fmt, const ChangeCallback& onChange) override;
		IntRenderer _createIntRenderer(int min, int max, const ChangeCallback& onChange) override;
		TextRenderer _createTextRenderer(const GenericESP::ChangeCallback &onChange) override;
		ButtonRenderer _createButtonRenderer() override;
		PopupRenderer _createPopupRenderer() override;
	};

}

#endif

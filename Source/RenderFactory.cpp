#include "GenericESP/RendererFactory.hpp"

using namespace GenericESP;

BoolRenderer RendererFactory::createBoolRenderer(const ChangeCallback& onChange) {
	return this->_createBoolRenderer(onChange);
}
ColorRenderer RendererFactory::createColorRenderer(const ChangeCallback& onChange) {
	return this->_createColorRenderer(onChange);
}
ComboRenderer RendererFactory::createComboRenderer(const std::vector<std::string>& localization, const ChangeCallback& onChange) {
	return this->_createComboRenderer(localization, onChange);
}
FloatRenderer RendererFactory::createFloatRenderer(float min, float max, const char* fmt, const ChangeCallback& onChange) {
	return this->_createFloatRenderer(min, max, fmt, onChange);
}
IntRenderer RendererFactory::createIntRenderer(int min, int max, const ChangeCallback& onChange) {
	return this->_createIntRenderer(min, max, onChange);
}
TextRenderer RendererFactory::createTextRenderer(const GenericESP::ChangeCallback& onChange) {
	return this->_createTextRenderer(onChange);
}
ButtonRenderer RendererFactory::createButtonRenderer() {
	return this->_createButtonRenderer();
}
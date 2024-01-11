#ifndef GENERICESP_ELEMENT_SIDEDELEMENT_HPP
#define GENERICESP_ELEMENT_SIDEDELEMENT_HPP

#include "../Enum/Side.hpp"
#include "../Renderer/EnumRenderer.hpp"
#include "../UnionedRect.hpp"
#include "Element.hpp"

namespace GenericESP {

	struct SidedElement : Element {

		MixableConfigurableValue<Side> side{
			"Side",
			StaticConfig<Side>{ static_cast<Side>(-1 /*placeholder*/), createEnumComboRenderer(sideLocalization) }
		};

		explicit SidedElement(Side defaultSide)
		{
			side.getSelected().getStaticConfig().thing = defaultSide;
		}

		// TODO get side ourselves

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		[[nodiscard]] static const ImRect& chooseRect(Side side, const UnionedRect& unionedRect)
#pragma clang diagnostic pop
		{
			switch (side) {
			case Side::LEFT:
			case Side::RIGHT:
				return unionedRect.horizontal;
			case Side::TOP:
			case Side::BOTTOM:
				return unionedRect.vertical;
			default:
				throw std::runtime_error("Side was invalid");
			}
		}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		[[nodiscard]] static ImRect& chooseRect(Side side, UnionedRect& unionedRect)
#pragma clang diagnostic pop
		{
			switch (side) {
			case Side::LEFT:
			case Side::RIGHT:
				return unionedRect.horizontal;
			case Side::TOP:
			case Side::BOTTOM:
				return unionedRect.vertical;
			default:
				throw std::runtime_error("Side was invalid");
			}
		}
	};

}

#endif

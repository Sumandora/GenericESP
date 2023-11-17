#ifndef GENERICESP_ELEMENT_SIDEDELEMENT_HPP
#define GENERICESP_ELEMENT_SIDEDELEMENT_HPP

#include "../Enum/Side.hpp"
#include "../Renderer/EnumRenderer.hpp"
#include "../UnionedRect.hpp"
#include "Element.hpp"

namespace GenericESP {

	template <typename EntityType>
	struct SidedElement : Element<EntityType> {

		MixableConfigurableValue<Side, EntityType> side{
			"Side",
			StaticConfig<Side>{ static_cast<Side>(-1 /*placeholder*/), createEnumComboRenderer(sideLocalization) }
		};

		explicit SidedElement(Side defaultSide)
		{
			side.getSelected().getStaticConfig().thing = defaultSide;
		}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		[[nodiscard]] const ImRect& chooseRect(Side side, const UnionedRect& unionedRect) const
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
		[[nodiscard]] ImRect& chooseRect(Side side, UnionedRect& unionedRect) const
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

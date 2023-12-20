#ifndef GENERICESP_ELEMENT_SIDEDELEMENT_HPP
#define GENERICESP_ELEMENT_SIDEDELEMENT_HPP

#include "../Enum/Side.hpp"
#include "../Renderer/EnumRenderer.hpp"
#include "../UnionedRect.hpp"
#include "Element.hpp"

namespace GenericESP {

	template <typename EntityType>
	struct SidedElement : Element<EntityType> {

		struct SideConfigValue : Serializable {
			StaticConfig<size_t> parent{
				-1,
				createEnumComboRenderer(sideLocalization)
			};

			[[nodiscard]] Serialization serialize() const override {
				return Serialization{ "Value", static_cast<size_t>(parent.thing) };
			}
			void deserialize(const Serialization& data) override {
				parent.thing = static_cast<Side>(data.get<size_t>("Value").value().get());
			}

			operator Side() const {
				return parent.thing;
			}
		};

		MixableConfigurableValue<SideConfigValue, EntityType> side{
			"Side",
			SideConfigValue{}
		};

		explicit SidedElement(Side defaultSide)
		{
			side.getSelected().getStaticConfig().thing.parent.thing = defaultSide;
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

		[[nodiscard]] Serialization serialize() const override = 0;
		void deserialize(const Serialization&) override = 0;
	};

}

#endif

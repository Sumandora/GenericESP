#ifndef GENERICESP_OPAQUELAMBDA_HPP
#define GENERICESP_OPAQUELAMBDA_HPP

#include <functional>
#include "ESP.hpp"

namespace GenericESP {

	template <typename T, typename RealT>
	inline std::function<T(const EntityType*)> makeOpaque(const std::function<T(const RealT*)>& transparent)
	{
		if constexpr (std::is_same_v<T, RealT>) {
			return transparent;
		}
		return [transparent](const EntityType* ptr) {
			return transparent(reinterpret_cast<const RealT*>(ptr));
		};
	}
}

#endif

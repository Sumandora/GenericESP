#ifndef GENERICESP_OPAQUELAMBDA_HPP
#define GENERICESP_OPAQUELAMBDA_HPP

#include "Element/Element.hpp"
#include <functional>

namespace GenericESP {

	template <typename ReturnType>
	struct OpaqueLambda {
		std::function<ReturnType(const EntityType*)> lambda;

		template <typename FPtr>
		struct FirstArgument;

		template <typename R, class F, class A1>
		struct FirstArgument<R (F::*)(A1) const> {
			using T = A1;
		};

		template <typename Lambda>
		// NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
		OpaqueLambda(Lambda&& l)
			: lambda([l = std::forward<Lambda>(l)](const void* arg) {
				return l(reinterpret_cast<FirstArgument<decltype(&Lambda::operator())>::T>(arg));
			})
		{
		}

		inline ReturnType operator()(const EntityType* e) const
		{
			return lambda(e);
		}
	};
}

#endif

#ifndef GENERICESP_OPAQUELAMBDA_HPP
#define GENERICESP_OPAQUELAMBDA_HPP

#include "ESP.hpp"
#include <functional>

namespace GenericESP {

	template <typename ReturnType>
	struct OpaqueLambda {
		std::function<ReturnType(const EntityType*)> lambda;

	private:
		template <class FPtr>
		struct first_argument;

		template <class T, class F, class Ent>
		struct first_argument<T (F::*)(Ent) const> {
			typedef Ent type;
		};
	public:

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
		template <typename Lambda>
		OpaqueLambda(Lambda&& l)
			: lambda([l = std::forward<Lambda>(l)](const void* arg) {
				return l(reinterpret_cast<first_argument<decltype(&Lambda::operator())>::type>(arg));
			})
		{
		}
#pragma clang diagnostic pop

		inline ReturnType operator()(const EntityType* e) const
		{
			return lambda(e);
		}
	};
}

#endif

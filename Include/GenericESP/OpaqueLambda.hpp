#ifndef GENERICESP_OPAQUELAMBDA_HPP
#define GENERICESP_OPAQUELAMBDA_HPP

#include "ESP.hpp"
#include <functional>

namespace GenericESP {

	template <typename ReturnType>
	struct OpaqueLambda {
		std::function<ReturnType(const EntityType*)> lambda;

		template <class FPtr>
		struct getEntityType;

		template <class T, class F, class Ent>
		struct getEntityType<T (F::*)(Ent) const> {
			typedef Ent type;
		};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
		template <typename Lambda>
		OpaqueLambda(Lambda&& l)
			: lambda([l = std::forward<Lambda>(l)](const void* arg) {
				return l(reinterpret_cast<getEntityType<decltype(&Lambda::operator())>::type>(arg));
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

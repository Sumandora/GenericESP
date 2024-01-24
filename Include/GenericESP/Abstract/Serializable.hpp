#ifndef GENERICESP_ABSTRACT_SERIALIZABLE_HPP
#define GENERICESP_ABSTRACT_SERIALIZABLE_HPP

#include "../Serialization/Serialization.hpp" // TODO can be removed with forward decl

namespace GenericESP {
	struct Serializable {
		[[nodiscard]] virtual SerializedTypeMap serialize() const = 0;
		virtual void deserialize(const SerializedTypeMap& map) = 0;
	};
}

#endif

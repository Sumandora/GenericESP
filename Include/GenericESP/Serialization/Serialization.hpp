#ifndef GENERICESP_SERIALIZATION_HPP
#define GENERICESP_SERIALIZATION_HPP

#include <unordered_map>
#include <string>
#include <variant>
#include <memory>
#include <vector>

namespace GenericESP {

	struct SerializedType;

	struct SerializedTypeMap : std::unordered_map<std::string, SerializedType> {
	};

	using SerializedTypeBaseType = std::variant<int, std::size_t, float, double, bool, ImColor, std::string, std::unique_ptr<SerializedTypeMap>>;
	struct SerializedType : SerializedTypeBaseType {
		SerializedType(SerializedTypeMap&& uniquePtr)
			: SerializedTypeBaseType(std::make_unique<SerializedTypeMap>(std::move(uniquePtr)))
		{
		}
	};

	struct Serializable {
		[[nodiscard]] virtual SerializedTypeMap serialize() const = 0;
		virtual void deserialize(const SerializedTypeMap&) = 0;
	};
}

#endif

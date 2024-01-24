#ifndef GENERICESP_SERIALIZATION_HPP
#define GENERICESP_SERIALIZATION_HPP

#include <unordered_map>
#include <string>
#include <variant>
#include <memory>
#include <vector>
#include <optional>

namespace GenericESP {

	struct SerializedTypeMap;

	using SerializedType = std::variant<int, std::size_t, float, double, bool, std::string, std::unique_ptr<SerializedTypeMap>>;

	struct SerializedTypeMap : std::unordered_map<std::string, SerializedType> {
		template <typename T> requires std::negation_v<std::is_same<T, std::unique_ptr<SerializedTypeMap>>>
		std::optional<T> get(const std::string& name) const {
			if(!contains(name))
				return std::nullopt;
			return std::get<T>(at(name));
		}

		void putSubtree(const std::string& name, SerializedTypeMap&& subtree) {
			if(subtree.empty())
				return; // If empty then don't store.
			(*this)[name] = std::make_unique<SerializedTypeMap>(std::move(subtree));
		}

		std::optional<std::reference_wrapper<SerializedTypeMap>> getSubtree(const std::string& name) const {
			if(!contains(name))
				return {}; // Either we never stored it or it doesn't exist.
			const auto& ref = std::get<std::unique_ptr<SerializedTypeMap>>(at(name));
			return *ref;
		}
	};


}

#endif

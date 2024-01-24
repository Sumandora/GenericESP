#ifndef GENERICESP_SERIALIZATION_HPP
#define GENERICESP_SERIALIZATION_HPP

#include <unordered_map>
#include <string>
#include <variant>
#include <memory>
#include <vector>

namespace GenericESP {

	struct SerializedTypeMap : std::unordered_map<std::string, std::variant<int, std::size_t, float, double, bool, std::string, std::unique_ptr<SerializedTypeMap>>> {
		template <typename T>
		const T& get(const std::string& name) const {
			return std::get<T>(at(name));
		}

		template <typename T>
		T& get(const std::string& name) {
			return std::get<T>(at(name));
		}


		void putSubtree(const std::string& name, SerializedTypeMap&& subtree) {
			(*this)[name] = std::make_unique<SerializedTypeMap>(std::move(subtree));
		}

		const SerializedTypeMap& getSubtree(const std::string& name) const {
			const auto& ref = get<std::unique_ptr<SerializedTypeMap>>(name);
			return *ref;
		}
	};
}

#endif

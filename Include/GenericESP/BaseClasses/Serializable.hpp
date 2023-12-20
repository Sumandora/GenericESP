#ifndef GENERICESP_BASECLASSES_SERIALIZABLE_HPP
#define GENERICESP_BASECLASSES_SERIALIZABLE_HPP

#include <string>
#include <map>
#include <utility>

namespace GenericESP {

	struct Serialization {
		using SerializationValue = std::variant<Serialization, std::string, bool, int, long, float, double, ImColor>;
		std::map<std::string, SerializationValue> types;

		inline Serialization() = default;

		inline Serialization(std::initializer_list<std::pair<std::string, Serialization>> serializations) {
			for (const auto& pair : serializations)
				types.insert(pair);
		}

		inline Serialization(const std::string& name, SerializationValue data) {
			types[name] = std::move(data);
		}

		void operator[](const std::string& name, const SerializationValue& serialization) {
			types[name] = serialization;
		}

		[[nodiscard]] const SerializationValue& operator[](const std::string& name) const {
			return types.at(name);
		}

		[[nodiscard]] SerializationValue& operator[](const std::string& name) {
			return types.at(name);
		}

		template <typename T>
		[[nodiscard]] std::optional<const std::reference_wrapper<T>> get(const std::string& name) const {
			const Serialization::SerializationValue& formatValue = types.at(name);
			if(std::holds_alternative<T>(formatValue))
				return { { std::get<T>(formatValue) } };
			return std::nullopt;
		}

		template <typename T>
		[[nodiscard]] std::optional<std::reference_wrapper<T>> get(const std::string& name) {
			const Serialization::SerializationValue& formatValue = types.at(name);
			if(std::holds_alternative<T>(formatValue))
				return { { std::get<T>(formatValue) } };
			return std::nullopt;
		}

		[[nodiscard]] bool has(const std::string& name) const {
			return types.contains(name);
		}
	};

	struct Serializable {
		virtual ~Serializable() = default;

		[[nodiscard]] virtual Serialization serialize() const = 0;
		virtual void deserialize(const Serialization&) = 0;
	};
}

#endif

#include "GenericESP/Serialization/Serialization.hpp"

using namespace GenericESP;

void SerializedTypeMap::putSubtree(const std::string& name, SerializedTypeMap&& subtree) {
	if(subtree.empty())
		return; // If empty then don't store.
	(*this)[name] = std::make_unique<SerializedTypeMap>(std::move(subtree));
}


std::optional<std::reference_wrapper<SerializedTypeMap>> SerializedTypeMap::getSubtree(const std::string& name) const {
	if(!contains(name))
		return {}; // Either we never stored it or it doesn't exist.
	const auto& ref = std::get<std::unique_ptr<SerializedTypeMap>>(at(name));
	return *ref;
}

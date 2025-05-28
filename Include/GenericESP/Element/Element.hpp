#ifndef GENERICESP_ELEMENT_HPP
#define GENERICESP_ELEMENT_HPP

// You can override this macro with one that provides default implementations if you desire so
#ifndef GENERICESP_SETTING
#define GENERICESP_SETTING(ty, name) virtual ty get_##name(const EntityType*) const = 0
#endif

namespace GenericESP {
	using EntityType = void;

	// struct Element {
	// 	ESP* base;
	// 	std::string id;
	// 	bool topLevel;

	// 	explicit Element(ESP* base, std::string id, bool topLevel);
	// 	Element(Element&&) = delete; // Moving elements is likely to fail because Mixables use lambdas excessively. If you are in need of moving an element, use a unique ptr or something else that stores the element indirectly
	// 	~Element();

	// 	GENERICESP_SETTING(bool, enabled);
	// };

}

#endif

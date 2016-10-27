#include "..\Exported Headers\Util\PropertySet.h"

Dyn_API::Utility::PropertySet::PropertySet(const size_t propLen)
	: PropertySet(0, propLen) {}

Dyn_API::Utility::PropertySet::PropertySet(const size_t sz, const size_t propLen)
	: properties(sz), propertyLength(propLen) {}

Dyn_API::Utility::PropertySet::PropertySet(const PropertySet & orig, const size_t propLen)
	: PropertySet(orig.properties, propLen) {}

Dyn_API::Utility::PropertySet::PropertySet(const PropertyMap & props, const size_t propLen)
	: properties(props), propertyLength(propLen) {
	properties.erase("");
}

Dyn_API::Utility::PropertySet::PropertySet(const std::initializer_list<property_type>& init, const size_t propLen)
	: properties(init), propertyLength(propLen) {
	properties.erase("");
}

Dyn_API::Utility::PropertySet::iterator Dyn_API::Utility::PropertySet::begin() {
	return properties.begin();
}

Dyn_API::Utility::PropertySet::const_iterator Dyn_API::Utility::PropertySet::begin() const {
	return properties.begin();
}

Dyn_API::Utility::PropertySet::iterator Dyn_API::Utility::PropertySet::end() {
	return properties.end();
}

Dyn_API::Utility::PropertySet::const_iterator Dyn_API::Utility::PropertySet::end() const {
	return properties.end();
}

size_t Dyn_API::Utility::PropertySet::size() const {
	return properties.size();
}

bool Dyn_API::Utility::PropertySet::contains(const_key_type & key) const {
	return properties.count(key) != 0;
}

size_t Dyn_API::Utility::PropertySet::erase(const_key_type & key) {
	return properties.erase(key);
}

Dyn_API::Utility::PropertySet::const_iterator Dyn_API::Utility::PropertySet::erase(const const_iterator & first, const const_iterator & last) {
	return properties.erase(first, last);
}

void Dyn_API::Utility::PropertySet::reserve(const size_t sz) {
	properties.reserve(sz);
}

Dyn_API::Utility::PropertySet::const_iterator Dyn_API::Utility::PropertySet::insert(const const_iterator & where, const property_type & prop) {
	return properties.insert(where, prop);
}

void Dyn_API::Utility::PropertySet::insert(const const_iterator & where, const const_iterator & first, const_iterator last) {
	for (; last != first; last--)
		properties.insert(where, last);
	return properties.insert(where, first);
}

void Dyn_API::Utility::PropertySet::convToText(std::stringstream &out) const {
	for (const_iterator iter(begin()), end(end()); iter != end; iter++)
		out << (((std::string(iter->first) += '=') += iter->second) += "\n");
}

void Dyn_API::Utility::PropertySet::loadFromText(std::stringstream & src) {
	key_type key;
	value_type val;
	size_t start, end;
	std::string str;
	char *const chrs(new char[propertyLength << 2]);
	while (!src.eof()) {
		src.getline(chrs, propertyLength << 2, '=');
		str = chrs;
		for (start = 0; str[start] == ' '; start++);
		for (end = str.size(); str[end - 1] == ' '; end--);
		key = str.substr(start, end - start);

		src.getline(chrs, propertyLength << 2);
		str = chrs;
		for (start = 0; str[start] == ' '; start++);
		for (end = str.size(); str[end - 1] == ' '; end--);
		val = str.substr(start, end - start);

		properties[key] = val;
	}
}

Dyn_API::Utility::PropertySet & Dyn_API::Utility::PropertySet::operator=(const PropertySet & right) {
	properties = right.properties;
	return *this;
}

bool Dyn_API::Utility::PropertySet::operator==(const PropertySet & right) const {
	if (this == &right)
		return true;
	return properties == right.properties;
}

bool Dyn_API::Utility::PropertySet::operator!=(const PropertySet & right) const {
	if (this == &right)
		return false;
	return properties != right.properties;
}

Dyn_API::Utility::PropertySet & Dyn_API::Utility::PropertySet::operator+=(const PropertySet & right) {
	for (const_iterator iter(right.begin()), end(right.end()); iter != end; iter++)
		properties.emplace(*iter);
	return *this;
}

Dyn_API::Utility::PropertySet Dyn_API::Utility::PropertySet::operator+(const PropertySet & right) const {
	return PropertySet(*this) += right;
}

Dyn_API::Utility::PropertySet & Dyn_API::Utility::PropertySet::operator-=(const PropertySet & right) {
	for (const_iterator iter(right.begin()), end(right.end()); iter != end; iter++)
		properties.erase(iter->first);
	return *this;
}

Dyn_API::Utility::PropertySet Dyn_API::Utility::PropertySet::operator-(const PropertySet & right) const {
	return PropertySet(*this) -= right;
}

Dyn_API::Utility::PropertySet::reference_type Dyn_API::Utility::PropertySet::operator[](const_key_type & key) {
	return properties.at(key);
}

Dyn_API::Utility::PropertySet::const_reference_type Dyn_API::Utility::PropertySet::operator[](const_key_type & key) const {
	return properties.at(key);
}

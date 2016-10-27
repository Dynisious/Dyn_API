#pragma once

#include "Utility.h"
#include "StringConverter.h"
#include <unordered_map>
#include <string>
#include <sstream>

namespace Dyn_API {

	namespace Utility {

		/*
		A Set of Strings mapped to strings. PropertySets use StringConverters
		to manipulate it's mappings and also provides functionality to output or load
		it's properties through string stream.*/
		class PropertySet {
		public:
			typedef std::string key_type;
			typedef const key_type const_key_type;
			typedef std::string value_type;
			typedef const value_type const_value_type;
			typedef value_type *pointer_type;
			typedef const_value_type *const_pointer_type;
			typedef value_type &reference_type;
			typedef const_value_type &const_reference_type;
			typedef std::unordered_map<key_type, value_type> PropertyMap;
			typedef PropertyMap::value_type property_type;
			typedef PropertyMap::iterator iterator;
			typedef PropertyMap::const_iterator const_iterator;

			PropertySet(const size_t propLen);
			PropertySet(const size_t sz, const size_t propLen);
			PropertySet(const PropertySet& orig, const size_t propLen);
			PropertySet(const PropertyMap& props, const size_t propLen);
			PropertySet(const std::initializer_list<property_type>& init, const size_t propLen);

			iterator begin();
			const_iterator begin() const;
			iterator end();
			const_iterator end() const;
			/*
			Returns the number of properties in this PropertySet.*/
			size_t size() const;
			/*
			True if there is a property which matches the passed key.*/
			bool contains(const_key_type &key) const;
			/*
			Removes any mappings to the passed key, returns the number of
			mappings which where removed.*/
			size_t erase(const_key_type &key);
			/*
			Erases all the values of the passed iterators.*/
			const_iterator erase(const const_iterator &first, const const_iterator &last);
			/*
			Ensures that there is enough memory allocated to store at least the
			passed number of properties.*/
			void reserve(const size_t sz);
			/*
			Inserts the passed property at the passed position.*/
			const_iterator insert(const const_iterator &where, const property_type &prop);
			/*
			Inserts the passed properties at the passed position.*/
			void insert(const const_iterator &where, const const_iterator &first, const_iterator last);
			/*
			Adds or overwrites the value at the passed key with the passed
			value (after it has gone through the passed StringConverter).*/
			template<class _T, class _Conv = StringConverter<_T>>
			void assign(const_key_type &key, const _T &val, const _Conv &converter = _Conv()) {
				if (key == "")
					throw std::invalid_argument("\"\" cannot be accepted as a valid property key.");
				properties[key] = converter(val);
			}
			/*
			Returns the value at the passed key after it has gone through the
			passed StringConverter*/
			template<class _T, class _Conv = StringConverter<_T>>
			_T to_type(const_key_type &key, const _Conv &converter = _Conv()) {
				return converter(properties.at(key));
			}
			/*
			Outputs all the properties as lines to the passed stringstream
			(formated as "'key'='value'").*/
			void convToText(std::stringstream & out) const;
			/*
			Reads lines from the passed stringstream as properties
			(formated as "'key'='value'"). If keys in the stringstream already
			have values they are overwritten.*/
			void loadFromText(std::stringstream & src);

			PropertySet & operator=(const PropertySet &right);
			bool operator==(const PropertySet &right) const;
			bool operator!=(const PropertySet &right) const;
			/*
			Adds any properties which have keys not common with this
			PropertySet to this property set.*/
			PropertySet & operator+=(const PropertySet &right);
			/*
			Performs "+=" on a copy of this PropertySet.*/
			PropertySet operator+(const PropertySet &right) const;
			/*
			Removes any properties which have common keys between these
			PropertySets.*/
			PropertySet & operator-=(const PropertySet &right);
			/*
			Performs "-=" on a copy of this PropertySet.*/
			PropertySet operator-(const PropertySet &right) const;
			reference_type operator[](const_key_type &key);
			const_reference_type operator[](const_key_type &key) const;

			size_t const propertyLength;

		private:
			PropertyMap properties;

		};

	}

}


#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <vector>

namespace Reflection
{

	enum Types
	{
		TYPE_Nil,
		TYPE_String,
		TYPE_Number,
		TYPE_Bool,
		TYPE_Vector3,
		TYPE_CFrame,
		TYPE_Instance,
		TYPE_Instances
	};

	class Property
	{
	public:
		std::string property;
		Types t;

		Property(Types t, std::string name) : t(t), property(name) {}
	};

	typedef std::vector<Property*>::iterator PropertyIterator;

	class PropertyDescriptorContainer
	{
	private:
		std::vector<Property*> properties;
	public:

		void push(Property* property) { properties.push_back(property); }

		inline Property* property(std::string name)
		{
			PropertyIterator iter = properties.begin();
			for (iter; iter < properties.end(); iter++)
			{
				if ((*iter)->property == name)
					return *iter;
			}
			return nullptr;
		}

	};

	template <typename Class, typename V>
	class GetSet
	{
	public:
		virtual V get(Class* obj) = 0;
		virtual void set(Class* obj, V& value) = 0;
	};

	template <typename Class, typename V>
	class PropertyDescriptor : public Property
	{
	public:

		GetSet<Class, V>* getset;

		template <typename Get, typename Set>
		class GetSetImpl :
			public GetSet<Class, V>
		{
		private:
			Get _get;
			Set _set;
		public:

			V get(Class* obj)
			{
				return (obj->*_get)();
			}

			void set(Class* obj, V& value)
			{
				(obj->*_set)(value);
			}

			GetSetImpl(Get nget, Set nset) : _get(nget), _set(nset) {}
		};

		template<typename Get, typename Set>
		PropertyDescriptor(std::string name, Types t, Get get, Set set, PropertyDescriptorContainer* container) : Property(t, name), getset(new GetSetImpl<Get, Set>(get, set)) { container->push(this); }
	};
}

#endif
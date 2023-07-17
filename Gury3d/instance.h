
#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <vector>

#include "property.h"

namespace RBX
{

	class Instance;

	typedef std::vector<Instance*> Instances;

	class Instance
	{
	private:

		std::string name;
		std::string className;

		Instance* parent;
		Instances* children;

		bool archivable;

	public:

		bool isRenderable;
		bool isAffectedByPhysics;

		bool isParentLocked;

		static Reflection::PropertyDescriptorContainer* properties;

		static Reflection::PropertyDescriptor<Instance, std::string> prop_name;
		static Reflection::PropertyDescriptor<Instance, std::string> prop_className;
		static Reflection::PropertyDescriptor<Instance, bool> prop_archivable;
		static Reflection::PropertyDescriptor<Instance, Instance*> prop_parent;

		template<typename T>
		inline T* findFirstChildOfClass(std::string name)
		{
			Instance* child;
			for (int i = 0; i < getChildren()->size(); i++)
			{
				child = getChildren()->at(i);
				if (child && child->getClassName() == name)
					return reinterpret_cast<T*>(child);
			}
			return 0;
		}

		void setName(std::string newName) { name = newName; }
		std::string getName() { return name; }

		void setClassName(std::string newClassName) { className = newClassName; }
		void setClassName2(std::string newClassName) { }

		bool getArchivable() { return archivable; }
		void setArchivable(bool narchivable) { archivable = narchivable; }

		virtual Instance* clone() const { return new Instance(*this); }
		virtual void remove();

		Instances* getChildren() { return children; }

		std::string getClassName() { return className; }

		void setParent(Instance* instance);
		Instance* getParent();

		Instance* findFirstChild(std::string name);

		Instance() : name("Instance"), className("Instance"), parent(NULL), children(new Instances()), isParentLocked(false) {}
		Instance(Instance* parent) : name("Instance"), className("Instance"), parent(NULL), children(new Instances()), isParentLocked(false) {}

		Instance(std::string name, bool isParentLocked) : name(name), className(name), parent(NULL), children(new Instances()), isParentLocked(isParentLocked) {}

		virtual ~Instance() { }
	};
}

#endif
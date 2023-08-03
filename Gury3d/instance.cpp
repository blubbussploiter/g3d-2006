#include "strings.h"

#include "instance.h"
#include "sounds.h"

using namespace RBX;

static RBX::Sound* pageTurn = RBX::Sound::fromFile(GetFileInPath("/content/sounds/pageturn.wav"));

Reflection::PropertyDescriptorContainer* Instance::properties = new Reflection::PropertyDescriptorContainer();
Reflection::PropertyDescriptor<Instance, std::string> Instance::prop_name("Name", Reflection::Types::TYPE_String, &Instance::getName, &Instance::setName, Instance::properties);
Reflection::PropertyDescriptor<Instance, Instance*> Instance::prop_parent("Parent", Reflection::Types::TYPE_Instance, &Instance::getParent, &Instance::setParent, Instance::properties);
Reflection::PropertyDescriptor<Instance, std::string> Instance::prop_className("className", Reflection::Types::TYPE_String, &Instance::getClassName, &Instance::setClassName2, Instance::properties);
Reflection::PropertyDescriptor<Instance, bool> Instance::prop_archivable("archivable", Reflection::Types::TYPE_Bool, &Instance::getArchivable, &Instance::setArchivable, Instance::properties);

bool RBX::Instance::isAncestorOf(RBX::Instance* descendant)
{
	const RBX::Instance* v2; // eax

	v2 = descendant;
	if (!descendant)
		return 0;
	while (1)
	{
		v2 = v2->parent;
		if (v2 == this)
			break;
		if (!v2)
			return 0;
	}
	return 1;
}

void RBX::Instance::remove()
{
	if (isParentLocked) return;
	if (parent)
	{
		if (std::find(parent->getChildren()->begin(), parent->getChildren()->end(), this) != parent->getChildren()->end())
			parent->getChildren()->erase(std::remove(parent->getChildren()->begin(), parent->getChildren()->end(), this));
	}
	for (RBX::Instance* i : *getChildren())
		i->remove();
	pageTurn->setVolume(0.2f);
	pageTurn->play();
	onRemove();
}

void RBX::Instance::setParent(Instance* instance)
{
	if (isParentLocked)
		return; // Throw an exception later
	if (parent) // Parent already exists, remove this from its childrens
	{
		if(std::find(parent->getChildren()->begin(), parent->getChildren()->end(), this) != parent->getChildren()->end())
			parent->getChildren()->erase(std::remove(parent->getChildren()->begin(), parent->getChildren()->end(), this));
	}
	parent = instance;
	if (parent) // Add this to parent children
	{
		parent->children->push_back(this);
	}
}

Instance* RBX::Instance::getParent()
{
	//if (parent == NULL)
	//	return getRootInstance();
	return parent;
}

Instance* RBX::Instance::findFirstChild(std::string name)
{
	Instance* child;
	for (unsigned int i = 0; i < getChildren()->size(); i++)
	{
		child = getChildren()->at(i);
		if (child && child->getName() == name)
			return child;
	}
	return 0;
}
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

bool RBX::Instance::contains(const RBX::Instance* child)
{
	const RBX::Instance* v2; // eax

	v2 = child;
	if (!child)
		return 0;
	while (v2 != this)
	{
		v2 = v2->parent;
		if (!v2)
			return 0;
	}
	return 1;
}

void RBX::Instance::signalOnDescendentAdded(RBX::Instance* beginParent, RBX::Instance* child)
{
	RBX::Instance* i;
	RBX::Instances* c;

	c = child->getChildren();

	for (i = beginParent; i; i = i->parent)
	{
		i->onDescendentAdded(child);
	}

	for (unsigned int i = 0; i < c->size(); i++)
	{
		RBX::Instance* in = c->at(i);
		child->signalOnDescendentAdded(child, in);
	}
}

void RBX::Instance::setParent(Instance* instance)
{
	RBX::Instance* oldParent;
	oldParent = parent;

	if (isParentLocked)
		return;
	if (oldParent)
	{
		if (std::find(parent->getChildren()->begin(), parent->getChildren()->end(), this) != parent->getChildren()->end())
			parent->getChildren()->erase(std::remove(parent->getChildren()->begin(), parent->getChildren()->end(), this));
	}
	parent = instance;

	if (parent)
	{
		parent->children->push_back(this);
		parent->onChildAdded(this);

		if (!parent->contains(oldParent))
			parent->signalOnDescendentAdded(parent, this);
	}
}

void RBX::Instance::remove()
{

	if (isParentLocked) return;

	for (RBX::Instance* i : *children)
	{
		i->remove();
	}

	if (parent)
	{
		if (std::find(parent->children->begin(), parent->children->end(), this) != parent->children->end())
		{
			parent->children->erase(std::remove(parent->children->begin(), parent->children->end(), this));
		}
	}

	pageTurn->play();

	onRemove();
}

Instance* RBX::Instance::getParent()
{
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
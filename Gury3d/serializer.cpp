#include "serializer.h"

#include "instance.h"
#include "part.h"

#include "workspace.h"

static rapidxml::xml_document<> doc;

bool RBX::Serializer::checkTag()
{
	return !strcmp(doc.first_node()->name(), "roblox");
}

RBX::Instance* fromName(const char* name)
{
	if (!strcmp(name, "Part"))
		return new RBX::PartInstance();
	return nullptr;
}

template<typename V>
void setValue(RBX::Instance* obj, Reflection::Property* property, V value)
{
	return static_cast<const Reflection::PropertyDescriptor < RBX::Instance, V >*>(property)->getset->set(obj, value);
}

template<typename V>
V getValue(RBX::Instance* obj, Reflection::Property* property)
{
	return static_cast<const Reflection::PropertyDescriptor < RBX::Instance, V >*>(property)->getset->get(obj);
}

Vector3 readVector3(rapidxml::xml_node<>* node)
{
	Vector3 v;
	v = Vector3::zero();

	float x, y, z;

	x = std::stof(node->first_node("X")->value());
	y = std::stof(node->first_node("Y")->value());
	z = std::stof(node->first_node("Z")->value());

	v = Vector3(x, y, z);
	
	return v;
}

CoordinateFrame readCFrame(rapidxml::xml_node<>* node)
{
	CoordinateFrame cf;
	float x, y, z, r00, r01, r02, r10, r11, r12, r20, r21, r22;

	x = std::stof(node->first_node("X")->value());
	y = std::stof(node->first_node("Y")->value());
	z = std::stof(node->first_node("Z")->value());
	r00 = std::stof(node->first_node("R00")->value());
	r01 = std::stof(node->first_node("R01")->value());
	r02 = std::stof(node->first_node("R02")->value());
	r10 = std::stof(node->first_node("R10")->value());
	r11 = std::stof(node->first_node("R11")->value());
	r12 = std::stof(node->first_node("R12")->value());
	r20 = std::stof(node->first_node("R20")->value());
	r21 = std::stof(node->first_node("R21")->value());
	r22 = std::stof(node->first_node("R22")->value());

	y /= 1.2f;
	cf = CoordinateFrame(Matrix3(r00, r01, r02, r10, r11, r12, r20, r21, r22), Vector3(x, y, z));

	return cf;
}

void setProperty(rapidxml::xml_node<>* node, RBX::Instance* instance, std::string propertyType, std::string propertyValue, std::string propertyName)
{
	Reflection::Property* property;
	property = instance->properties->property(propertyName);

	if (!property)
		return;

	if (propertyType == "Vector3")
		setValue<Vector3>(instance, property, readVector3(node));

	if (propertyType == "CoordinateFrame")
		setValue<CoordinateFrame>(instance, property, readCFrame(node));

	if (propertyType == "string")
		setValue<std::string>(instance, property, propertyValue);

	if (propertyType == "token" || propertyType == "int")
		setValue<int>(instance, property, std::stoi(propertyValue));

	if(propertyType == "float")
		setValue<float>(instance, property, std::stof(propertyValue));

	if (propertyType == "bool")
		setValue<bool>(instance, property, propertyValue == "true");
}

void iteratePropertiesNode(rapidxml::xml_node<>* properties, RBX::Instance* instance)
{
	for (rapidxml::xml_node<>* prop = properties->first_node(); prop; prop = prop->next_sibling())
	{
		std::string propertyType;
		std::string propertyValue;
		std::string propertyName;

		propertyType = prop->name();
		propertyValue = prop->value();

		propertyName = prop->first_attribute()->value();

		setProperty(prop, instance, propertyType, propertyValue, propertyName);
	}
}

void iterateNode(rapidxml::xml_node<>* scanNode)
{
	for (rapidxml::xml_node<>* node = scanNode->first_node(); node; node = node->next_sibling())
	{
		if (!strcmp(node->name(), "Item"))
		{
			rapidxml::xml_attribute<>* classAttr = node->first_attribute("class");
			std::string className = classAttr->value();

			RBX::Instance* inst = fromName(className.c_str());

			if (inst)
			{
				rapidxml::xml_node<>* properties = node->first_node("Properties");
				if (properties)
				{
					iteratePropertiesNode(properties, inst);
				}
				inst->setParent(RBX::Workspace::singleton());
				printf("Serializer::serialized '%s'\n", inst->getName().c_str());
			}
		}
		iterateNode(node);
	}
}

void RBX::Serializer::load(const std::string& fileName)
{
	rapidxml::file<> file(fileName.c_str());

	if (file.size() <= 0)
		return;

	doc.parse<0>(file.data());

	if (!checkTag())
		return;

	iterateNode(doc.first_node());

}
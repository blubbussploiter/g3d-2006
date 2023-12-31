#include <thread>
#include <map>

#include "serializer.h"

#include "instance.h"
#include "part.h"

#include "workspace.h"
#include "stdout.h"

#include "classes.h"
rapidxml::xml_document<> doc;

static std::map<std::string, int> xml_tokens =
{
	{ "Vector3", 0},
	{ "CFrame", 1},
	{ "CoordinateFrame", 2},
	{ "Color3", 3},
	{ "string", 4},
	{ "token", 5},
	{ "int", 6},
	{ "float", 7},
	{ "bool", 8}
};

bool RBX::Serializer::checkTag()
{
	return !strcmp(doc.first_node()->name(), "roblox");
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

Color3 readColor3(rapidxml::xml_node<>* node)
{
	Color3 color;
	color = Color3::white();
	return color;
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

	Matrix3 matrix;

	matrix = Matrix3(r00, r01, r02, r10, r11, r12, r20, r21, r22);
	cf = CoordinateFrame(matrix, Vector3(x, y, z));

	return cf;
}

void setProperty(rapidxml::xml_node<>* node, RBX::Instance* instance, std::string propertyType, std::string propertyValue, std::string propertyName)
{

	int token;
	Reflection::Property* property;

	property = instance->properties->property(propertyName);

	if (property && !propertyType.empty())
	{
		if (xml_tokens.find(propertyType) != xml_tokens.end())
		{
			token = xml_tokens[propertyType];
			switch (token)
			{
			case 0: /* vector3 */
			{
				setValue<Vector3>(instance, property, readVector3(node));
				break;
			}
			case 1:
			case 2: /* cframe */
			{
				setValue<CoordinateFrame>(instance, property, readCFrame(node));
				break;
			}
			case 3: /* color3 */
			{
				setValue<Color3>(instance, property, readColor3(node));
				break;
			}
			case 4: /* string */
			{
				setValue<std::string>(instance, property, propertyValue);
				break;
			}
			case 5:/* token */
			case 6:/* int */
			{
				setValue<int>(instance, property, std::stoi(propertyValue));
				break;
			}
			case 7: /* float */
			{
				setValue<float>(instance, property, std::stof(propertyValue));
				break;
			}
			case 8: /* bool */
			{
				setValue<bool>(instance, property, propertyValue == "true");
				break;
			}
			default: break;
			}
		}
	}
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

RBX::Instance* readInstance(rapidxml::xml_node<>* instanceNode)
{
	RBX::Instance* inst = 0;
	if (!strcmp(instanceNode->name(), "Item"))
	{
		rapidxml::xml_attribute<>* classAttr = instanceNode->first_attribute("class");
		if (classAttr)
		{
			std::string className = classAttr->value();
			inst = RBX::fromName(className);

			if (inst)
			{
				rapidxml::xml_node<>* properties = instanceNode->first_node("Properties");
				if (properties)
				{
					iteratePropertiesNode(properties, inst);
				}
			}

		}
	}
	return inst;
}

void iterateNode(rapidxml::xml_node<>* scanNode, RBX::Instance* parent)
{
	for (rapidxml::xml_node<>* node = scanNode->first_node(); node; node = node->next_sibling())
	{
		RBX::Instance* read = readInstance(node);
		if (read)
		{
			if (parent)
			{
				//RBX::StandardOut::print(RBX::MESSAGE_INFO, "iterateNode(), parent = '%s'", parent->getName().c_str());
				read->setParent(parent);
			}
			iterateNode(node, read);
		}
	}
}

void RBX::Serializer::load(std::string fileName)
{
	rapidxml::file<> file(fileName.c_str());

	if (file.size() <= 0)
		return;

	doc.parse<0>(file.data());

	if (!checkTag())
		return;

	iterateNode(doc.first_node(), 0);
	doc.clear();
}
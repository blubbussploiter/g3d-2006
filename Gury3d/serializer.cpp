#include <thread>
#include <map>

#include "serializer.h"

#include "instance.h"
#include "part.h"

#include "workspace.h"
#include "stdout.h"

#include "classes.h"
#include "content.h"

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
	{ "bool", 8},
	{"Content", 9}
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
	rapidxml::xml_node<char>* r, *g, *b;

	r = node->first_node("R");
	g = node->first_node("G");
	b = node->first_node("B");

	if (r &&
		g &&
		b)
	{
		return Color3(std::stof(r->value()), std::stof(g->value()), std::stof(b->value()));
	}
	else
	{
		char* hex = node->value();
		if (hex)
		{
			return Color3::fromARGB(std::stoul(hex));
		}
	}

	return Color3::white();
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

RBX::Content getContent(rapidxml::xml_node<>* node)
{
	rapidxml::xml_node<>* binaryNode, *nullNode;

	binaryNode = node->first_node("binary");
	nullNode = node->first_node("null");

	if (nullNode)
	{
		return RBX::Content();
	}

	if (binaryNode)
	{
		return RBX::Content(1, binaryNode->value());
	}

	/* other, http stuff idk */
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
			case 9: /* Content */
			{
				setValue<RBX::Content>(instance, property, getContent(node));
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

void iterateNode(rapidxml::xml_node<>* scanNode, RBX::Instance* parent, RBX::Instances* instances=0)
{
	for (rapidxml::xml_node<>* node = scanNode->first_node(); node; node = node->next_sibling())
	{
		RBX::Instance* read = readInstance(node);
		if (read)
		{
			if (parent)
			{
				read->setParent(parent);
			}
			if (instances && !parent)
			{
				instances->push_back(read);
			}
			iterateNode(node, read, instances);
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

RBX::Instances* RBX::Serializer::loadInstances(std::string fileName)
{
	rapidxml::file<> file(fileName.c_str());
	RBX::Instances* i = new RBX::Instances();

	if (file.size() <= 0)
		return i;

	doc.parse<0>(file.data());

	if (!checkTag())
		return i;

	iterateNode(doc.first_node(), 0, i);
	doc.clear();

	return i;
}

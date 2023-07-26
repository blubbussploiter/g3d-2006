#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include "instance.h"

namespace RBX
{
	class Referent
	{
	public:
		std::string ref;
		RBX::Instance* i;
		Referent(std::string ref, RBX::Instance* i) : ref(ref), i(i) {}
	};

	class Serializer
	{
	private:
		static bool checkTag();
	public:
		static void load(std::string fileName);
	};
}

#endif
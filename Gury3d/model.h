#include "render_shapes.h"
#include "part.h"

namespace RBX
{

	class ModelInstance : public RBX::Instance
	{
	public:
		RBX::PartInstance* primaryPart;

		void move(Vector3 vect);
		void rotate(Vector3 rot);
		void lookAt(Vector3 vect);
		void breakJoints();

		ModelInstance()
		{
			setClassName("Model");
			setName("Model");
		}

		virtual ~ModelInstance() { breakJoints(); }
	};

	static void setModelTransparency(RBX::ModelInstance* m, float transparency)
	{
		RBX::Instances* children;
		children = m->getChildren();
		for (int i = 0; i < children->size(); i++)
		{
			RBX::Instance* child = children->at(i);
			if (child->getClassName() == "PVInstance")
				((RBX::PVInstance*)(child))->setTransparency(transparency);
		}
	}
}
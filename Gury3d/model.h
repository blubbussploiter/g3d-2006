
#include "render_shapes.h"
#include "controller.h"
#include "part.h"

namespace RBX
{

	class ModelInstance : public RBX::Instance
	{
	private:
		RBX::ControllerTypes controllerType;
		RBX::Controller* controller;
	public:
		RBX::PartInstance* primaryPart;

		static Reflection::PropertyDescriptor<ModelInstance, RBX::ControllerTypes> prop_controllerType;

		void move(Vector3 vect);
		void rotate(Vector3 rot);

		void lookAt(Vector3 vect);

		void breakJoints();
		void buildJoints();

		void setController(int c);
		ControllerTypes getController() { return controllerType; }

		RBX::PartInstance* getPrimaryPartInternal();

		ModelInstance()
		{
			controllerType = ControllerTypes::None;
			controller = 0;
			setClassName("Model");
			setName("Model");
		}

		virtual ~ModelInstance() { breakJoints(); }
	};

	static void setModelTransparency(RBX::ModelInstance* m, float transparency)
	{
		RBX::Instances* children;
		children = m->getChildren();
		for (unsigned int i = 0; i < children->size(); i++)
		{
			RBX::Instance* child = children->at(i);
			if (child->getClassName() == "PVInstance")
				((RBX::PVInstance*)(child))->setTransparency(transparency);
		}
	}
}
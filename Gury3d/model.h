#ifndef MODEL_H
#define MODEL_H

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

		static const Reflection::PropertyDescriptor<ModelInstance, RBX::ControllerTypes> prop_controllerType;

		void move(Vector3 vect);
		void rotate(Vector3 rot);

		void lookAt(Vector3 vect);

		void breakJoints();
		void buildJoints();

		void createController(); /* recursive */
		void makeController(); /* non recursive */

		void setController(int c);
		void setPrimaryPartCFrame(CoordinateFrame cframe);

		ControllerTypes getController() { return controllerType; }
		RBX::PartInstance* getPrimaryPartInternal();

		RBX::PartInstance* getPrimaryPart() { if (!primaryPart) primaryPart = getPrimaryPartInternal(); return primaryPart; }

		ModelInstance()
		{
			controllerType = ControllerTypes::None;
			controller = nullptr;
			primaryPart = nullptr;
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

#endif
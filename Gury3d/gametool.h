#ifndef GAMETOOL_H
#define GAMETOOL_H

#include "hopperbin.h"
#include "render_shapes.h"

namespace RBX
{
	class GameTool : public RBX::HopperBin
	{
	private:
		RBX::PVInstance* dragged;
		Vector3 lastPosition;
		btVector3 lastAngularVelocity, lastLinearVelocity;
	public:
		void update(RenderDevice* rd, UserInput* ui);
		void deactivate();
		void activate();
		GameTool()
		{
			binType = GAMETOOL_BIN;
		}
	};
}

#endif
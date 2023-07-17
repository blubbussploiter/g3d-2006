
#ifndef PART_H
#define PART_H

#include "render_shapes.h"
#include "brickcolor.h"

namespace RBX
{

		class PartInstance : public RBX::PVInstance
		{
		private:
			BrickColor* brickColor;
		public:

			static Reflection::PropertyDescriptor<PartInstance, Vector3> prop_sz;
			static Reflection::PropertyDescriptor<PartInstance, Vector3> prop_pos;
			static Reflection::PropertyDescriptor<PartInstance, CoordinateFrame> prop_cframe;
			static Reflection::PropertyDescriptor<PartInstance, BrickColor> prop_brickcolor1;
			static Reflection::PropertyDescriptor<PartInstance, Color3> prop_color3;
			static Reflection::PropertyDescriptor<PartInstance, int> prop_brickcolor;
			static Reflection::PropertyDescriptor<PartInstance, int> prop_shape;
			static Reflection::PropertyDescriptor<PartInstance, bool> prop_anchored;
			static Reflection::PropertyDescriptor<PartInstance, float> prop_transparency;
			static Reflection::PropertyDescriptor<PartInstance, int> prop_frontSurface;
			static Reflection::PropertyDescriptor<PartInstance, int> prop_backSurface;
			static Reflection::PropertyDescriptor<PartInstance, int> prop_rightSurface;
			static Reflection::PropertyDescriptor<PartInstance, int> prop_leftSurface;
			static Reflection::PropertyDescriptor<PartInstance, int> prop_topSurface;
			static Reflection::PropertyDescriptor<PartInstance, int> prop_bottomSurface;

			void setShape(int s) { shape = s; }
			int getShape() { return shape; }

			//int 

			void setBrickColor(int bc) { brickColor = BrickColor::fromNumber(bc); }
			int getBrickColor() { return brickColor->getNumber(); }

			void setFace(FACES f, SURFACES s);

			virtual void render(RenderDevice* rd);
			PartInstance() { shape = shape::part; brickColor = BrickColor::fromNumber(0); }
			virtual ~PartInstance() {}
		};
}

#endif
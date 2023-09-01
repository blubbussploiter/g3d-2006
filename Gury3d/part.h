
#ifndef PART_H
#define PART_H

#include "pvinstance.h"
#include "brickcolor.h"

#include "enum.h"

namespace RBX
{

		class PartInstance : public RBX::PVInstance
		{
		private:
			BrickColor* brickColor;
		public:

			static Reflection::PropertyDescriptor<PartInstance, BrickColor> prop_brickcolor1;
			static Reflection::PropertyDescriptor<PartInstance, int> prop_brickcolor;

			void setBrickColor(int bc) {
				Color3* b;
				brickColor = BrickColor::fromNumber(bc);
				b = brickColor->getColor();
				color = Color3(b->r, b->g, b->b);
			}
			int getBrickColor() { return brickColor->getNumber(); }

			virtual void render(RenderDevice* rd);
			PartInstance() { shape = Shape::part; brickColor = BrickColor::fromNumber(0); }
			virtual ~PartInstance() {}
		};
}

#endif
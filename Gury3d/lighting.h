#include <G3DAll.h>
#include "instance.h"

namespace RBX
{
	class Lighting : public RBX::Instance
	{
	private:
		double latitude;

		Color3 top_ambient,bottom_ambient, spotLight_color;
		Color4 clear_color;

		LightingParameters* params;
	public:

		static const Reflection::PropertyDescriptor<RBX::Lighting, Color3> prop_topAmbient;
		static const Reflection::PropertyDescriptor<RBX::Lighting, Color3> prop_bottomAmbient;
		static const Reflection::PropertyDescriptor<RBX::Lighting, Color3> prop_spotLight;

		void begin(RenderDevice* renderDevice, float shininess);
		void end(RenderDevice* renderDevice);

		void setSpotLight(Color3 c) { spotLight_color = c; }
		void setTopAmbient(Color3 c) { top_ambient = c; }
		void setBottomAmbient(Color3 c) { bottom_ambient = c; }

		Color3 getSpotLight() { return spotLight_color; }
		Color3 getTopAmbient() { return top_ambient; }
		Color3 getBottomAmbient() { return bottom_ambient; }

		LightingParameters getParameters();
		static Lighting* singleton();

		Lighting()
		{
			latitude = 41.7;

			clear_color = Color4(1.0f, 1.0f,1.0f);
			top_ambient = Color3(0.81960785f, 0.81568629f, 0.8509804f);
			bottom_ambient = Color3(0.47843137f, 0.52549022f, 0.47058824f);
			spotLight_color = Color3(0.59607846f, 0.53725493f, 0.40000001f);

			setName("Lighting");
			setClassName("Lighting");
		}
	};
}
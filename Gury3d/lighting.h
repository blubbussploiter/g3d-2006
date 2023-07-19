#include <G3DAll.h>

namespace RBX
{
	class Lighting
	{
	private:
		double latitude;

		Color3 top_ambient;
		Color3  bottom_ambient;
		Color3  base_ambient;
		Color3  clear_color;
		Color3  spot;
	public:

		void begin(RenderDevice* renderDevice, Vector3 lightPosition, float shininess);
		void end(RenderDevice* renderDevice);
		static Lighting* singleton();

		Lighting() :
			latitude(41.7),
			top_ambient(0.819608f, 0.815686f, 0.85098f),
			bottom_ambient(0.478431f, 0.52549f, 0.470588f),
			base_ambient(Color3::gray()),
			clear_color(Color3::black()),
			spot(0.75f, 0.75f, 0.75f)
		{
		}
	};
}
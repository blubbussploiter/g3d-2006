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

		Lighting()
		{
			latitude = 41.7;

			spot = Color3(0.75, 0.75, 0.75);

			clear_color = Color3::black();
			top_ambient = Color3(0.819608, 0.815686, 0.85098);
			bottom_ambient = Color3(0.478431, 0.52549, 0.470588);
			base_ambient = Color3::gray();
		}
	};
}
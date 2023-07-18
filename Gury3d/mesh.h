
#include "render_shapes.h"

namespace RBX
{
	namespace Render
	{
		class Mesh : public RBX::PVInstance
		{
		private:
			std::string meshPath;
			std::vector<Vector3> vertices;
			std::vector<Vector3> normals;
			std::vector<Vector3> uvs;
			Vector3 mesh_scale;
			int faces;
		public:
			void fromFile(std::string path);
			void setMeshScale(Vector3 scale) { mesh_scale = scale; }
			void renderDecals();
			void render(RenderDevice* d);
			void renderFace(RBX::FACES face, bool isAlpha=0, bool isDrawingDeca=0);
			Mesh() : mesh_scale(1.25f, 1.25f, 1.25f)
			{
				setClassName("PVMesh");
				setName("PVMesh");
			}
			virtual ~Mesh() {}
		};
	}
}

#ifndef MESH_H
#define MESH_H

#include "pvenums.h"
#include "render_shapes.h"

namespace RBX
{
	namespace Render
	{

		enum MeshType
		{
			Head,
			Torso,
			Wedge,
			Sphere,
			Cylinder,
			Filemesh,
			Brick
		};

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
			void fromMeshType(MeshType types);
			void setMeshScale(Vector3 scale) { mesh_scale = scale; }
			void renderDecals();
			void render(RenderDevice* d);
			void renderFace(RBX::NormalId face, bool isAlpha=0, bool isDrawingDeca=0);
			Mesh()
			{
				setClassName("PVMesh");
				setName("PVMesh");
				mesh_scale = Vector3(1.25, 1.25, 1.25);
			}
			virtual ~Mesh() {}
		};
	}
}

#endif
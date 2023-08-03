
#ifndef MESH_H
#define MESH_H

#include "pvenums.h"
#include "render_shapes.h"

#include "content.h"

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
			Content meshId;
			std::vector<Vector3> vertices;
			std::vector<Vector3> normals;
			std::vector<Vector3> uvs;
			Vector3 mesh_scale;
			MeshType meshType;
			int faces;
		public:

			static const Reflection::PropertyDescriptor<Mesh, MeshType> prop_meshType;
			static const Reflection::PropertyDescriptor<Mesh, Vector3> prop_meshScale;
			static const Reflection::PropertyDescriptor<Mesh, Content> prop_meshId;

			MeshType getMeshType() { return meshType; }
			void setMeshType(int m) 
			{
				meshType = (MeshType)m;
				switch (meshType)
				{
					case Head:
					{
						fromFile(GetFileInPath("/content/font/head.mesh"));
						break;
					}
					default: break;
				}
			}

			void setMeshScale(Vector3 scale) { mesh_scale = scale * 1.7f; }
			Vector3 getMeshScale() { return mesh_scale; }

			void fromFile(std::string path);
			void fromMeshType(MeshType types);

			void setMeshId(Content meshId);
			Content getMeshId() { return meshId; }

			void renderDecals();
			void render(RenderDevice* d);

			void renderFace(RBX::NormalId face, bool isAlpha=0, bool isDrawingDeca=0);
			Mesh()
			{
				setClassName("SpecialMesh");
				setName("SpecialMesh");
				mesh_scale = Vector3::one();
			}
			virtual ~Mesh() {}
		};
	}
}

#endif
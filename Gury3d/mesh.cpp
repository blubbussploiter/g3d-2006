#include "mesh.h"
#include "lighting.h"

const Reflection::PropertyDescriptor<RBX::Render::SpecialMesh, RBX::Render::MeshType> RBX::Render::SpecialMesh::prop_meshType("MeshType", Reflection::Types::TYPE_Number, &RBX::Render::SpecialMesh::getMeshType, &RBX::Render::SpecialMesh::setMeshType, RBX::Render::SpecialMesh::properties);
const Reflection::PropertyDescriptor<RBX::Render::SpecialMesh, Vector3> RBX::Render::SpecialMesh::prop_meshScale("Scale", Reflection::Types::TYPE_Vector3, &RBX::Render::SpecialMesh::getMeshScale, &RBX::Render::SpecialMesh::setMeshScale, RBX::Render::SpecialMesh::properties);
const Reflection::PropertyDescriptor<RBX::Render::SpecialMesh, RBX::Content> RBX::Render::SpecialMesh::prop_meshId("MeshId", Reflection::Types::TYPE_Content, &RBX::Render::SpecialMesh::getMeshId, &RBX::Render::SpecialMesh::setMeshId, RBX::Render::SpecialMesh::properties);

void RBX::Render::SpecialMesh::fromFile(std::string path)
{
	FILE* f = fopen(path.c_str(), "r");

	float vx, vy, vz,
		nx, ny, nz,
		ux, uy, uz;

	fscanf(f, "version 1.00\n");
	fscanf(f, "%d\n", &faces);

	for (int i = 0; i < faces; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fscanf(f, "[%f,%f,%f][%f,%f,%f][%f,%f,%f]", &vx, &vy, &vz, &nx, &ny, &nz, &ux, &uy, &uz);
			vx *= 0.5;
			vy *= 0.5;
			vz *= 0.5;
			uy = 1.0 - uy;
			vertices.push_back(Vector3(vx, vy, vz));
			normals.push_back(Vector3(nx, ny, nz));
			uvs.push_back(Vector3(ux, uy, uz));
		}
	}
	
	fclose(f);

	num_faces = faces * 3;
	meshId = Content(path);
}

void RBX::Render::SpecialMesh::fromMeshType(MeshType types)
{
	setMeshType(types);
}

void RBX::Render::SpecialMesh::setMeshId(Content SpecialMeshId)
{
	std::string contentPath;

	RBX::ContentProvider::singleton()->downloadContent(SpecialMeshId, contentPath);

	if (contentPath.empty()) return;

	vertices.clear();
	normals.clear();
	uvs.clear();

	fromFile(contentPath);

	RBX::ContentProvider::singleton()->cleanupContent(SpecialMeshId);
}

void RBX::Render::SpecialMesh::renderDecals(RenderDevice* rd)
{
	RBX::Instances* children;
	children = getParent()->getChildren();
	for (unsigned int i = 0; i < children->size(); i++)
	{
		RBX::Instance* child = children->at(i);
		if (child->getClassName() == "Decal")
		{
			RBX::Decal* d = (RBX::Decal*)child;
			d->render(rd, this);
		}
	}
}

void RBX::Render::SpecialMesh::render(RenderDevice* d)
{
	switch (meshType)
	{
		case Head:
		{
			buildHeadMesh(mesh_scale);
			break;
		}
		default: break;
	}
}

void RBX::Render::SpecialMesh::renderFace(RBX::NormalId face, bool isDrawingDecal)
{
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < num_faces; ++i) 
	{
		glNormal(normals[i]);
		glTexCoord(uvs[i]);
		glVertex(vertices[i]);
	}

	glEnd();
}

void RBX::Render::buildHeadMesh(Vector3 size)
{

}

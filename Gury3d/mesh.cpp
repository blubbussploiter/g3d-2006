#include "mesh.h"
#include "lighting.h"

const Reflection::PropertyDescriptor<RBX::Render::Mesh, RBX::Render::MeshType> RBX::Render::Mesh::prop_meshType("MeshType", Reflection::Types::TYPE_Number, &RBX::Render::Mesh::getMeshType, &RBX::Render::Mesh::setMeshType, RBX::Render::Mesh::properties);
const Reflection::PropertyDescriptor<RBX::Render::Mesh, Vector3> RBX::Render::Mesh::prop_meshScale("Scale", Reflection::Types::TYPE_Vector3, &RBX::Render::Mesh::getMeshScale, &RBX::Render::Mesh::setMeshScale, RBX::Render::Mesh::properties);
const Reflection::PropertyDescriptor<RBX::Render::Mesh, RBX::Content> RBX::Render::Mesh::prop_meshId("MeshId", Reflection::Types::TYPE_Content, &RBX::Render::Mesh::getMeshId, &RBX::Render::Mesh::setMeshId, RBX::Render::Mesh::properties);

void RBX::Render::Mesh::fromFile(std::string path)
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
	meshId = Content(path);
}

void RBX::Render::Mesh::fromMeshType(MeshType types)
{
	setMeshType(types);
}

void RBX::Render::Mesh::setMeshId(Content meshId)
{
	std::string contentPath;

	RBX::ContentProvider::singleton()->downloadContent(meshId, contentPath);

	if (contentPath.empty()) return;

	vertices.clear();
	normals.clear();
	uvs.clear();

	fromFile(contentPath);

	RBX::ContentProvider::singleton()->cleanupContent(meshId);
}

void RBX::Render::Mesh::renderDecals()
{
	RBX::Instances* children;
	children = getParent()->getChildren();
	for (unsigned int i = 0; i < children->size(); i++)
	{
		RBX::Instance* child = children->at(i);
		if (child->getClassName() == "Decal")
		{
			RBX::Decal* d = (RBX::Decal*)child;
			d->render(this);
		}
	}
}

void RBX::Render::Mesh::render(RenderDevice* d)
{

	RBX::PVInstance* parent;
	CoordinateFrame cframe;

	if (getParent() && getParent()->getClassName() != "PVInstance")
		return;

	parent = (RBX::PVInstance*)getParent();
	cframe = parent->getCFrame();

	RBX::Lighting::singleton()->begin(d, 25.0f);
	d->setObjectToWorldMatrix(cframe);

	renderFace(RBX::FRONT);
	renderDecals();

	RBX::Lighting::singleton()->end(d);
}

void RBX::Render::Mesh::renderFace(RBX::NormalId face, bool isAlpha, bool isDrawingDecal)
{
	RBX::PVInstance* parent;
	Color3 color;

	float transparency;
	float alpha = 1;

	parent = (RBX::PVInstance*)getParent();
	color = parent->getColor();

	transparency = parent->getTransparency();

	glEnable(GL_DEPTH_TEST);

	if (transparency <= 1)
	{
		if (!isAlpha)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			alpha = (1 - transparency);
		}
	}

	glColor4f(color.r, color.g, color.b, alpha);

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < faces * 3; ++i) 
	{
		glNormal(normals[i] * mesh_scale);
		glTexCoord(uvs[i]);
		glVertex(vertices[i] * mesh_scale);
	}

	glEnd();

	if (transparency <= 1)
	{
		if (!isAlpha)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glDisable(GL_BLEND);
		}
	}

	glDisable(GL_DEPTH_TEST);

}
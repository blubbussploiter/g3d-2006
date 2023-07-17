#include "mesh.h"
#include "lighting.h"

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
}

void RBX::Render::Mesh::renderDecals()
{
	RBX::Instances* children;
	children = getParent()->getChildren();
	for (int i = 0; i < children->size(); i++)
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

	RBX::Lighting::singleton()->begin(d, cframe.translation, 25);
	d->setObjectToWorldMatrix(cframe);

	renderFace(RBX::FRONT);
	renderDecals();

	RBX::Lighting::singleton()->end(d);
}

void RBX::Render::Mesh::renderFace(RBX::FACES face, bool isAlpha, bool isDrawingDecal)
{
	RBX::PVInstance* parent;
	Color3 color;

	float transparency;
	float alpha = 1;

	Vector3 scale;

	parent = (RBX::PVInstance*)getParent();
	color = parent->getColor();

	transparency = parent->getTransparency();
	scale = mesh_scale;

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
		glNormal(normals[i] * scale);

		glTexCoord(uvs[i]);
		glVertex(vertices[i] * scale);
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
#include "ModelObject.h"
#include "objloader.h"
#ifdef USE_D3D9
#include "D3d9Renderer.h"
#else
#include "OpenGLRenderer.h"
#include "LegacyOpenGLRenderer.h"
#endif

ModelObject::ModelObject(const char* modelPath) : PhysicalObject()
{
#ifdef USE_D3D9
	renderer = new D3d9Renderer(this);
#else
	if (GLEW_VERSION_1_5)
	{
		renderer = new OpenGLRenderer(this);
	}
	else
	{
		renderer = new LegacyOpenGLRenderer(this);
	}
#endif
	this->modelPath = (char*)modelPath;
	LoadModel(modelPath);
	BuildRigidBody();
	renderer->BuildBuffers(
		&vertices[0], &normals[0], NULL, NULL,
		vertexCount, 0, 0
	);
}

ModelObject::~ModelObject(void)
{
	delete renderer;
}

void ModelObject::BuildShape()
{
	string key = "convex_hull " + string(modelPath);
	if (CollisionShapes.find(key) == CollisionShapes.end())
	{
		btScalar* vertices = new btScalar[vertexCount * 3];
		for (size_t i = 0; i < vertexCount; i++)
		{
			vertices[3 * i + 0] = this->vertices[i].x * PHYSICS_WORLD_SCALE;
			vertices[3 * i + 1] = this->vertices[i].y * PHYSICS_WORLD_SCALE;
			vertices[3 * i + 2] = this->vertices[i].z * PHYSICS_WORLD_SCALE;
		}
		CollisionShapes[key] = new btConvexHullShape(&vertices[0], vertexCount, 3 * sizeof(btScalar));
		delete[] vertices;
	}
	collisionShape = CollisionShapes[key];
}

#ifdef USE_OPENGL
void ModelObject::SetProgramId(GLuint programId)
{
	renderer->SetProgramId(programId);
}
#endif

void ModelObject::LoadModel(const char* modelPath)
{
	loadOBJ(modelPath, vertices, uvs, normals);
	vertexCount = vertices.size();
}

void ModelObject::Draw(Camera* camera)
{
	renderer->Draw(camera);
}
#ifdef USE_D3D9
#include "D3d9World.h"
#else
#include "OpenGLWorld.h"
#endif
#include "BoxObject.h"

using namespace std;

int main()
{
	int nBox;
	printf("nBox: ");
	scanf("%d", &nBox);

	printf("scale: ");
	scanf("%f", &PHYSICS_WORLD_SCALE);

#ifdef USE_D3D9
	D3d9World world;
#else
	OpenGLWorld world;
#endif
    world.Init();
	

	BoxObject* box = new BoxObject[nBox];

    RDRVEC3 boxmin(-0.1f, -0.1f, -0.1f);
	RDRVEC3 boxmax( 0.1f,  0.1f,  0.1f);

	for (int i = 0; i < nBox; i++)
	{
#ifdef USE_OPENGL
		box[i].SetProgramId(world.programId);
#endif
		box[i].SetMinMax(boxmin, boxmax);
		box[i].SetPosition(RDRVEC3(-1.0f + i*0.1f, 10.0f + i*1.0f, 5.0f), true);
		box[i].SetEuler(RDRVEC3(44.0f, 10.0f, 30.0f), true);
		box[i].BuildWorld();
		world.DrawableObjects.push_back(&box[i]);
		world.PhysicalObjects.push_back(&box[i]);
	}


	ModelObject obj("../suzanne.obj");
#ifdef USE_OPENGL
	obj.SetProgramId(world.programId);
#endif
	obj.SetPosition(RDRVEC3(-0.7, 1, 4.55), true);
	obj.SetEuler(RDRVEC3(0, 180, 0), true);
	obj.BuildWorld();
	world.DrawableObjects.push_back(&obj);
	world.PhysicalObjects.push_back(&obj);


	Camera& camera = world.camera;
	camera.SetPosition(RDRVEC3(0.0f, 1.0f, -3.0f), true);
	//camera.SetEuler(glm::vec3(0, 0, 0), true);
	camera.BuildWorld();

	world.Render();

	delete[] box;

	return 0;
}

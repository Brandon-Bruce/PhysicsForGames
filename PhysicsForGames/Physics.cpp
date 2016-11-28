#include "Physics.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

#include "glm/ext.hpp"
#include "glm/gtc/quaternion.hpp"

#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "SpringJoint.h"
#include "Box.h"

#include "CollisionChecker.h"
#include "MyCollisionCallback.h"

#include "Ragdoll.h"
#include "Fluid.h"

#define Assert(val) if (val){}else{ *((char*)0) = 0;}
#define ArrayCount(val) (sizeof(val)/sizeof(val[0]))

bool Physics::startup()
{
    if (Application::startup() == false)
    {
        return false;
    }
	
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
	Gizmos::create(150000U, 150000U);

    m_camera = FlyCamera(1280.0f / 720.0f, 10.0f);
    m_camera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
    m_camera.sensitivity = 3;

	m_renderer = new Renderer();

	// Initiate gravity
	gravity = glm::vec3(0, -1.f, 0);

	//Make Actors
	Sphere* sphere1;
	sphere1 = new Sphere(glm::vec4(1, 0, 1, 1), glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 5.0f, 1, true, 1);
	actors.push_back(sphere1);

	Sphere* sphere2;
	sphere2 = new Sphere(glm::vec4(1, 0, 1, 1), glm::vec3(10, 10, 0), glm::vec3(0, 0, 0), glm::vec3(-3, 0, 0), 5.0f, 1, false, 1);
	actors.push_back(sphere2);

	Sphere* sphere3;
	sphere3 = new Sphere(glm::vec4(1, 1, 0, 1), glm::vec3(-10, 30, 0), glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), 5.0f, 1, false, 1);
	actors.push_back(sphere3);

	Box* box1;
	box1 = new Box(glm::vec4(1, 0, 1, 0), glm::vec3(-10, 6, 0), glm::vec3(0, 0, 0), 5.0f, 1, true, glm::vec3(2, 2, 2));
	actors.push_back(box1);

	Box* box2;
	box2 = new Box(glm::vec4(1, 0, 1, 0), glm::vec3(-7.5, 20, 0), glm::vec3(-10, 1, 10), 5.0f, 1, false, glm::vec3(2, 2, 2));
	actors.push_back(box2);

	SpringJoint* spring;
	spring = new SpringJoint(glm::vec4(1, 1, 1, 1), sphere1, sphere2, 10.f, 0.f);
	actors.push_back(spring);
	spring = nullptr;

	//down
	Plane* plane;
	plane = new Plane(glm::vec4(1, 0, 0, 1), glm::vec3(0.f, 1.f, 0.f), 2);
	actors.push_back(plane);
	plane = nullptr;

	//right
	plane = new Plane(glm::vec4(0, 1, 0, 1), glm::vec3(1.f, 0.f, 0.f), -20);
	actors.push_back(plane);
	plane = nullptr;
	
	//left
	plane = new Plane(glm::vec4(0, 1, 0, 1), glm::vec3(-1.f, 0.f, 0.f), -20);
	actors.push_back(plane);
	plane = nullptr;


	//back
	plane = new Plane(glm::vec4(0, 0, 1, 1), glm::vec3(0.f, 0.f, -1.f), -20);
	actors.push_back(plane);
	plane = nullptr;

	//forward
	plane = new Plane(glm::vec4(0, 0, 1, 1), glm::vec3(0.f, 0.f, 1.f), -20);
	actors.push_back(plane);
	plane = nullptr;

	/**************
	**PhysX stuff**
	**************/
	SetUpPhysX();
	CreateDefaultScene();
	SetUpIntroToPhysX();

	SetUpController();
	MakeRagDoll();
	m_particleEmmiter = Fluid::Create(m_physics, m_physicsScene);

	cooldown = 0;
    return true;
}

void Physics::SetUpPhysX()
{
	m_physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_defaultAllocator,
											m_defaultErrorCallback);
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_physicsFoundation,
								PxTolerancesScale());
	PxInitExtensions(*m_physics);
	m_physicsMaterial = m_physics->createMaterial(1, 1, 0);
	m_physicsCooker = PxCreateCooking(PX_PHYSICS_VERSION, *m_physicsFoundation,
										PxCookingParams(PxTolerancesScale()));
}

void Physics::CreateDefaultScene()
{
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -9.807f, 0);
	sceneDesc.filterShader = &FilterGroup::myFliterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(8);
	PxScene* result = m_physics->createScene(sceneDesc);
	
	PxSimulationEventCallback* collisionCallBack = new MyCollisionCallback();
	result->setSimulationEventCallback(collisionCallBack);

	m_physicsScene = result;
}

void Physics::SetUpIntroToPhysX()
{
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi,
													PxVec3(0.0f, 0.0f, 1.0f)));

	PxRigidStatic* plane = PxCreateStatic(*m_physics, pose, PxPlaneGeometry(),
		*m_physicsMaterial);

	plane->setName("Plane1");
	FilterGroup::setUpFiltering(plane, FilterGroup::GROUND, FilterGroup::BOX | FilterGroup::BULLET | FilterGroup::CHARACTER);

	//Add it to the physX scene
	m_physicsScene->addActor(*plane);

	//add a box1
	PxBoxGeometry box(2, 2, 2);
	PxTransform trasnform(PxVec3(0, 5, -10));
	PxRigidStatic* staticActor = PxCreateStatic(*m_physics, trasnform, box,
													*m_physicsMaterial);
	staticActor->setName("Box1");
	FilterGroup::setUpFiltering(staticActor, FilterGroup::BOX, FilterGroup::GROUND | FilterGroup::CHARACTER);

	//add to PhysX scene
	m_physicsScene->addActor(*staticActor);

	//box 2
	box;
	trasnform = PxTransform(PxVec3(0, 20, -10));
	staticActor = PxCreateStatic(*m_physics, trasnform, box,
		*m_physicsMaterial);
	staticActor->setName("Box2");
	FilterGroup::setUpFiltering(staticActor, FilterGroup::BOX, FilterGroup::GROUND | FilterGroup::CHARACTER);

	SetShapeAsTrigger(staticActor);

	//add to PhysX scene
	m_physicsScene->addActor(*staticActor);

	//add a box3
	float density = 1.0f;
	box = PxBoxGeometry(2, 2, 2);
	trasnform = PxTransform(PxVec3(0, 30, -10));
	PxRigidDynamic* dynamicActor = PxCreateDynamic(*m_physics, trasnform, box,
		*m_physicsMaterial, density);

	dynamicActor->setName("Box3");
	FilterGroup::setUpFiltering(dynamicActor, FilterGroup::BOX, FilterGroup::GROUND | FilterGroup::BULLET | FilterGroup::CHARACTER);

	//add to PhysX scene
	m_physicsScene->addActor(*dynamicActor);

	//add a box4
	density = 1.0f;
	box = PxBoxGeometry(2, 2, 2);
	trasnform = PxTransform(PxVec3(0, 40, -9));
	dynamicActor = PxCreateDynamic(*m_physics, trasnform, box,
		*m_physicsMaterial, density);

	dynamicActor->setName("Box");
	FilterGroup::setUpFiltering(dynamicActor, FilterGroup::BOX, FilterGroup::GROUND | FilterGroup::BULLET | FilterGroup::CHARACTER);

	//add to PhysX scene
	m_physicsScene->addActor(*dynamicActor);


	//Test area for fluid
	//Left
	box = PxBoxGeometry(2, 2, 7);
	trasnform = PxTransform(PxVec3(20, 2, 0));
	staticActor = PxCreateStatic(*m_physics, trasnform, box,
		*m_physicsMaterial);

	staticActor->setName("Left");
	FilterGroup::setUpFiltering(staticActor, FilterGroup::BOX, FilterGroup::GROUND | FilterGroup::BULLET | FilterGroup::CHARACTER);

	//add to PhysX scene
	m_physicsScene->addActor(*staticActor);

	//Right
	box = PxBoxGeometry(2, 2, 7);
	trasnform = PxTransform(PxVec3(30, 2, 0));
	staticActor = PxCreateStatic(*m_physics, trasnform, box,
		*m_physicsMaterial);

	staticActor->setName("Right");
	FilterGroup::setUpFiltering(staticActor, FilterGroup::BOX, FilterGroup::GROUND | FilterGroup::BULLET | FilterGroup::CHARACTER);

	//add to PhysX scene
	m_physicsScene->addActor(*staticActor);

	//Front
	box = PxBoxGeometry(5, 2, 2);
	trasnform = PxTransform(PxVec3(25, 2, -5));
	staticActor = PxCreateStatic(*m_physics, trasnform, box,
		*m_physicsMaterial);

	staticActor->setName("Front");
	FilterGroup::setUpFiltering(staticActor, FilterGroup::BOX, FilterGroup::GROUND | FilterGroup::BULLET | FilterGroup::CHARACTER);
	
	//add to PhysX scene
	m_physicsScene->addActor(*staticActor);

	//Back
	box = PxBoxGeometry(5, 2, 2);
	trasnform = PxTransform(PxVec3(25, 2, 5));
	staticActor = PxCreateStatic(*m_physics, trasnform, box,
		*m_physicsMaterial);

	staticActor->setName("Back");
	FilterGroup::setUpFiltering(staticActor, FilterGroup::BOX, FilterGroup::GROUND | FilterGroup::BULLET | FilterGroup::CHARACTER);

	//add to PhysX scene
	m_physicsScene->addActor(*staticActor);

	// check if PvdConnection manager is available on this platform
	if (m_physics->getPvdConnectionManager() == NULL)
		return;
	// setup connection parameters
	const char* pvd_host_ip = "127.0.0.1";
	// IP of the PC which is running PVD
	int port = 5425;
	// TCP port to connect to, where PVD is listening
	unsigned int timeout = 100;
	// timeout in milliseconds to wait for PVD to respond, 
	//consoles and remote PCs need a higher timeout.
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
	// and now try to connectPxVisualDebuggerExt
	auto theConnection = PxVisualDebuggerExt::createConnection(
		m_physics->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);



}

void Physics::SetUpController()
{
	_characterYVelocity = 0;
	_playerGravity = -0.5f;
	_characterRotation = 0;
	myHitReport = new ControllerHitReport();
	PxControllerManager* characterManager = PxCreateControllerManager(*m_physicsScene);
	//describe controller
	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = m_physicsMaterial;
	desc.reportCallback = myHitReport;
	desc.density = 10;
	//create layout controller
	playerController = characterManager->createController(desc);
	playerController->setPosition(PxExtendedVec3(5, 5, 5));
	myHitReport->clearPlayerContactNormal(); //initialize contact normal

	PxRigidDynamic* actor = playerController->getActor();
	actor->setName("Character Controller");
	FilterGroup::setUpFiltering(actor, FilterGroup::CHARACTER, FilterGroup::GROUND | FilterGroup::BULLET | FilterGroup::BOX);
	m_physicsScene->addActor(*actor);
}

void Physics::MakeRagDoll()
{
	PxArticulation* ragdoll;
	ragdoll = Ragdoll::MakeRagDoll(m_physics, Ragdoll::ragdollData,
		PxTransform(PxVec3(-20, 5, 0)), 0.1f, m_physicsMaterial);
	m_physicsScene->addArticulation(*ragdoll);
	ragdoll->setName("RagDoll");
	ragdolls.push_back(ragdoll);
}

void Physics::SetShapeAsTrigger(PxRigidActor* actor)
{
	PxRigidStatic* staticActor = actor->is<PxRigidStatic>();
	assert(staticActor);

	const PxU32 numShapes = staticActor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)*numShapes, 16);
	staticActor->getShapes(shapes, numShapes);

	for (PxU32 i = 0; i < numShapes; ++i)
	{
		shapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
}

void Physics::shutdown()
{
	for (unsigned int i = 0; i < actors.size(); ++i)
		delete actors[i];

	delete m_renderer;
    Gizmos::destroy();
    Application::shutdown();

	m_physicsScene->release();
	m_physicsFoundation->release();
	m_physics->release();
}

bool Physics::update()
{
    if (Application::update() == false)
    {
        return false;
    }

    Gizmos::clear();

    float dt = (float)glfwGetTime();
    m_delta_time = dt;
    glfwSetTime(0.0);

	// Update Actors
	for (unsigned int i = 0; i < actors.size(); ++i)
		actors[i]->Update(gravity, m_delta_time);

	// Add gizmos fro objects
	for (unsigned int i = 0; i < actors.size(); ++i)
		actors[i]->MakeGizmo();

	//check for collisions
	CheckForCollisions();

    vec4 white(1);
    vec4 black(0, 0, 0, 1);

    for (int i = 0; i <= 20; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, -0.01, -10), vec3(-10 + i, -0.01, 10),
            i == 10 ? white : black);
        Gizmos::addLine(vec3(-10, -0.01, -10 + i), vec3(10, -0.01, -10 + i),
            i == 10 ? white : black);
    }

	/***************
	**PhysX update**
	***************/
	if(m_delta_time > 0)
	{
		m_physicsScene->simulate(dt > 0.033f ? 0.033f : dt);
		while (m_physicsScene->fetchResults() == false)
		{

		}
	}

    m_camera.update(1.0f / 60.0f);

	if (glfwGetKey(m_window, GLFW_KEY_SPACE))
	{
		GunFire();
	}

	if (m_particleEmmiter)
	{
		m_particleEmmiter->update(m_delta_time);
		//render particles
		m_particleEmmiter->renderParticles();
	}

	CharacterController();

	renderGizmos(m_physicsScene);

    return true;
}

void Physics::GunFire()
{
	if (cooldown > 0)
	{
		cooldown -= m_delta_time;
		return;
	}
	else
	{
		cooldown += 0.1f;
	}

	if (bullets.size() > 50)
	{
		PxRigidDynamic* actor = bullets.front();
		actor->release();
		bullets.pop();
	}

	//Transform
	vec3 camPos = m_camera.world[3].xyz();
	vec3 boxVel = -m_camera.world[2].xyz() * 20.0f;
	PxTransform boxTransform(PxVec3(camPos.x, camPos.y, camPos.z));

	//Geometry
	PxSphereGeometry sphere(0.5f);

	//Density
	float density = 10.0f;

	PxRigidDynamic* newActor = PxCreateDynamic(*m_physics, boxTransform, sphere,
													*m_physicsMaterial, density);
	//set speed of bullet
	float muzzlespeed = 50.0f;

	//Set bullet velocity
	glm::vec3 direction(-m_camera.world[2]);
	physx::PxVec3 velocity = physx::PxVec3(direction.x, direction.y, direction.z) * muzzlespeed;
	newActor->setLinearVelocity(velocity, true);
	m_physicsScene->addActor(*newActor);
	bullets.push(newActor);

	//set up collision callback for bullets
	newActor->setName("Bullet");
	FilterGroup::setUpFiltering(newActor, FilterGroup::BULLET, FilterGroup::GROUND | FilterGroup::BOX);
}

void Physics::CharacterController()
{
	bool onGround; //set to true if we are on the ground
	float movementSpeed = 10.0f; //forward and back movement speed
	float rotationSpeed = 1.0f; //turn speed
								//check if we have a contact normal.  if y is greater than .3 we assume this is solid ground.  This is a rather primitive way to do this.  Can you do better?
	if (myHitReport->getPlayerContactNormal().y > 0.3f)
	{
		_characterYVelocity = -0.1f;
		onGround = true;
	}
	else
	{
		_characterYVelocity += _playerGravity * m_delta_time;
		onGround = false;
	}
	myHitReport->clearPlayerContactNormal();
	const PxVec3 up(0, 1, 0);
	//scan the keys and set up our intended velocity based on a global transform	
	PxVec3 velocity(0, _characterYVelocity, 0);
	if (glfwGetKey(this->m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		velocity.x -= movementSpeed*m_delta_time;
	}
	if (glfwGetKey(this->m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		velocity.x += movementSpeed*m_delta_time;
	}
	if (glfwGetKey(this->m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		_characterRotation -= rotationSpeed*m_delta_time;
	}
	if (glfwGetKey(this->m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		_characterRotation += rotationSpeed*m_delta_time;
	}

	//To do.. add code to control z movement and jumping
	float minDistance = 0.001f;
	PxControllerFilters filter;
	//make controls relative to player facing
	PxQuat rotation(_characterRotation, PxVec3(0, 1, 0));
	//move the controller
	playerController->move(rotation.rotate(velocity), minDistance, m_delta_time, filter);

}

void Physics::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
    Gizmos::draw(m_camera.proj, m_camera.view);

	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	glm::mat4 guiMatrix = glm::ortho<float>(0, (float)width, (float)height, 0);
	Gizmos::draw2D(guiMatrix);

    m_renderer->RenderAndClear(m_camera.view_proj);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void AddWidget(PxShape* shape, PxRigidActor* actor, vec4 geo_color)
{
    PxTransform full_transform = PxShapeExt::getGlobalPose(*shape, *actor);
    vec3 actor_position(full_transform.p.x, full_transform.p.y, full_transform.p.z);
    glm::quat actor_rotation(full_transform.q.w,
        full_transform.q.x,
        full_transform.q.y,
        full_transform.q.z);
    glm::mat4 rot(actor_rotation);

    mat4 rotate_matrix = glm::rotate(10.f, glm::vec3(7, 7, 7));

    PxGeometryType::Enum geo_type = shape->getGeometryType();

    switch (geo_type)
    {
    case (PxGeometryType::eBOX) :
    {
        PxBoxGeometry geo;
        shape->getBoxGeometry(geo);
        vec3 extents(geo.halfExtents.x, geo.halfExtents.y, geo.halfExtents.z);
        Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
    } break;
    case (PxGeometryType::eCAPSULE) :
    {
        PxCapsuleGeometry geo;
        shape->getCapsuleGeometry(geo);
        Gizmos::addCapsule(actor_position, geo.halfHeight * 2, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (PxGeometryType::eSPHERE) :
    {
        PxSphereGeometry geo;
        shape->getSphereGeometry(geo);
        Gizmos::addSphereFilled(actor_position, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (PxGeometryType::ePLANE) :
    {

    } break;
    }
}

void Physics::renderGizmos(PxScene* physics_scene)
{
    PxActorTypeFlags desiredTypes = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;
    PxU32 actor_count = physics_scene->getNbActors(desiredTypes);
    PxActor** actor_list = new PxActor*[actor_count];
	physics_scene->getActors(desiredTypes, actor_list, actor_count);
    
    vec4 geo_color(1, 0, 0, 1);
    for (int actor_index = 0;
        actor_index < (int)actor_count;
        ++actor_index)
    {
        PxActor* curr_actor = actor_list[actor_index];
        if (curr_actor->isRigidActor())
        {
            PxRigidActor* rigid_actor = (PxRigidActor*)curr_actor;
            PxU32 shape_count = rigid_actor->getNbShapes();
            PxShape** shapes = new PxShape*[shape_count];
            rigid_actor->getShapes(shapes, shape_count);

            for (int shape_index = 0;
                shape_index < (int)shape_count;
                ++shape_index)
            {
                PxShape* curr_shape = shapes[shape_index];
                AddWidget(curr_shape, rigid_actor, geo_color);
            }

            delete[]shapes;
        }
    }

    delete[] actor_list;

    int articulation_count = physics_scene->getNbArticulations();

    for (int a = 0; a < articulation_count; ++a)
    {
        PxArticulation* articulation;
		physics_scene->getArticulations(&articulation, 1, a);

        int link_count = articulation->getNbLinks();

        PxArticulationLink** links = new PxArticulationLink*[link_count];
        articulation->getLinks(links, link_count);

        for (int l = 0; l < link_count; ++l)
        {
            PxArticulationLink* link = links[l];
            int shape_count = link->getNbShapes();

            for (int s = 0; s < shape_count; ++s)
            {
                PxShape* shape;
                link->getShapes(&shape, 1, s);
                AddWidget(shape, link, geo_color);
            }
        }
        delete[] links;
    }

	for (auto articulation : ragdolls)
	{
		PxU32 nLinks = articulation->getNbLinks();
		PxArticulationLink** links = new PxArticulationLink*[nLinks];
		articulation->getLinks(links, nLinks);

		//Render all shapes in physx actor
		while (nLinks--)
		{
			PxArticulationLink* link = links[nLinks];
			PxU32 nShapes = link->getNbShapes();
			PxShape** shapes = new PxShape*[nShapes];
			link->getShapes(shapes, nShapes);
			while (nShapes--)
			{
				AddWidget(shapes[nShapes], link, geo_color);
			}
			delete[] shapes;
		}
		delete[] links;
	}
}

void Physics::CheckForCollisions()
{
	unsigned int actorCount = actors.size();
	//check for collisons against all other objects
	for (unsigned int outer = 0; outer < actorCount - 1; ++outer)
	{
		for (unsigned int inner = outer + 1; inner < actorCount; ++inner)
		{
			PhysicsObject* object1 = actors[outer];
			PhysicsObject* object2 = actors[inner];
			if ((object1->layer == 0 || object2->layer == 0) ||
				object1->layer != object2->layer)
			{
				int _shapeID1 = object1->GetShapeID();
				int _shapeID2 = object2->GetShapeID();
				// using function pointers
				int functionIndex = (_shapeID1 * NUMBERSHAPE) + _shapeID2;
				fn collisionFucntionPtr = CollisionChecker::CollisionFunctionArray[functionIndex];
				if (collisionFucntionPtr != nullptr)
				{
					collisionFucntionPtr(object1, object2);
				}
			}
		}
	}
}

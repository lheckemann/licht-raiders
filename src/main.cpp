#include <irrlicht.h>
#include <string>
#include <ctime>
#include <vector>
#include <boost/filesystem.hpp>

#include "globs.h"
#include "map.h"
#include "entity.h"


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#include <windows.h>
#endif

using namespace irr;
using core::vector3df;
using core::vector2df;
using video::SColor;

EventReceiver receiver;
ConfigFile UserConfig;

video::IVideoDriver* driver;
scene::ISceneManager* smgr;
gui::IGUIEnvironment* guienv;
scene::ISceneCollisionManager* collMan;

Map *map;

GameState state;

// I couldn't resist, alright? :D
void bork(std::string msg) {
#ifndef BE_POLITE
	printf("SHIT! %s!\n", msg.c_str());
#else
	printf("%s, aborting.\n", msg.c_str());
#endif
	exit(-1);
}


IrrlichtDevice *setupDevice(EventReceiver &receiver, ConfigFile *UserConfig) {
	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
	if (UserConfig->read<bool>("display.hardwareDriverOnly", true)) {
		if (IrrlichtDevice::isDriverSupported(video::EDT_OPENGL)) {
			params.DriverType = video::EDT_OPENGL;
		}
		else if (IrrlichtDevice::isDriverSupported(video::EDT_DIRECT3D9)) {
			params.DriverType = video::EDT_DIRECT3D9;
		}
		else if (IrrlichtDevice::isDriverSupported(video::EDT_DIRECT3D8)) {
			params.DriverType = video::EDT_DIRECT3D8;
		}
		else {
			printf("No suitable video driver found.\n");
			return NULL;
		}
	}
	else {
		if (UserConfig->read<bool>("display.forceSoftwareDriver", false)) {
			params.DriverType = video::EDT_BURNINGSVIDEO;
		}
		else {
			if (IrrlichtDevice::isDriverSupported(video::EDT_OPENGL)) {
				params.DriverType = video::EDT_OPENGL;
			}
			else if (IrrlichtDevice::isDriverSupported(video::EDT_DIRECT3D9)) {
				params.DriverType = video::EDT_DIRECT3D9;
			}
			else if (IrrlichtDevice::isDriverSupported(video::EDT_DIRECT3D8)) {
				params.DriverType = video::EDT_DIRECT3D8;
			}
			else if (IrrlichtDevice::isDriverSupported(video::EDT_BURNINGSVIDEO)) {
				params.DriverType = video::EDT_BURNINGSVIDEO;
			}
			else {
				bork("No suitable video driver found");
			}
		}
	}

	params.WindowSize = core::dimension2d<u32>(UserConfig->read<int>("display.width", 800), UserConfig->read<int>("display.height", 600));
	params.Bits = 32;
	params.Fullscreen = UserConfig->read<bool>("display.fullscreen", false);
	params.Stencilbuffer = false;
	params.Doublebuffer = true;
	params.Vsync = true;
	params.EventReceiver = &receiver;
	return createDeviceEx(params);
}

std::string get_userdata_path() {
#ifdef __unix
	string result ( getenv("HOME") );
	result += "/.lichtraiders/";
#else // __unix
#ifdef __WIN32__
	string result ( getenv("APPDATA") );
	result += "\\lichtraiders\\";
#endif // __WIN32__
#endif // __unix
	return result;
}

float camRot = 0, camHeight = 20, camHeightChange = 0;
vector3df camMove(0, 0, 0);
float camSpeed;

int main() {
	state.phase = GameState::PHASE_ENTERING;
	state.state = GameState::STATE_INGAME; // asdf
	try {
		UserConfig = ConfigFile(get_userdata_path() + "user.cfg");
	}
	catch (ConfigFile::file_not_found) {
		printf("No user config file found, creating...\n");
		boost::filesystem::create_directories(get_userdata_path());
		FILE *f;
		f = fopen((get_userdata_path() + "user.cfg").c_str(), "w");
		if (f == NULL) {
			printf("Could not create user config file. Aborting. Please check that the path to the user config file is available: %s", (get_userdata_path() + "user.cfg").c_str());
			exit(-1);
		}
		fclose(f);
		UserConfig = ConfigFile(get_userdata_path() + "user.cfg");
	}

	IrrlichtDevice *device = setupDevice(receiver, &UserConfig);
	if (!device) {
		printf("Could not create device.\n");
	}

	device->setWindowCaption(L"Licht Raiders");
	device->setResizable(false);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	collMan = smgr->getSceneCollisionManager();

/* Set up GUI */
//	gui::IGUISkin* skin = env->getSkin();
	setup_GUI();

/* Set up camera */

	scene::ISceneNode *cam_base = smgr->addEmptySceneNode(0, DEFAULT_ID);
	scene::ICameraSceneNode *cam = smgr->addCameraSceneNode(cam_base, vector3df(0, 20, -2), vector3df(0, 0, 0), DEFAULT_ID);
	cam->setTarget(core::vector3df(0,0,0));
	cam->setFarValue(42000.0f);
	cam->setNearValue(0.0625f);

	vector3df rotatedCamMove;

	vector3df camPos = cam->getPosition();
	vector3df camTarget = cam->getTarget();
	vector3df tempVec; // Used for miscellaneous things, just as a temporary vec3df storage

/* Map */
	FILE *mapfile;
	mapfile = fopen("data/maps/test.map", "rb");
	if (mapfile == NULL) {
		bork("Could not open test map");
	}
	map = new Map;
	map->load(mapfile);
	fclose(mapfile);

/* Set up lighting */

	smgr->setAmbientLight(SColor(0xFFFFFF));
	scene::ILightSceneNode *light = smgr->addLightSceneNode(0, vector3df(0,30,0), SColor(0xffffff), 160.0f, DEFAULT_ID);
	scene::ILightSceneNode *light2 = smgr->addLightSceneNode(0, vector3df(0,30,0), SColor(0xffffff), 160.0f, DEFAULT_ID);

/* Set up skybox */

	const io::path skyboxFilename ("data/textures/skybox/top.png");
	video::ITexture *sb_tex = driver->getTexture(skyboxFilename);
	smgr->addSkyBoxSceneNode(sb_tex, sb_tex, sb_tex, sb_tex, sb_tex, sb_tex, 0, DEFAULT_ID);

/* T-Minus ten! */
	ITimer* timer = device->getTimer();
	timer->setTime(0);
	unsigned int now = 0;
	unsigned int lastUpdate = 0;
	int frame = 0;
	vector3df mousething;
//	scene::IMesh *arrowMesh = smgr->addArrowMesh("ITSANARROW", 0xFFFFFF, 0xFF0000);
//	scene::IMeshSceneNode *mouseNode = smgr->addMeshSceneNode(arrowMesh, 0, DEFAULT_ID); // Put this back once we implement entity movement
//	mouseNode->setRotation(vector3df(0, 0, 180));

	core::line3df ray;
	core::triangle3df dummyTri;

	Entity randomEntity(vector2df(1, 1));
/* We have liftoff! */
	while (device->run()) {
		now = timer->getTime();
		if (now >= lastUpdate + 1000.0/MAX_FPS) {
			frame++;
			lastUpdate = now;

			driver->beginScene(true, true, SColor(255, 255, 0, 255));
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();

			rotatedCamMove = camMove;
			rotatedCamMove.rotateXZBy(camRot);
			camPos = cam_base->getPosition();
			camTarget = cam->getTarget();
			camPos += camMove;
			camTarget += camMove;
			cam_base->setPosition(camPos);
			cam->setTarget(camTarget);
			camPos = cam->getAbsolutePosition(); // Does reusing this variable for this count as bad? Does mah face look bovvered?
			camPos.Y = light->getPosition().Y;
			light->setPosition(camPos);
			light2->setPosition(camTarget);
			camHeight += camHeightChange;
			tempVec = vector3df(50,camHeight,0);
			tempVec.rotateXZBy(camRot);
			cam->setPosition(tempVec);

			if(frame % 400 == 0) printf("%i FPS\n", driver->getFPS());
		}
		else {
			device->sleep(10);
		}
	}
	device->drop();
	return 0;
}

//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------


//ALL The includes in one file
#include "GameIncludes.h"


GameObject* pTextureSwapHelper = 0;

// TO DO: ----------------------------------
//

float animSpeed = 0.2f;

bool pause = false;
bool fastforward = false;
bool fastbackward = false;
bool reset = false;


bool humanWalk = true;
bool humanRun = false;

Clip* cliprun;
Clip* clipHumanRun;
Clip* clipHumanWalk;


bool walk = true;
bool idle = false;

Clip* clipWalk;
Clip* clipIdle;

//-----------------------------------------------------------------------------
//  Game::Game()
//		Game Engine Constructor
//-----------------------------------------------------------------------------
Game::Game(const char* const pWindowName, const int Width, const int Height)
	:Engine(pWindowName, Width, Height)
{
	assert(pWindowName);
	this->globalTimer.Tic();
	this->intervalTimer.Tic();
}

//-----------------------------------------------------------------------------
//  Game::~Game()
//		Game Engine destructor
//-----------------------------------------------------------------------------
Game::~Game()
{

}

//-----------------------------------------------------------------------------
// Game::Initialize()
//		Allows the engine to perform any initialization it needs to before 
//      starting to run.  This is where it can query for any required services 
//      and load any non-graphic related content. 
//-----------------------------------------------------------------------------
void Game::Initialize()
{
	// Start Managers
	//CameraMan::Create(this->info.windowWidth, this->info.windowHeight);
	ImageMan::Create();
	InputMan::Create();
	ModelMan::Create();
	ShaderMan::Create();
	FrameMan::Create();
	ClipMan::Create();
	AnimMan::Create();
	GlyphMan::Create();
	FontMan::Create();
}


//-----------------------------------------------------------------------------
// Game::LoadContent()
//		Allows you to load all content needed for your engine,
//	    such as objects, graphics, etc.
//-----------------------------------------------------------------------------
void Game::LoadContent()
{
	// Camera setup

	// Camera setup
	Camera* pCam0 = new Camera(Camera::Type::PERSPECTIVE_3D);
	pCam0->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);

	//// For Human
	pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 20000.0f);
	pCam0->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 0.0f), Vect(4000.0f, 0.0f, 0.0f));
	
	//// For teddy
	//pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 20000.0f);
	//pCam0->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 50.0f), Vect(400.0f, 100.0f, 0.0f));



	pCam0->updateCamera();
	CameraMan::Add(Camera::Name::CAMERA_0, pCam0);
	CameraMan::SetCurrent(Camera::Name::CAMERA_0,Camera::Type::PERSPECTIVE_3D);




	//---------------------------------------------------------------------------------------------------------
	// Setup the current 2D orthographic Camera
	//---------------------------------------------------------------------------------------------------------
	Camera* pCam2D = new Camera(Camera::Type::ORTHOGRAPHIC_2D);

	pCam2D->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam2D->setOrthographic(-pCam2D->getScreenWidth() / 2.0f, pCam2D->getScreenWidth() / 2.0f,
		-pCam2D->getScreenHeight() / 2.0f, pCam2D->getScreenHeight() / 2.0f,
		1.0f, 1000.0f);
	pCam2D->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, -1.0f), Vect(0.0f, 0.0f, 2.0f));

	// Holder for the current 2D  cameras
	CameraMan::Add(Camera::Name::CAMERA_1, pCam2D);
	CameraMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::ORTHOGRAPHIC_2D);


	// Load the Cube model
	CubeModel* pCubeModel = new CubeModel("cubeModel.azul");
	assert(pCubeModel);
	ModelMan::Add(pCubeModel);

	// Load the Sphere model
	SphereModel* pSphereModel = new SphereModel("SphereModel.azul");
	assert(pSphereModel);
	ModelMan::Add(pSphereModel);

	// Load the Pyramid model
	PyramidModel* pPyramidModel = new PyramidModel("PyramidModel.azul");
	assert(pPyramidModel);
	ModelMan::Add(pPyramidModel);

	// Load the 2Pyramid model
	_2PyramidModel* p_2PyramidModel = new _2PyramidModel("_2PyramidModel.azul");
	assert(p_2PyramidModel);
	ModelMan::Add(p_2PyramidModel);

	
	CustomModel* pBuggy_Model = new CustomModel("buggy.spu", Texture::Name::BUGGY);
	assert(pBuggy_Model);
	ModelMan::Add(pBuggy_Model);

	CustomModel* pWarbear_Model = new CustomModel("warbear.spu", Texture::Name::WARBEAR);
	assert(pWarbear_Model);
	ModelMan::Add(pWarbear_Model);

	//animHuman_01->SetW8S("hskn.spu","hskn");

	SkinModel* pHumanSkin_Model = new SkinModel("hskn_verts.spu", "hskn.spu", Texture::Name::DEFAULT);
	assert(pHumanSkin_Model);
	ModelMan::Add(pHumanSkin_Model);
	
	SkinModel* pTeddySkin_Model = new SkinModel("Tskn_verts.spu", "Tskn.spu", Texture::Name::DEFAULT);
	assert(pTeddySkin_Model);
	ModelMan::Add(pTeddySkin_Model);
	

	///////COLLIDER///////

	// Load the model
	Ritter_SphereModel* pRitterSphereModel = new Ritter_SphereModel("Ritter_sphereModel.azul");
	assert(pSphereModel);
	ModelMan::Add(pRitterSphereModel);
	//////COLLIDER///////

	// Create/Load Shader 
	ShaderObject* pShaderObject_color = new ShaderObject(ShaderObject::Name::COLOR_LIGHT, "colorRender",ShaderObject::Type::VERTEX);
	assert(pShaderObject_color);
	ShaderMan::Add(pShaderObject_color);

	ShaderObject* pShaderObject_texture = new ShaderObject(ShaderObject::Name::TEXTURE_SIMPLE, "textureRender", ShaderObject::Type::VERTEX);
	assert(pShaderObject_texture);
	ShaderMan::Add(pShaderObject_texture);

	ShaderObject* pShaderObject_textureLight =
		new ShaderObject(ShaderObject::Name::TEXTURE_POINT_LIGHT, 
			"texturePointLightDiff", ShaderObject::Type::VERTEX);
	assert(pShaderObject_textureLight);
	ShaderMan::Add(pShaderObject_textureLight);

	ShaderObject* pShader_ColorSimple = new ShaderObject(ShaderObject::Name::SIMPLE_COLOR, "SimpleColorShader", ShaderObject::Type::VERTEX);
	assert(pShader_ColorSimple != 0);
	ShaderMan::Add(pShader_ColorSimple);

	ShaderObject* pShader_RealColorLight = new ShaderObject(ShaderObject::Name::REAL_COLOR_LIGHT, "ColorPointLightDiff", ShaderObject::Type::VERTEX);
	assert(pShader_ColorSimple != 0);
	ShaderMan::Add(pShader_RealColorLight);
	
	ShaderObject* pShaderObject_colorSingle = new ShaderObject(ShaderObject::COLOR_SINGLE, "colorSingleRender", ShaderObject::Type::VERTEX);
	assert(pShaderObject_colorSingle);
	ShaderMan::Add(pShaderObject_colorSingle);
	
	ShaderObject* pShaderObject_SkinColor = new ShaderObject(ShaderObject::COLOR_SKIN, "colorSkin", ShaderObject::Type::VERTEX);
	assert(pShaderObject_SkinColor);
	ShaderMan::Add(pShaderObject_SkinColor);
	
	ShaderObject* pShaderObject_SkinLight = new ShaderObject(ShaderObject::LIGHT_SKIN, "SkinLight", ShaderObject::Type::VERTEX);
	assert(pShaderObject_SkinLight);
	ShaderMan::Add(pShaderObject_SkinLight);


	ShaderObject* pShaderObject_mixer = new ShaderObject(ShaderObject::Name::COMPUTE_MIXER, "computeMixer", ShaderObject::Type::COMPUTE);
	assert(pShaderObject_mixer);
	ShaderMan::Add(pShaderObject_mixer);

	ShaderObject* pShaderObject_world = new ShaderObject(ShaderObject::Name::COMPUTE_WORLD, "computeWorldMatrix", ShaderObject::Type::COMPUTE);
	assert(pShaderObject_world);
	ShaderMan::Add(pShaderObject_world);

	//default texture //default texture
	Texture::Add("HotPink.tga", Texture::Name::DEFAULT);

	// Textures 
	Texture::Add("Rocks.tga", Texture::Name::ROCKS, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	Texture::Add("Stone.tga", Texture::Name::STONES);
	Texture::Add("RedBrick.tga", Texture::Name::RED_BRICK);
	Texture::Add("Duckweed.tga", Texture::Name::DUCKWEED);
	Texture::Add("Aliens.tga", Texture::Name::INVADERS);
	Texture::Add("Consolas36pt.tga",Texture::Name::Consolas36pt);


	// Images
	Image* pImageGreen = new Image(Image::Name::Alien_Green, Texture::Name::INVADERS, Rect(136.0f, 64.0f, 85.0f, 63.0f));
	//Image* pImageGreen = new Image(Image::Name::Alien_Green, Texture::Name::INVADERS, Rect(554.0f, 63.0f, 98.0f, 64.0f));

	assert(pImageGreen);
	ImageMan::Add(pImageGreen);

///////////222222222222222222222222222222222dddddddddddddddddddddddddddddddddddd//////////////////////////////////////
	// Load the model
	SpriteModel* pSpriteModel = new SpriteModel("SpriteModel.azul");
	assert(pSpriteModel);
	ModelMan::Add(pSpriteModel);

	// Create/Load Shader 
	ShaderObject* pShaderObject_sprite = new ShaderObject(ShaderObject::Name::SPRITE, "spriteRender", ShaderObject::Type::VERTEX);
	assert(pShaderObject_sprite);
	ShaderMan::Add(pShaderObject_sprite);


	// Sprite
	GraphicsObject_Sprite* pGraphics_Sprite = new GraphicsObject_Sprite(pSpriteModel, pShaderObject_sprite, pImageGreen, Rect(55, 55, 50, 50));
	GameObject2D* pA1 = new GameObject2D(pGraphics_Sprite);
	GameObjectMan::Add(pA1, GameObjectMan::GetRoot());
	//pA1->posX = 900.0f;
	//pA1->posY = 450.0f;

	///////////222222222222222222222222222222222dddddddddddddddddddddddddddddddddddd//////////////////////////////////////

	////////////////////////////Font//////////////////////////////////
	std::string str = "Consolas36pt.xml";

	GlyphMan::AddXml(Glyph::Name::Consolas36pt, str, Texture::Name::Consolas36pt);

	GraphicsObjectFont* grph = new GraphicsObjectFont(pSpriteModel,pShaderObject_sprite);

	GameObject2DFont* font = new GameObject2DFont(grph,"ABCD",120,860);

	GameObjectMan::Add(font, GameObjectMan::GetRoot());

	FontMan::Add(Font::Name::Text, font);

	//cntr

	GraphicsObjectFont* grph2 = new GraphicsObjectFont(pSpriteModel, pShaderObject_sprite);

	GameObject2DFont* ctr = new GameObject2DFont(grph2,"ABCD",320,860);

	GameObjectMan::Add(ctr, GameObjectMan::GetRoot());

	FontMan::Add(Font::Name::COUNTER, ctr);



	////////////////////////////Font//////////////////////////////////

	Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	Vect pos(1, 1, 1);
	Vect pos3(4.0f, -2.5f, 0.0f);
	Vect pos2(-4.0f, 2.0f, 0.0f);


	// Create GameObject
	SimpleGameObject* pGameObj = nullptr;
	GraphicsObject* pGraphicsObj = nullptr;

	pGraphicsObj = new GraphicsObject_TextureLight
	(pBuggy_Model, pShaderObject_textureLight, Texture::Name::BUGGY, color, pos);
	pGameObj = new SimpleGameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(-80.0f, -880.0f, 502.0f));
	pGameObj->SetDeltaRot(0.007f);
	pGameObj->SetScale(Vect(1.0f, 1.0f, 1.0f));
	pGameObj->SetName("Buggy_1");
	pGameObj->SetMove(SimpleGameObject::Move_X);
	GameObjectMan::Add(pGameObj);


	pGraphicsObj = new GraphicsObject_TextureLight(pWarbear_Model, pShaderObject_textureLight, Texture::Name::WARBEAR, color, pos);
	pGameObj = new SimpleGameObject(pGraphicsObj);
	pGameObj->SetPos(Vect(880.0f, -888.0f, -226.0f));
	pGameObj->SetDeltaRot(0.07f);
	pGameObj->SetScale(Vect(4.0f, 4.0f, 4.0f));
	pGameObj->SetName("Warbear");
	pTextureSwapHelper = pGameObj;
	GameObjectMan::Add(pGameObj);




	//////////////AAAAAAAAAAAAAAAAAAnnnnnnnnnnnnnnnniiiiiiiiiimaaaaaaaaaatiiiiooooonnnnnnnn///////////////////////
	

	AnimController* animHuman_01 = 
		new AnimControllerHuman
		(pPyramidModel, pHumanSkin_Model, pShaderObject_textureLight,
		pShaderObject_SkinLight,AnimController::InstancenName::HUMAN_01);
	AnimMan::Add(animHuman_01);




	// setup relationship and hierarchy
	animHuman_01->SetAnimationHierarchy("H_W.spu");

	animHuman_01->ComputeHierarchyTable(SSBO::Name::HUMAN_HIERARCHY, SSBO::Name::HUMAN_WORLD);

	// initialize animation data
	animHuman_01->SetAnimationData("H_W.spu", "Hero_Walk");

	//Send data to compute shader
	animHuman_01->ComputeKeyFrame(SSBO::Name::HUMAN_WALK_KEY, SSBO::Name::HUMAN_WALK_RESULT);

	//animHuman_01->SetAnimationData("G_R.spu","Generic_Run");
	animHuman_01->SetAnimationData("H_R_F.spu","HRF");

	//Send data to compute shader
	animHuman_01->ComputeKeyFrame(SSBO::Name::HUMAN_RUN_KEY, SSBO::Name::HUMAN_RUN_RESULT);

	animHuman_01->SetInvPose("hskn.spu", "hskn");

	//clipshot = ClipMan::Find(Clip::ClipName::GENERIC_RUN);
	clipHumanWalk = ClipMan::Find(Clip::ClipName::HERO_WALK);
	clipHumanRun = ClipMan::Find(Clip::ClipName::HERO_RUN_FAST);


	Vect Trans = Vect(-680.172638f, 1288.917847f - 500, 526.212402f);
	Matrix T = Matrix(Matrix::Trans::XYZ, -Trans);
	Matrix M = T;

	Matrix R = Matrix(Matrix::Rot1::Z, 1.5f);

	M = M * R;

	animHuman_01->pSkeleton->pGameRigid->SetWorld(&M);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	AnimController* animTeddy_01 = 
	new AnimControllerTeddy
	(pPyramidModel, pTeddySkin_Model, pShaderObject_textureLight,
	pShaderObject_SkinLight, AnimController::InstancenName::TEDDY_01);
	AnimMan::Add(animTeddy_01);
	
	//// setup relationship and hierarchy
	animTeddy_01->SetAnimationHierarchy("teddy_TGA.spu");
	
	animTeddy_01->ComputeHierarchyTable(SSBO::Name::TEDDY_HIERARCHY, SSBO::Name::TEDDY_WORLD);
	//animTeddy_02->SetAnimationHierarchy("teddy_TGA.spu");

	//// initialize animation data
	animTeddy_01->SetAnimationData("teddy_TGA.spu", "walk");

	//Send data to compute shader
	animTeddy_01->ComputeKeyFrame(SSBO::Name::TEDDY_WALK_KEY, SSBO::Name::TEDDY_WALK_RESULT);

	animTeddy_01->SetAnimationData("teddy_TGA.spu", "idle");

	//Send data to compute shader
	animTeddy_01->ComputeKeyFrame(SSBO::Name::TEDDY_IDLE_KEY, SSBO::Name::TEDDY_IDLE_RESULT);

	animTeddy_01->SetInvPose("Tskn.spu", "Tskn"); 

	clipWalk = ClipMan::Find(Clip::ClipName::TEDDY_WALK);
	clipIdle = ClipMan::Find(Clip::ClipName::TEDDY_IDLE);
}


void Demo()
{
	// ------------------------------------------------
	// Animate ME
	// ------------------------------------------------
	Keyboard* key = InputMan::GetKeyboard();

	if (key->GetKeyState(Keyboard::KEY_F))
	{
		animSpeed = 0.4f;
	}

	if (key->GetKeyState(Keyboard::KEY_B))
	{
		animSpeed = -0.4f;
	}

	if (key->GetKeyState(Keyboard::KEY_N))
	{
		animSpeed = 0.2f;
	}

	if (key->GetKeyState(Keyboard::KEY_R))
	{
		humanWalk = true;
		humanRun = false;
	}
	if (key->GetKeyState(Keyboard::KEY_P))
	{
		humanWalk = false;
		humanRun = true;
	}
	if (key->GetKeyState(Keyboard::KEY_W))
	{
		walk = true;
		idle = false;
	}
	if (key->GetKeyState(Keyboard::KEY_I))
	{
		walk = false;
		idle = true;
	}

	if (humanWalk)
	{
		AnimController* animHuman_01 = 
		AnimMan::Find(AnimController::InstancenName::HUMAN_01);
		animHuman_01->ProcessAnimation
		(clipHumanWalk,SSBO::Name::HUMAN_WALK_KEY, SSBO::Name::HUMAN_WALK_RESULT);

		// create skeleton
		animHuman_01->SetAnimationPose();

		animHuman_01->pSkeleton->pFirstBone->AnimUpdate(animHuman_01->tCurrent, animHuman_01, animSpeed,
														SSBO::Name::HUMAN_WALK_RESULT);
	}
	if (humanRun)
	{

		AnimController* animHuman_01 = AnimMan::Find(AnimController::InstancenName::HUMAN_01);

		animHuman_01->ProcessAnimation(clipHumanRun, SSBO::Name::HUMAN_RUN_KEY, SSBO::Name::HUMAN_RUN_RESULT);

		// create skeleton
		animHuman_01->SetAnimationPose();


		animHuman_01->pSkeleton->pFirstBone->AnimUpdate(animHuman_01->tCurrent, animHuman_01, animSpeed,
														SSBO::Name::HUMAN_RUN_RESULT);
	}

	//////////////////////////////////////////ttttteeeeddddddddddyyyyyyy WALK///////////////////////////////////////////////////////////////////////////
	if (walk)
	{
		AnimController* animTeddy_01 =
		AnimMan::Find(AnimController::InstancenName::TEDDY_01);
		animTeddy_01->ProcessAnimation
		(clipWalk,SSBO::Name::TEDDY_WALK_KEY, SSBO::Name::TEDDY_WALK_RESULT);


		//create skeleton
		animTeddy_01->SetAnimationPose();

		animTeddy_01->pSkeleton->pFirstBone->AnimUpdate(animTeddy_01->tCurrent, 
														animTeddy_01, animSpeed, SSBO::Name::TEDDY_WALK_RESULT);

	}
	if (idle)
	{
		AnimController* animTeddy_01 = AnimMan::Find(AnimController::InstancenName::TEDDY_01);

		animTeddy_01->ProcessAnimation(clipIdle,SSBO::Name::TEDDY_IDLE_KEY, SSBO::Name::TEDDY_IDLE_RESULT);
		//create skeleton
		animTeddy_01->SetAnimationPose();

		animTeddy_01->pSkeleton->pFirstBone->AnimUpdate(animTeddy_01->tCurrent,
														animTeddy_01, animSpeed, SSBO::Name::TEDDY_IDLE_RESULT);
	}
}

int i = 0;

	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------
void Game::Update(float )
{
	std::stringstream sstm;
	std::string result;

	char* name = "Human  ";


	i++;

	sstm << name << i;
	result += sstm.str();


	FontMan::Update(Font::Name::COUNTER, result);

	// ------------------------------------------------
	// Camera update
	// ------------------------------------------------

	Camera* pCam3D = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);
	pCam3D->updateCamera();

	Camera* pCam2D = CameraMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);
	pCam2D->updateCamera();


		////// Note: these static should be tied into an animation system
		//static Time tCurrent(Time::ZERO);

		// Mark our end time.
		this->intervalTimer.Toc();
		this->intervalTimer.Tic();
		Demo();
		Time t;
		GameObjectMan::Update(t);
}

//-----------------------------------------------------------------------------
// Game::Draw()
//		This function is called once per frame
//	    Use this for draw graphics to the screen.
//      Only do rendering here
//-----------------------------------------------------------------------------
void Game::Draw()
{
	GameObjectMan::Draw();
}


//-----------------------------------------------------------------------------
// Game::UnLoadContent()
//       unload content (resources loaded above)
//       unload all content that was loaded before the Engine Loop started
//-----------------------------------------------------------------------------
void Game::UnLoadContent()
{	
	CameraMan::Destroy();
	Texture::Destroy();
	InputMan::Destroy();
	GameObjectMan::Destroy();
	ModelMan::Destroy();
	ShaderMan::Destroy();

	FrameMan::Destroy();
	ClipMan::Destroy();
	ImageMan::Destroy();
	AnimMan::Destroy();
	FontMan::Destroy();
	GlyphMan::Destroy();
	SSBOMan::Destroy();
	
}

//------------------------------------------------------------------
// Game::ClearBufferFunc()
// Allows user to change the way the clear buffer function works
//------------------------------------------------------------------
void Game::ClearBufferFunc()
{
	const GLfloat grey[] = { 0.7f, 0.7f, 0.8f, 1.0f };
	const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, grey);
	glClearBufferfv(GL_DEPTH, 0, &one);
}


// ---  End of File ---------------

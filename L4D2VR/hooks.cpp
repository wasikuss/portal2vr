#include "hooks.h"
#include "game.h"
#include "texture.h"
#include "sdk.h"
#include "sdk_server.h"
#include "vr.h"
#include "offsets.h"
#include <iostream>

Hooks::Hooks(Game *game)
{
	if (MH_Initialize() != MH_OK)
	{
		Game::errorMsg("Failed to init MinHook");
	}

	m_Game = game;
	m_VR = m_Game->m_VR;

	m_PushHUDStep = -999;
	m_PushedHud = false;

	initSourceHooks();

	hkGetRenderTarget.enableHook();
	hkCalcViewModelView.enableHook();
	/*hkServerFireTerrorBullets.enableHook();
	hkClientFireTerrorBullets.enableHook();*/

	hkProcessUsercmds.enableHook();
	hkReadUsercmd.enableHook();

	//hkWriteUsercmdDeltaToBuffer.enableHook();
	hkWriteUsercmd.enableHook();

	hkAdjustEngineViewport.enableHook();
	hkViewport.enableHook();
	hkGetViewport.enableHook();

	/*hkCreateMove.enableHook();
	hkTestMeleeSwingCollisionClient.enableHook();
	hkTestMeleeSwingCollisionServer.enableHook();
	hkDoMeleeSwingServer.enableHook();
	hkStartMeleeSwingServer.enableHook();
	hkPrimaryAttackServer.enableHook();
	hkItemPostFrameServer.enableHook();
	hkGetPrimaryAttackActivity.enableHook();*/

	hkEyePosition.enableHook();

	//hkDrawModelExecute.enableHook();
	hkRenderView.enableHook();
	hkPushRenderTargetAndViewport.enableHook();
	hkPopRenderTargetAndViewport.enableHook();
	hkVgui_Paint.enableHook();
	hkIsSplitScreen.enableHook();
	hkPrePushRenderTarget.enableHook();
	hkGetFullScreenTexture.enableHook();

	hkWeapon_ShootPosition.enableHook();
	hkTraceFirePortal.enableHook();
	hkHandlePortallingClient.enableHook();
}

Hooks::~Hooks()
{
	if (MH_Uninitialize() != MH_OK)
	{
		Game::errorMsg("Failed to uninitialize MinHook");
	}
}


int Hooks::initSourceHooks()
{
	LPVOID pGetRenderTargetVFunc = (LPVOID)(m_Game->m_Offsets->GetRenderTarget.address);
	hkGetRenderTarget.createHook(pGetRenderTargetVFunc, &dGetRenderTarget);

	LPVOID pRenderViewVFunc = (LPVOID)(m_Game->m_Offsets->RenderView.address);
	hkRenderView.createHook(pRenderViewVFunc, &dRenderView);

	LPVOID calcViewModelViewAddr = (LPVOID)(m_Game->m_Offsets->CalcViewModelView.address);
	hkCalcViewModelView.createHook(calcViewModelViewAddr, &dCalcViewModelView);

	LPVOID ProcessUsercmdsAddr = (LPVOID)(m_Game->m_Offsets->ProcessUsercmds.address);
	hkProcessUsercmds.createHook(ProcessUsercmdsAddr, &dProcessUsercmds);

	LPVOID ReadUserCmdAddr = (LPVOID)(m_Game->m_Offsets->ReadUserCmd.address);
	hkReadUsercmd.createHook(ReadUserCmdAddr, &dReadUsercmd);

	/*LPVOID WriteUsercmdDeltaToBufferAddr = (LPVOID)(m_Game->m_Offsets->WriteUsercmdDeltaToBuffer.address);
	hkWriteUsercmdDeltaToBuffer.createHook(WriteUsercmdDeltaToBufferAddr, &dWriteUsercmdDeltaToBuffer);*/

	LPVOID WriteUsercmdAddr = (LPVOID)(m_Game->m_Offsets->WriteUsercmd.address);
	hkWriteUsercmd.createHook(WriteUsercmdAddr, &dWriteUsercmd);

	LPVOID AdjustEngineViewportAddr = (LPVOID)(m_Game->m_Offsets->AdjustEngineViewport.address);
	hkAdjustEngineViewport.createHook(AdjustEngineViewportAddr, &dAdjustEngineViewport);

	LPVOID ViewportAddr = (LPVOID)(m_Game->m_Offsets->Viewport.address);
	hkViewport.createHook(ViewportAddr, &dViewport);

	LPVOID GetViewportAddr = (LPVOID)(m_Game->m_Offsets->GetViewport.address);
	hkGetViewport.createHook(GetViewportAddr, &dGetViewport);

	LPVOID EyePositionAddr = (LPVOID)(m_Game->m_Offsets->EyePosition.address);
	hkEyePosition.createHook(EyePositionAddr, &dEyePosition);

	/*LPVOID DrawModelExecuteAddr = (LPVOID)(m_Game->m_Offsets->DrawModelExecute.address);
	hkDrawModelExecute.createHook(DrawModelExecuteAddr, &dDrawModelExecute);*/

	LPVOID PushRenderTargetAddr = (LPVOID)(m_Game->m_Offsets->PushRenderTargetAndViewport.address);
	hkPushRenderTargetAndViewport.createHook(PushRenderTargetAddr, &dPushRenderTargetAndViewport);

	LPVOID PopRenderTargetAddr = (LPVOID)(m_Game->m_Offsets->PopRenderTargetAndViewport.address);
	hkPopRenderTargetAndViewport.createHook(PopRenderTargetAddr, &dPopRenderTargetAndViewport);

	LPVOID VGui_PaintAddr = (LPVOID)(m_Game->m_Offsets->VGui_Paint.address);
	hkVgui_Paint.createHook(VGui_PaintAddr, &dVGui_Paint);

	LPVOID IsSplitScreenAddr = (LPVOID)(m_Game->m_Offsets->IsSplitScreen.address);
	hkIsSplitScreen.createHook(IsSplitScreenAddr, &dIsSplitScreen);

	LPVOID PrePushRenderTargetAddr = (LPVOID)(m_Game->m_Offsets->PrePushRenderTarget.address);
	hkPrePushRenderTarget.createHook(PrePushRenderTargetAddr, &dPrePushRenderTarget);

	LPVOID GetFullScreenTextureAddr = (LPVOID)(m_Game->m_Offsets->GetFullScreenTexture.address);
	hkGetFullScreenTexture.createHook(GetFullScreenTextureAddr, &dGetFullScreenTexture);

	LPVOID Weapon_ShootPositionAddr = (LPVOID)(m_Game->m_Offsets->Weapon_ShootPosition.address);
	hkWeapon_ShootPosition.createHook(Weapon_ShootPositionAddr, &dWeapon_ShootPosition);
	
	LPVOID TraceFirePortalAddr = (LPVOID)(m_Game->m_Offsets->TraceFirePortalServer.address);
	hkTraceFirePortal.createHook(TraceFirePortalAddr, &dTraceFirePortal);

	LPVOID HandlePortallingClientAddr = (LPVOID)(m_Game->m_Offsets->HandlePortallingServer.address);
	hkHandlePortallingClient.createHook(HandlePortallingClientAddr, &dHandlePortallingClient);

	LPVOID EyeAnglesAddr = (LPVOID)(m_Game->m_Offsets->EyeAngles.address);
	hkEyeAngles.createHook(EyeAnglesAddr, &dEyeAngles);

	/*void *clientMode = nullptr;
	while (!clientMode)
	{
		Sleep(10);
		clientMode = **(void ***)(m_Game->m_Offsets->g_pClientMode.address);
	}
	hkCreateMove.createHook( (*(void ***)clientMode)[27], dCreateMove );*/

	return 1;
}


ITexture *__fastcall Hooks::dGetFullScreenTexture()
{
	ITexture *result = hkGetFullScreenTexture.fOriginal();
	return result;
}

ITexture* __fastcall Hooks::dGetRenderTarget(void* ecx, void* edx)
{
	ITexture* result = hkGetRenderTarget.fOriginal(ecx);
	return result;
}

void __fastcall Hooks::dRenderView(void *ecx, void *edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
{
	if (!m_VR->m_CreatedVRTextures)
		m_VR->CreateVRTextures();

	if (m_VR->m_Game->m_VguiSurface->IsCursorVisible())
		return hkRenderView.fOriginal(ecx, setup, hudViewSetup, nClearFlags, whatToDraw);

	IMaterialSystem* matSystem = m_Game->m_MaterialSystem;

	/*int width, height;
	IMatRenderContext* rndrContext = matSystem->GetRenderContext();
	rndrContext->SetRenderTarget(NULL);
	rndrContext->GetRenderTargetDimensions(width, height);
	rndrContext->Release();
	std::cout << "RT - W: " << width << ", H: " << height << "\n";

	//rndrContext->SetRenderTarget(NULL);
	hkRenderView.fOriginal(ecx, setup, hudViewSetup, nClearFlags, whatToDraw);

	m_VR->m_RenderedNewFrame = true;

	return;*/

	//std::cout << "CViewSetup: " << &setup << "\n";
	//std::cout << "CViewSetup: " << &setup << ", " << &hudViewSetup << "\n";

	//std::cout << "CViewSetup.x: " << setup.x << "\n";

	//std::cout << "CViewSetup: " << &hudViewSetup << "\n";

	CViewSetup leftEyeView = setup;
	CViewSetup rightEyeView = setup;

	//std::cout << "Width: " << m_VR->m_RenderWidth << ", Height: " << m_VR->m_RenderHeight << ", Fov" << m_VR->m_Fov << ", Aspect: " << m_VR->m_Aspect << "\n";
	//std::cout << "Width: " << setup.width << ", Height: " << setup.height << ", Fov" << setup.fov << ", Aspect: " << setup.m_flAspectRatio << "\n";

	/*Vector a, b;

	a = m_VR->GetViewOriginLeft();
	b = m_VR->GetViewAngle();

	std::cout << "GetViewOriginLeft: (" << a.x << ", " << a.y << ", " << a.z << ") ";
	std::cout << "GetViewAngle: (" << b.x << ", " << b.y << ", " << b.z << ")\n";*/

	/*std::cout << "Hud - ";
	std::cout << "X: " << hudViewSetup.x << ", Y: " << hudViewSetup.y << " ";
	std::cout << "uX: " << hudViewSetup.m_nUnscaledX << ", uY: " << hudViewSetup.m_nUnscaledY << " ";
	std::cout << "w: " << hudViewSetup.width << ", h: " << hudViewSetup.height << " ";
	std::cout << "uw: " << hudViewSetup.m_nUnscaledWidth << ", uh: " << hudViewSetup.m_nUnscaledHeight << " ";*/
	/*std::cout << "Origin: (" << hudViewSetup.origin.x << ", " << hudViewSetup.origin.y << ", " << hudViewSetup.origin.z << ") ";
	std::cout << "Angles: (" << hudViewSetup.angles.x << ", " << hudViewSetup.angles.y << ", " << hudViewSetup.angles.z << ")\n";*/

	// Copies the game presentation RT address
	/*ITexture* rt = rndrContext->GetRenderTarget();
	rndrContext->Release();*/
	//hudViewSetup.x = 500;
	/*hudViewSetup.width = m_VR->m_RenderWidth;
	hudViewSetup.height = m_VR->m_RenderHeight;
	hudViewSetup.m_flAspectRatio = m_VR->m_Aspect;*/
	/*hudViewSetup.origin = m_VR->GetViewOriginLeft();
	hudViewSetup.angles = m_VR->GetViewAngle();*/

	m_VR->m_SetupOrigin = setup.origin;

	// Left eye CViewSetup
	leftEyeView.x = 0;
	leftEyeView.width = m_VR->m_RenderWidth;
	leftEyeView.height = m_VR->m_RenderHeight;
	leftEyeView.fov = m_VR->m_Fov;
	leftEyeView.fovViewmodel = m_VR->m_Fov;
	leftEyeView.m_flAspectRatio = m_VR->m_Aspect;
	leftEyeView.zNear = 6;
	leftEyeView.zNearViewmodel = 6;
	leftEyeView.origin = m_VR->GetViewOriginLeft();
	leftEyeView.angles = m_VR->GetViewAngle();

	Vector hmdAngle = m_VR->GetViewAngle();
	QAngle inGameAngle(hmdAngle.x, hmdAngle.y, hmdAngle.z);
	m_Game->m_EngineClient->SetViewAngles(inGameAngle);

	IMatRenderContext* rndrContext = matSystem->GetRenderContext();
	//rndrContext->SetRenderTarget(NULL);
	//ITexture* rt = hkGetFullScreenTexture.fOriginal();
	rndrContext->SetRenderTarget(m_VR->m_LeftEyeTexture);
	rndrContext->Release();
	hkRenderView.fOriginal(ecx, leftEyeView, hudViewSetup, nClearFlags, whatToDraw);
	m_PushedHud = false;

	// Right eye CViewSetup
	rightEyeView.x = 0;
	rightEyeView.width = m_VR->m_RenderWidth;
	rightEyeView.height = m_VR->m_RenderHeight;
	rightEyeView.fov = m_VR->m_Fov;
	rightEyeView.fovViewmodel = m_VR->m_Fov;
	rightEyeView.m_flAspectRatio = m_VR->m_Aspect;
	rightEyeView.zNear = 6;
	rightEyeView.zNearViewmodel = 6;
	rightEyeView.origin = m_VR->GetViewOriginRight();
	rightEyeView.angles = m_VR->GetViewAngle();

	//std::cout << "CViewSetup (Edited): " << &leftEyeView << ", " << &rightEyeView << "\n";

	/*hudViewSetup.width = m_VR->m_RenderWidth;
	hudViewSetup.height = m_VR->m_RenderHeight;
	hudViewSetup.m_flAspectRatio = m_VR->m_Aspect;
	hudViewSetup.origin = m_VR->GetViewOriginRight();
	hudViewSetup.angles = m_VR->GetViewAngle();*/

	rndrContext = matSystem->GetRenderContext();
	rndrContext->SetRenderTarget(m_VR->m_RightEyeTexture);
	rndrContext->Release();
	hkRenderView.fOriginal(ecx, rightEyeView, hudViewSetup, nClearFlags, whatToDraw);

	rndrContext = matSystem->GetRenderContext();
	rndrContext->SetRenderTarget(NULL);
	rndrContext->Release();

	/*int width, height;
	rndrContext->GetRenderTargetDimensions(width, height);
	std::cout << "RT - W: " << width << ", H: " << height << "\n";*/

	//ITexture* rt = matSystem->FindTexture("_rt_Fullscreen", "RenderTargets"); // TEXTURE_GROUP_RENDER_TARGET //_rt_Fullscreen //_rt_FullFrameFB1

	/*if (rt) {
		int sWidth = rt->GetActualWidth();
		int sHeight = rt->GetActualHeight();

		std::cout << "IsRenderTarget: " << rt->IsRenderTarget() << ", W: " << sWidth << ", H: " << sHeight << "\n";
	}*/

	// Copy right eye content to presentation RT
	/*rndrContext = matSystem->GetRenderContext();
	rndrContext->SetRenderTarget(NULL);
	rndrContext->CopyRenderTargetToTexture(m_VR->m_RightEyeTexture);
	rndrContext->Release();*/

	m_VR->m_RenderedNewFrame = true;
}

bool __fastcall Hooks::dCreateMove(void *ecx, void *edx, float flInputSampleTime, CUserCmd *cmd)
{
	if (!cmd->command_number)
		return hkCreateMove.fOriginal(ecx, flInputSampleTime, cmd);

	if (m_VR->m_IsVREnabled && m_VR->m_RoomscaleActive)
	{
		Vector setupOriginToHMD = m_VR->m_SetupOriginToHMD;
		setupOriginToHMD.z = 0;
		float distance = VectorLength(setupOriginToHMD);
		if (distance > 1)
		{
			float forwardSpeed = DotProduct2D(setupOriginToHMD, m_VR->m_HmdForward);
			float sideSpeed = DotProduct2D(setupOriginToHMD, m_VR->m_HmdRight);
			cmd->forwardmove += distance * forwardSpeed;
			cmd->sidemove += distance * sideSpeed;
		}
	}

	return false;
}

void __fastcall Hooks::dEndFrame(void *ecx, void *edx)
{
	return hkEndFrame.fOriginal(ecx);
}

void __fastcall Hooks::dCalcViewModelView(void *ecx, void *edx, const Vector &eyePosition, const QAngle &eyeAngles)
{
	Vector vecNewOrigin = eyePosition;
	QAngle vecNewAngles = eyeAngles;

	//std::cout << "dCalcViewModelView: (" << m_VR->m_IsVREnabled << ")\n";

	if (m_VR->m_IsVREnabled)
	{
		vecNewOrigin = m_VR->GetRecommendedViewmodelAbsPos();
		vecNewAngles = m_VR->GetRecommendedViewmodelAbsAngle();
	}

	return hkCalcViewModelView.fOriginal(ecx, vecNewOrigin, vecNewAngles);
}

int Hooks::dServerFireTerrorBullets(int playerId, const Vector &vecOrigin, const QAngle &vecAngles, int a4, int a5, int a6, float a7)
{
	Vector vecNewOrigin = vecOrigin;
	QAngle vecNewAngles = vecAngles;

	// Server host
	if (m_VR->m_IsVREnabled && playerId == m_Game->m_EngineClient->GetLocalPlayer())
	{
		vecNewOrigin = m_VR->GetRightControllerAbsPos();
		vecNewAngles = m_VR->GetRightControllerAbsAngle();
	}
	// Clients
	else if (m_Game->m_PlayersVRInfo[playerId].isUsingVR)
	{
		vecNewOrigin = m_Game->m_PlayersVRInfo[playerId].controllerPos;
		vecNewAngles = m_Game->m_PlayersVRInfo[playerId].controllerAngle;
	}

	return hkServerFireTerrorBullets.fOriginal(playerId, vecNewOrigin, vecNewAngles, a4, a5, a6, a7);
}

int Hooks::dClientFireTerrorBullets(int playerId, const Vector &vecOrigin, const QAngle &vecAngles, int a4, int a5, int a6, float a7)
{
	Vector vecNewOrigin = vecOrigin;
	QAngle vecNewAngles = vecAngles;
	
	if (m_VR->m_IsVREnabled && playerId == m_Game->m_EngineClient->GetLocalPlayer())
	{
		vecNewOrigin = m_VR->GetRightControllerAbsPos();
		vecNewAngles = m_VR->GetRightControllerAbsAngle();
	}

	return hkClientFireTerrorBullets.fOriginal(playerId, vecNewOrigin, vecNewAngles, a4, a5, a6, a7);
}


float __fastcall Hooks::dProcessUsercmds(void *ecx, void *edx, edict_t *player, void *buf, int numcmds, int totalcmds, int dropped_packets, bool ignore, bool paused)
{
	// Function pointer for CBaseEntity::entindex
	typedef int(__thiscall *tEntindex)(void *thisptr);
	static tEntindex oEntindex = (tEntindex)(m_Game->m_Offsets->CBaseEntity_entindex.address);

	IServerUnknown * pUnknown = player->m_pUnk;
	Server_BaseEntity *pPlayer = (Server_BaseEntity*)pUnknown->GetBaseEntity();

	int index = oEntindex(pPlayer);
	m_Game->m_CurrentUsercmdID = index;

	float result = hkProcessUsercmds.fOriginal(ecx, player, buf, numcmds, totalcmds, dropped_packets, ignore, paused);

	// check if swinging melee wep
	/*if (m_Game->m_PlayersVRInfo[index].isUsingVR && m_Game->m_PlayersVRInfo[index].isMeleeing)
	{
		typedef Server_WeaponCSBase *(__thiscall *tGetActiveWep)(void *thisptr);
		static tGetActiveWep oGetActiveWep = (tGetActiveWep)(m_Game->m_Offsets->GetActiveWeapon.address);
		Server_WeaponCSBase *curWep = oGetActiveWep(pPlayer);
		
		if (curWep)
		{
			int wepID = curWep->GetWeaponID();
			if (wepID == 19) // melee weapon
			{
				if (m_Game->m_PlayersVRInfo[index].isNewSwing)
				{
					m_Game->m_PlayersVRInfo[index].isNewSwing = false;
					curWep->entitiesHitThisSwing = 0;
				}

				typedef void *(__thiscall *tGetMeleeWepInfo)(void *thisptr);
				static tGetMeleeWepInfo oGetMeleeWepInfo = (tGetMeleeWepInfo)(m_Game->m_Offsets->GetMeleeWeaponInfo.address);
				void *meleeWepInfo = oGetMeleeWepInfo(curWep);

				Vector initialForward, initialRight, initialUp;
				QAngle::AngleVectors(m_Game->m_PlayersVRInfo[index].prevControllerAngle, &initialForward, &initialRight, &initialUp);
				Vector initialMeleeDirection = VectorRotate(initialForward, initialRight, 50.0);
				VectorNormalize(initialMeleeDirection);

				Vector finalForward, finalRight, finalUp;
				QAngle::AngleVectors(m_Game->m_PlayersVRInfo[index].controllerAngle, &finalForward, &finalRight, &finalUp);
				Vector finalMeleeDirection = VectorRotate(finalForward, finalRight, 50.0);
				VectorNormalize(finalMeleeDirection);

				Vector pivot;
				CrossProduct(initialMeleeDirection, finalMeleeDirection, pivot);
				VectorNormalize(pivot);

				float swingAngle = acos(DotProduct(initialMeleeDirection, finalMeleeDirection)) * 180 / 3.14159265;

				m_Game->m_Hooks->hkGetPrimaryAttackActivity.fOriginal(curWep, meleeWepInfo); // Needed to call TestMeleeSwingCollision

				m_Game->m_PerformingMelee = true;

				Vector traceDirection = initialMeleeDirection;
				int numTraces = 10;
				float traceAngle = swingAngle / numTraces;
				for (int i = 0; i < numTraces; ++i)
				{
					traceDirection = VectorRotate(traceDirection, pivot, traceAngle);
					m_Game->m_Hooks->hkTestMeleeSwingCollisionServer.fOriginal(curWep, traceDirection);
				}

				m_Game->m_PerformingMelee = false;
			}
		}
	}
	else
	{
		m_Game->m_PlayersVRInfo[index].isNewSwing = true;
	}*/

	m_Game->m_PlayersVRInfo[index].prevControllerAngle = m_Game->m_PlayersVRInfo[index].controllerAngle;

	return result;
}

int Hooks::dReadUsercmd(void *buf, CUserCmd *move, CUserCmd *from)
{
	hkReadUsercmd.fOriginal(buf, move, from);

	int i = m_Game->m_CurrentUsercmdID;
	if (move->tick_count < 0) // Signal for VR CUserCmd
	{
		move->tick_count *= -1;

		if (move->command_number < 0)
		{
			move->command_number *= -1;
			m_Game->m_PlayersVRInfo[i].isMeleeing = true;
		}
		else
		{
			m_Game->m_PlayersVRInfo[i].isMeleeing = false;
		}

		m_Game->m_PlayersVRInfo[i].isUsingVR = true;
		m_Game->m_PlayersVRInfo[i].controllerAngle.x = (float)move->mousedx / 10;
		m_Game->m_PlayersVRInfo[i].controllerAngle.y = (float)move->mousedy / 10;
		m_Game->m_PlayersVRInfo[i].controllerPos.x = move->viewangles.z;
		m_Game->m_PlayersVRInfo[i].controllerPos.y = move->upmove;

		// Decode controllerAngle.z
		int rollEncoding = move->command_number / 10000000;
		move->command_number -= rollEncoding * 10000000;
		m_Game->m_PlayersVRInfo[i].controllerAngle.z = (rollEncoding * 2) - 180;

		// Decode viewangles.x
		int decodedZInt = (move->viewangles.x / 10000);
		float decodedAngle = abs((float)(move->viewangles.x - (decodedZInt * 10000)) / 10);
		decodedAngle -= 360;
		float decodedZ = (float)decodedZInt / 10;

		m_Game->m_PlayersVRInfo[i].controllerPos.z = decodedZ;

		move->viewangles.x = decodedAngle;
		move->viewangles.z = 0;
		move->upmove = 0;
	}
	else
	{
		m_Game->m_PlayersVRInfo[i].isUsingVR = false;
	}
	return 1;
}

void __fastcall Hooks::dWriteUsercmdDeltaToBuffer(void *ecx, void *edx, int a1, void *buf, int from, int to, bool isnewcommand) 
{
	return hkWriteUsercmdDeltaToBuffer.fOriginal(ecx, a1, buf, from, to, isnewcommand);
}

int Hooks::dWriteUsercmd(void *buf, CUserCmd *to, CUserCmd *from)
{
	if (m_VR->m_IsVREnabled)
	{
		CInput *m_Input = **(CInput ***)(m_Game->m_Offsets->g_pppInput.address);
		CVerifiedUserCmd *pVerifiedCommands = *(CVerifiedUserCmd **)((uintptr_t)m_Input + 0xF0);
		CVerifiedUserCmd *pVerified = &pVerifiedCommands[(to->command_number) % 150];

		// Signal to the server that this CUserCmd has VR info
		to->tick_count *= -1;

		int originalCommandNum = to->command_number;

		QAngle controllerAngles = m_VR->GetRightControllerAbsAngle();
		to->mousedx = controllerAngles.x * 10; // Strip off 2nd decimal to save bits.
		to->mousedy = controllerAngles.y * 10;
		int rollEncoding = (((int)controllerAngles.z + 180) / 2 * 10000000);
		to->command_number += rollEncoding;

		if (VectorLength(m_VR->m_RightControllerPose.TrackedDeviceVel) > 1.1)
		{
			to->command_number *= -1; // Signal to server that melee swing in motion
		}

		Vector controllerPos = m_VR->GetRightControllerAbsPos();
		to->viewangles.z = controllerPos.x;
		to->upmove = controllerPos.y;

		// Space in CUserCmd is tight, so encode viewangle.x and controllerPos.z together.
		// Encoding will overflow if controllerPos.z goes beyond +-21474.8
		float xAngle = to->viewangles.x;
		int encodedAngle = (xAngle + 360) * 10;
		int encoding = (int)(controllerPos.z * 10) * 10000;
		encoding += encoding < 0 ? -encodedAngle : encodedAngle;
		to->viewangles.x = encoding;

		hkWriteUsercmd.fOriginal(buf, to, from);

		to->viewangles.x = xAngle;
		to->tick_count *= -1;
		to->viewangles.z = 0;
		to->upmove = 0;
		to->command_number = originalCommandNum;

		// Must recalculate checksum for the edited CUserCmd or gunshots will sound
		// terrible in multiplayer.
		/*pVerified->m_cmd = *to;
		pVerified->m_crc = to->GetChecksum();*/
		return 1;
	}
	return hkWriteUsercmd.fOriginal(buf, to, from);
}

void Hooks::dAdjustEngineViewport(int &x, int &y, int &width, int &height)
{
	hkAdjustEngineViewport.fOriginal(x, y, width, height);
}

void Hooks::dViewport(void *ecx, void *edx, int x, int y, int width, int height)
{
	hkViewport.fOriginal(ecx, x, y, width, height);
}

void Hooks::dGetViewport(void *ecx, void *edx, int &x, int &y, int &width, int &height)
{
	hkGetViewport.fOriginal(ecx, x, y, width, height);
}

int Hooks::dPrimaryAttackServer(void *ecx, void *edx)
{
	return hkPrimaryAttackServer.fOriginal(ecx);
}

void Hooks::dItemPostFrameServer(void *ecx, void *edx)
{
	hkItemPostFrameServer.fOriginal(ecx);
}

int Hooks::dGetPrimaryAttackActivity(void *ecx, void *edx, void *meleeInfo)
{
	return hkGetPrimaryAttackActivity.fOriginal(ecx, meleeInfo);
}

Vector *Hooks::dEyePosition(void *ecx, void *edx, Vector *eyePos)
{
	Vector *result = hkEyePosition.fOriginal(ecx, eyePos);

	//std::cout << "dEyePosition X:" << result->x << ", Y: " << result->y << ", Z: " << result->z << "\n";

	if (m_Game->m_PerformingMelee)
	{
		int i = m_Game->m_CurrentUsercmdID;
		*result = m_Game->m_PlayersVRInfo[i].controllerPos;
	}

	return result;
}

Vector* Hooks::dWeapon_ShootPosition(void* ecx, void* edx, Vector* eyePos)
{
	Vector* result = hkWeapon_ShootPosition.fOriginal(ecx, eyePos);

	//std::cout << "dWeapon_ShootPosition X:" << result->x << ", Y: " << result->y << ", Z: " << result->z << "\n";

	return result;
}


void Hooks::dDrawModelExecute(void *ecx, void *edx, void *state, const ModelRenderInfo_t &info, void *pCustomBoneToWorld)
{
	if (m_Game->m_SwitchedWeapons)
		m_Game->m_CachedArmsModel = false;

	bool hideArms = m_Game->m_IsMeleeWeaponActive || m_VR->m_HideArms;
	
	if (info.pModel && hideArms && !m_Game->m_CachedArmsModel)
	{
		std::string modelName = m_Game->m_ModelInfo->GetModelName(info.pModel);
		if (modelName.find("/arms/") != std::string::npos)
		{
			m_Game->m_ArmsMaterial = m_Game->m_MaterialSystem->FindMaterial(modelName.c_str(), "Model textures");
			m_Game->m_ArmsModel = info.pModel;
			m_Game->m_CachedArmsModel = true;
		}
	}

	if (info.pModel && info.pModel == m_Game->m_ArmsModel && hideArms)
	{
		m_Game->m_ArmsMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		m_Game->m_ModelRender->ForcedMaterialOverride(m_Game->m_ArmsMaterial);
		hkDrawModelExecute.fOriginal(ecx, state, info, pCustomBoneToWorld);
		m_Game->m_ModelRender->ForcedMaterialOverride(NULL);
		return;
	}

	hkDrawModelExecute.fOriginal(ecx, state, info, pCustomBoneToWorld);
}

void Hooks::dPushRenderTargetAndViewport(void *ecx, void *edx, ITexture *pTexture, ITexture *pDepthTexture, int nViewX, int nViewY, int nViewW, int nViewH)
{
	if (!m_VR->m_CreatedVRTextures)
		return hkPushRenderTargetAndViewport.fOriginal(ecx, pTexture, pDepthTexture, nViewX, nViewY, nViewW, nViewH);

	//std::cout << "dPushRenderTargetAndViewport: " << m_PushHUDStep << "\n";

	if (m_PushHUDStep == 2)
		++m_PushHUDStep;
	else
		m_PushHUDStep = -999;

	// RenderView calls PushRenderTargetAndViewport multiple times with different textures. 
	// When the call order goes PopRenderTargetAndViewport -> IsSplitScreen -> PrePushRenderTarget -> PushRenderTargetAndViewport,
	// then it pushed the HUD/GUI render target to the RT stack.
	if (m_PushHUDStep == 3)
	{
		pTexture = m_VR->m_HUDTexture;

		IMatRenderContext *renderContext = m_Game->m_MaterialSystem->GetRenderContext();
		renderContext->ClearBuffers(false, true, true);

		hkPushRenderTargetAndViewport.fOriginal(ecx, pTexture, pDepthTexture, nViewX, nViewY, nViewW, nViewH);

		renderContext->OverrideAlphaWriteEnable(true, true);
		renderContext->ClearColor4ub(0, 0, 0, 0);
		renderContext->ClearBuffers(true, false);

		m_VR->m_RenderedHud = true;
		m_PushedHud = true;
	}
	else
	{
		hkPushRenderTargetAndViewport.fOriginal(ecx, pTexture, pDepthTexture, nViewX, nViewY, nViewW, nViewH);
	}
}

void Hooks::dPopRenderTargetAndViewport(void *ecx, void *edx)
{
	if (!m_VR->m_CreatedVRTextures)
		return hkPopRenderTargetAndViewport.fOriginal(ecx);

	//std::cout << "dPopRenderTargetAndViewport: " << m_PushHUDStep << "\n";

	m_PushHUDStep = 0;

	if (m_PushedHud)
	{
		m_Game->m_MaterialSystem->GetRenderContext()->OverrideAlphaWriteEnable(false, true);
		m_Game->m_MaterialSystem->GetRenderContext()->ClearColor4ub(0, 0, 0, 255);
	}

	hkPopRenderTargetAndViewport.fOriginal(ecx);
}

void Hooks::dVGui_Paint(void *ecx, void *edx, int mode)
{
	if (!m_VR->m_CreatedVRTextures || m_VR->m_Game->m_VguiSurface->IsCursorVisible())
		return hkVgui_Paint.fOriginal(ecx, mode);

	if (m_PushedHud)
		mode = PAINT_UIPANELS | PAINT_INGAMEPANELS;

	hkVgui_Paint.fOriginal(ecx, mode);
}

int Hooks::dIsSplitScreen()
{
	//std::cout << "dIsSplitScreen: " << m_PushHUDStep << "\n";

	if (m_PushHUDStep == 0)
		++m_PushHUDStep;
	else
		m_PushHUDStep = -999;

	return hkIsSplitScreen.fOriginal();
}

DWORD *Hooks::dPrePushRenderTarget(void *ecx, void *edx, int a2)
{
	//std::cout << "dPrePushRenderTarget: " << m_PushHUDStep << "\n";

	if (m_PushHUDStep == 1)
		++m_PushHUDStep;
	else
		m_PushHUDStep = -999;

	return hkPrePushRenderTarget.fOriginal(ecx, a2);
}


float __fastcall Hooks::dTraceFirePortal(void* ecx, void* edx, const Vector& vTraceStart, const Vector& vDirection, bool bPortal2, int iPlacedBy, void* tr) //trace_tx& tr, Vector& vFinalPosition //  , Vector& vFinalPosition, QAngle& qFinalAngles, int iPlacedBy, bool bTest /*= false*/
{
	Vector vNewTraceStart = vTraceStart;
	Vector vNewDirection = vDirection;

	if (m_VR->m_IsVREnabled && iPlacedBy == 2)
	{
		vNewTraceStart = m_VR->m_RightControllerPosAbs;
		vNewDirection = m_VR->m_RightControllerForward;
	}

	return hkTraceFirePortal.fOriginal(ecx, vNewTraceStart, vNewDirection, bPortal2, iPlacedBy, tr);
}

void __fastcall Hooks::dHandlePortallingClient(void* ecx, void* edx)
{
	DWORD *cPortalPlayer = (DWORD*)*((DWORD*)ecx + 0x1);
	CBaseEntity* pBaseEntity = (CBaseEntity*)cPortalPlayer;

	//Vector* eyeAnglesBefore = hkEyeAngles.fOriginal(pBaseEntity);
	QAngle angAbsRotationBefore = pBaseEntity->m_angAbsRotation;

	hkHandlePortallingClient.fOriginal(ecx);

	//Vector* eyeAnglesAfter = hkEyeAngles.fOriginal(pBaseEntity);
	QAngle angAbsRotationAfter = pBaseEntity->m_angAbsRotation;

	if (angAbsRotationBefore.y != angAbsRotationAfter.y) {
		float difference = angAbsRotationAfter.y - angAbsRotationBefore.y;
		m_VR->m_RotationOffset += difference;
	}

	return;
}

Vector *__fastcall Hooks::dEyeAngles(void* ecx, void* edx)
{
	return hkEyeAngles.fOriginal(ecx);
}
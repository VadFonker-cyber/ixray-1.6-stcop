// EngineAPI.h: interface for the CEngineAPI class.
//
//****************************************************************************
// Support for extension DLLs
//****************************************************************************
#pragma once
#include <array>

#include "IRender_RHI.h"

// Abstract 'Pure' class for DLL interface
class ENGINE_API DLL_Pure {
public:
	CLASS_ID				CLS_ID;

	DLL_Pure(void *params)	{CLS_ID=0; };
	DLL_Pure()				{CLS_ID=0; };
	virtual	DLL_Pure*		_construct		()	{ return this; 	}
	virtual ~DLL_Pure()		{};
};

// Class creation/destroying interface
extern "C" 
{
typedef  DLL_Pure*	  __cdecl Factory_Create	(CLASS_ID	CLS_ID);
typedef  void		  __cdecl Factory_Destroy	(DLL_Pure*	O);
};

enum class EditorUI : u8
{
	Shaders,
	Weather,
	DebugDraw,
	CmdVars,
	CmdConsole,
	LuaDebug,
	LuaCodespace,
	ActorInfos,
	HudAdjust,
	ScenesViewer,
	Count
};

class ENGINE_API		CEngineAPI
{
private:
	HMODULE				hGame;
	HMODULE				hRender;
	HMODULE				hRenderRHI;

public:
	HMODULE				hGameSpy;

	std::array<bool, static_cast<u8>(EditorUI::Count)> EditorStates = {};

public:
	Factory_Create*		pCreate;
	Factory_Destroy*	pDestroy;
	void				Initialize	();
	
	void				InitializeNotDedicated();
	void				Destroy		();

	void				CreateRendererList();

	CEngineAPI	();
	~CEngineAPI	();
};

#define NEW_INSTANCE(a)		Engine.External.pCreate(a)
#define DEL_INSTANCE(a)		{ Engine.External.pDestroy(a); a=NULL; }

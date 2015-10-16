#ifndef ENGINE_H
#define ENGINE_H

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9tex.h>

#include "SceneMgr.h"

//struct CUSTOMVERTEX {FLOAT X, Y, Z, RHW; DWORD COLOR;};
//#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class Engine
{
public:
	Engine(){}
	~Engine(){}
	void Initialize(HWND hWnd, int width, int height);
	void Update();
	void Close();

protected:
	LPDIRECT3D9 mD3D;    // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 mD3DDevice;    // the pointer to the device class
	
	SceneMgr sceneMgr;

	int mWidth;
	int mHeight;
};

#endif

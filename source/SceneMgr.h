#ifndef SCENEMGR_H
#define SCENEMGR_H

#include <d3d9.h>
#include <d3dx9tex.h>

#include "Script.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class SceneMgr
{
public:
	SceneMgr(){}
	~SceneMgr(){}
	void Initialize(LPDIRECT3DDEVICE9 pD3DDevice, HWND hWnd);
	void Update(int width, int height);
	void Close();

private:
	void HandleInput();
	void LoadScene(char * filename);
	void Render(int width, int height);

	LPDIRECT3DTEXTURE9 LookupTexture(std::string name);

	typedef struct
	{
		std::string * name;
		LPDIRECT3DTEXTURE9 texture;
	} entry;
	std::vector <entry> textureEntries;

	typedef struct
	{
		LPDIRECT3DTEXTURE9 texture;
		float x;
		float y;
	} SceneTile;
	std::vector <SceneTile> mapTileEntries;

	LPDIRECT3DTEXTURE9 characterTexture;

	float characterX;
	float characterY;
	static const int MAP_COL = 35;
	static const int MAP_ROW = 20;
	static const int CHAR_OFFSET_X = 17;
	static const int CHAR_OFFSET_Y = 10;


	static const int CHAR_STEP = 15;
	int characterDX;
	int characterDY;

	bool * mMapInfo;
	int mMapWidth;
	LPD3DXSPRITE mSprite;

	Script * mScript;

	LPDIRECT3DDEVICE9 mD3DDevice;
	LPDIRECTINPUT8 fDI; // Root DirectInput Interface
	LPDIRECTINPUTDEVICE8 fDIKeyboard; // The keyboard device
};
#endif
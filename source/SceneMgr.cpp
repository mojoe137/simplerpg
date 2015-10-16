#include "SceneMgr.h"


void SceneMgr::Initialize(LPDIRECT3DDEVICE9 pD3DDevice, HWND hWnd)
{
	mMapInfo = NULL;
	mD3DDevice = pD3DDevice;
	DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&fDI,
		NULL
		);

	if (fDI == NULL)
	{
		OutputDebugString("Failed to connect to direct input\r\n");
	}
	// Create the connection to the keyboard device
	fDI->CreateDevice(GUID_SysKeyboard, &fDIKeyboard, NULL);
	if (fDIKeyboard)
	{
		fDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
		fDIKeyboard->SetCooperativeLevel(
			hWnd,
			DISCL_FOREGROUND | DISCL_EXCLUSIVE
			);
		fDIKeyboard->Acquire();
	}
	else
	{
		OutputDebugString("Failed to connect to keyboard\r\n");
	}

	characterX = 0;
	characterY = 0;
	characterDX = 0;
	characterDY = 0;

	LoadScene("./../scripts/map.txt");

	D3DXCreateTextureFromFileEx(pD3DDevice, "./../Content/Images/char35.png", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0xFFFFFFFF, NULL, NULL, &characterTexture);

	if( !SUCCEEDED(D3DXCreateSprite(pD3DDevice,&mSprite)))
	{
		OutputDebugString("Failed to create sprite");
	}
}

void SceneMgr::Update(int width, int height)
{
	HandleInput();
	Render(width, height);
}

void SceneMgr::Render(int width, int height)
{
	mSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(((float)width)/(MAP_COL*32),((float)height)/(MAP_ROW*32));

	//Draw the background map
	for( int i = 0; i < mapTileEntries.size(); i++ )
	{
		D3DXVECTOR2 translation((float)width/MAP_COL*(mapTileEntries[i].x-(characterX - CHAR_OFFSET_X + ((float)characterDX)/CHAR_STEP)),(float)height/MAP_ROW*(mapTileEntries[i].y  - (characterY - CHAR_OFFSET_Y + ((float)characterDY)/CHAR_STEP)));
		D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&translation);
		mSprite->SetTransform(&mat);
		mSprite->Draw(mapTileEntries[i].texture,NULL,NULL,NULL,0xFFFFFFFF);
	}

	//Draw the character
	D3DXVECTOR2 translation(((float)width)/MAP_COL*CHAR_OFFSET_X,((float)height)/MAP_ROW*CHAR_OFFSET_Y);
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&translation);
	mSprite->SetTransform(&mat);
	mSprite->Draw(characterTexture,NULL,NULL,NULL,0xFFFFFFFF);

	mSprite->End();
}

void SceneMgr::Close()
{
	delete mScript;
	mSprite->Release();
	for( int i = 0; i < textureEntries.size(); i++ )
	{
		textureEntries[i].texture->Release();
	}
	characterTexture->Release();
}

LPDIRECT3DTEXTURE9 SceneMgr::LookupTexture(std::string name)
{
	for( int i = 0; i < textureEntries.size(); i++ )
	{
		if( textureEntries[i].name->compare(name) == 0 )
		{
			return textureEntries[i].texture;
		}
	}
	return textureEntries[0].texture;
}

void SceneMgr::HandleInput()
{
		// The following macro allows you to test if
	// a key is currently pressed
	#define KEYDOWN(name, key) (name[key] & 0x80)
	// Here we set up an array of 256 chars. This will hold the
	// entire keyboard state after it has been retrieved.
	char fDIKeyboardState[256];
	HRESULT hr;
	// get the keyboard state
	hr = fDIKeyboard->GetDeviceState(
		sizeof(fDIKeyboardState),
		(LPVOID)&fDIKeyboardState
		);
	if (FAILED(hr))
	{
		// It's possible that we lost access to the keyboard
		// Here we acquire access to the keyboard again
		fDIKeyboard->Acquire();
	}

	char temp[50];
	sprintf(temp, "X:%f Y%f\r\n", characterX, characterY);
	OutputDebugString(temp);

	if( characterDY == 0 && characterDX == 0 )
	{
		if (KEYDOWN(fDIKeyboardState, DIK_UP) && mMapInfo[(int)(characterX + (characterY-1) * mMapWidth)])
		{
			characterDY = -1;
		}
		else if (KEYDOWN(fDIKeyboardState, DIK_DOWN)&& mMapInfo[(int)(characterX + (characterY+1) * mMapWidth)])
		{
			characterDY = 1;
		}
		else if (KEYDOWN(fDIKeyboardState, DIK_RIGHT)&& mMapInfo[(int)(characterX+1 + characterY * mMapWidth)])
		{
			characterDX = 1;
		}
		else if (KEYDOWN(fDIKeyboardState, DIK_LEFT)&& mMapInfo[(int)(characterX-1 + characterY * mMapWidth)])
		{
			characterDX = -1;
		}
	}

	if( characterDX > 0 )
	{
		characterDX++;
		if( characterDX >= CHAR_STEP )
		{
			characterX++;
			characterDX = 0;
		}
	}
	if( characterDX < 0 )
	{
		characterDX--;
		if( characterDX <= -CHAR_STEP )
		{
			characterX--;
			characterDX = 0;
		}
	}
	if( characterDY > 0 )
	{
		characterDY++;
		if( characterDY >= CHAR_STEP )
		{
			characterY++;
			characterDY = 0;
		}
	}
	if( characterDY < 0 )
	{
		characterDY--;
		if( characterDY <= -CHAR_STEP )
		{
			characterY--;
			characterDY = 0;
		}
	}
}

void SceneMgr::LoadScene(char * filename)
{
	mScript = new Script(filename);

	textureEntries.clear();
	mapTileEntries.clear();

	if( mMapInfo != NULL )
	{
		delete mMapInfo;
	}

	//Load the tile
	std::string tile = mScript->GetString("TILES");

	while( tile.length() > 0 )
	{
		int index = tile.find_first_of("\n");
		if( index >= 1 )
		{
			int spaceIndex = tile.find_first_of(" ");
			int newline = tile.find_first_of("\n");
			entry temp;
			temp.name = new std::string(tile.substr(0,spaceIndex));
			std::string filename = "./../Content/Images/" + tile.substr(spaceIndex+1, newline - spaceIndex-1);
			if( !SUCCEEDED(D3DXCreateTextureFromFile(mD3DDevice,  filename.c_str(), &temp.texture)))
			{
				OutputDebugString("Failed to create texture:");
				OutputDebugString(filename.c_str());
				OutputDebugString("\n");
			}
			else
			{
				textureEntries.push_back(temp);
			}
		}
		tile = tile.substr(index+1);
	}

	//Load the map
	std::string originalMap = mScript->GetString("MAP");
	std::string map;
	map.clear();

	for( int i = 0; i < originalMap.length(); i++ )
	{
		if(originalMap[i] != '\r' && originalMap[i] != '\n')
		{
			map.push_back(originalMap[i]);
		}
	}

	mMapInfo = new bool[map.length()];

	int width = mScript->GetInt("WIDTH");
	mMapWidth = width;

	characterX = mScript->GetInt("CHAR_X");
	characterY = mScript->GetInt("CHAR_Y");

	int x = 0;
	int y = 0;
	
	for(int i = 0; i < map.length(); i++ )
	{
		SceneTile newTile;
		newTile.x = x;
		newTile.y = y;
		newTile.texture = LookupTexture(map.substr(i,1));
		mMapInfo[i] = map[i] < 'a';
		mapTileEntries.push_back(newTile);
		x++;
		if( x >= width )
		{
			x = 0;
			y++;
		}
	}

}

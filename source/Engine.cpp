#include "Engine.h"
#include "Script.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

// this function initializes and prepares Direct3D for use
void Engine::Initialize(HWND hWnd, int width, int height)
{
	mWidth = width;
	mHeight = height;
	mD3D = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS mD3Dpp;

    ZeroMemory(&mD3Dpp, sizeof(mD3Dpp));
    mD3Dpp.Windowed = TRUE;
    mD3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    mD3Dpp.hDeviceWindow = hWnd;
    mD3Dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    mD3Dpp.BackBufferWidth = width;
    mD3Dpp.BackBufferHeight = height;

    // create a device class using this information and the info from the mD3Dpp stuct
    mD3D->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &mD3Dpp,
                      &mD3DDevice);

	sceneMgr.Initialize(mD3DDevice, hWnd);

}


// this is the function used to render a single frame
void Engine::Update(void)
{
    mD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    mD3DDevice->BeginScene();

	sceneMgr.Update(mWidth, mHeight);

    mD3DDevice->EndScene();
    mD3DDevice->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void Engine::Close(void)
{
	sceneMgr.Close();
    mD3DDevice->Release();    // close and release the 3D device
    mD3D->Release();    // close and release Direct3D
}



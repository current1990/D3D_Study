#ifndef _TEXCUBE_HPP_
#define _TEXCUBE_HPP_

#include "d3dUtility.h"
#include "cube.hpp"

#pragma comment(lib, "d3dUtility.lib")

extern IDirect3DDevice9 *pD3DDEV;
extern IDirect3DTexture9 *pTex;
extern cube *Box;

bool Setup()
{
	//Set Cube
	Box = new cube(pD3DDEV);

	//Set Light
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Diffuse   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular  = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	pD3DDEV->SetLight(0, &light);
	pD3DDEV->LightEnable(0, true);

	pD3DDEV->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pD3DDEV->SetRenderState(D3DRS_SPECULARENABLE, true);

	//Set Texture
	D3DXCreateTextureFromFile(pD3DDEV,
							  TEXT("crate.jpg"),
							  &pTex);

	//Set Default Tex Filter
	pD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//Set Projection
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 2,
							   800.0f / 600.0f,
							   1.0f, 100.0f);
	pD3DDEV->SetTransform(D3DTS_PROJECTION, &proj);

	::MessageBox(0, TEXT("\'WASD\' for view control\n\'123\' for Sampling Mode"),
				 TEXT("Tips"), MB_OK);

	return true;
}

bool Render(float DeltaTime)
{
	pD3DDEV->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);

	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		//Set Tex Filter
		if( GetAsyncKeyState('1') & 0x8000f )
		{
			::MessageBox(0, TEXT("Anisotropic"), TEXT("Change Mode"), MB_OK);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
		}
		if( GetAsyncKeyState('2') & 0x8000f )
		{
			::MessageBox(0, TEXT("Linear"), TEXT("Change Mode"), MB_OK);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}
		if( GetAsyncKeyState('3') & 0x8000f )
		{
			::MessageBox(0, TEXT("Point"), TEXT("Change Mode"), MB_OK);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		}

		//Set Camera Position
		static D3DXMATRIX cam;

		static float angle = 3.0f * D3DX_PI / 2.0f;
		static float height = 2.0f;

		if( ::GetAsyncKeyState('W') & 0x8000f )
			height += 2 * DeltaTime;
		if( ::GetAsyncKeyState('S') & 0x8000f )
			height -= 2 * DeltaTime;
		if( ::GetAsyncKeyState('A') & 0x8000f )
			angle -= DeltaTime;
		if( ::GetAsyncKeyState('D') & 0x8000f )
			angle += DeltaTime;

		D3DXVECTOR3 EyeAt(sinf(angle) * 3.0f, height, cosf(angle) * 3.0f);
		static D3DXVECTOR3 LookAt(0.0f, 0.0f, 0.0f);
		static D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		
		D3DXMatrixLookAtLH(&cam, &EyeAt, &LookAt, &up);
		pD3DDEV->SetTransform(D3DTS_VIEW, &cam);

		pD3DDEV->SetMaterial(&d3d::WHITE_MTRL);
		pD3DDEV->SetTexture(0, pTex);

		Box->Draw(0, 0, 0);
		pD3DDEV->EndScene();
	}

	pD3DDEV->Present(0, 0, 0, 0);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DDevice9>(pD3DDEV);
	d3d::Release<IDirect3DTexture9>(pTex);
	d3d::Delete<cube>(Box);
}

#endif
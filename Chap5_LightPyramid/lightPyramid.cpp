#include "lightPyramid.h"

struct Vertex
{
	float _x, _y, _z;
	float _nx, _ny, _nz;
	Vertex(float x, float y, float z,
		   float nx, float ny, float nz) :
		   _x(x), _y(y), _z(z),
		   _nx(nx), _ny(ny), _nz(nz)
	{
	}
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

bool Setup()
{
	//Setup Pyramid
	pD3DDEV->CreateVertexBuffer(12 * sizeof(Vertex),
								D3DUSAGE_WRITEONLY,
								Vertex::FVF,
								D3DPOOL_MANAGED,
								&pVB,
								NULL);

	Vertex *pyramid;
	pVB->Lock(0, 12 * sizeof(Vertex), (void**)&pyramid, 0);

	//front
	pyramid[0] = Vertex(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
	pyramid[1] = Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, -0.707f);
	pyramid[2] = Vertex(1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);

	//left
	pyramid[3] = Vertex(-1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f);
	pyramid[4] = Vertex(0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f);
	pyramid[5] = Vertex(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);

	//right
	pyramid[6] = Vertex(1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
	pyramid[7] = Vertex(0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f);
	pyramid[8] = Vertex(1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f);

	//back
	pyramid[9] = Vertex(1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
	pyramid[10] = Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
	pyramid[11] = Vertex(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);

	pVB->Unlock();

	//Set pyramid material
	D3DMATERIAL9 pyramidMtrl ={ WHITE, WHITE, WHITE, BLACK, 5.0f };

	pD3DDEV->SetMaterial(&pyramidMtrl);

	//Setup light
	D3DLIGHT9 light1;
	::ZeroMemory(&light1, sizeof(light1));
	light1.Type = D3DLIGHT_DIRECTIONAL;
	light1.Ambient = WHITE * 0.3f;
	light1.Diffuse = RED;
	light1.Specular = WHITE * 0.7f;
	light1.Direction ={ 1.0f, 0.0f, 0.0f };

	pD3DDEV->SetLight(0, &light1);
	pD3DDEV->LightEnable(0, true);

	//Setup Camera
	D3DXMATRIX cam;
	D3DXVECTOR3 EyeAt(0.0f, 2.0f, -3.0f);
	D3DXVECTOR3 LookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cam, &EyeAt, &LookAt, &up);
	pD3DDEV->SetTransform(D3DTS_VIEW, &cam);


	//Set Render states
	pD3DDEV->SetRenderState(D3DRS_LIGHTING, true);
	pD3DDEV->SetRenderState(D3DRS_NORMALIZENORMALS, true);	//Normalize Normals
	pD3DDEV->SetRenderState(D3DRS_SPECULARENABLE, true);

	return true;
}

bool Render(float DeltaTime)
{
	pD3DDEV->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);

	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		//Set Rotation
		D3DXMATRIX rotation;

		static float y = 0.0f;
		D3DXMatrixRotationY(&rotation, y);

		y += DeltaTime;
		if( y >= D3DX_PI * 2.0f )
			y = 0.0f;

		pD3DDEV->SetTransform(D3DTS_WORLD, &rotation);

		//Set projection
		RECT client;
		GetClientRect(FindWindow(NULL, TEXT("Pyramid")), &client);
		D3DXMATRIX proj;
		float X = GetSystemMetrics(0);
		D3DXMatrixPerspectiveFovLH(&proj,
								   D3DX_PI / 2.0f,
								   static_cast<float>((client.right - client.left) / (client.bottom - client.top)),
								   1.0f,
								   100.0f);
		pD3DDEV->SetTransform(D3DTS_PROJECTION, &proj);

		pD3DDEV->SetStreamSource(0, pVB, 0, sizeof(Vertex));
		pD3DDEV->SetFVF(Vertex::FVF);
		pD3DDEV->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);

		pD3DDEV->EndScene();
	}
	pD3DDEV->Present(0, 0, 0, 0);

	return true;
}

void Cleanup()
{
	Release<IDirect3DDevice9>(pD3DDEV);
	Release<IDirect3DVertexBuffer9>(pVB);
}
#include "vertices.h"

//Customized Vertex format
#define D3DFVF_COLOREDVERTEX D3DFVF_XYZRHW | D3DFVF_DIFFUSE

struct COLORED_VERTEX
{
	float x, y, z, rhw;
	DWORD color;
};


bool Setup()
{
	//vertices forming a triangle
	COLORED_VERTEX vertices[] =
	{
		{ 400.0f, 150.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
		{ 600.0f, 450.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
		{ 200.0f, 450.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
	};

	//Create the Vertex Buffer
	if( FAILED(pD3DDEV->CreateVertexBuffer(3 * sizeof(COLORED_VERTEX), D3DUSAGE_WRITEONLY, D3DFVF_COLOREDVERTEX,
		D3DPOOL_MANAGED, &pVB, NULL)) )
	{
		::MessageBox(0, TEXT("CreateVertexBuffer Failuer"), TEXT("ERROR"), MB_OK);
		return false;
	}

	//Fill the Vertex Buffer
	COLORED_VERTEX *pVertices;
	if( FAILED(pVB->Lock(0, 0, (void**)&pVertices, 0)) )
	{
		::MessageBox(0, TEXT("VertexBuffer Lock Failuer"), TEXT("ERROR"), MB_OK);
		return false;
	}
	pVertices[0] = vertices[0];
	pVertices[1] = vertices[1];
	pVertices[2] = vertices[2];

	pVB->Unlock();

	//pD3DDEV->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

	return true;
}

bool Render(float DelteTime)
{

	//Set BackBuffer to white
	pD3DDEV->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//Set Wireframe
	//pD3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//Render
	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		//All actual render operations should be enclosed by BeginScene and EndScene

		//Step 1. Set Source of data stream
		pD3DDEV->SetStreamSource(0, pVB, 0, sizeof(COLORED_VERTEX));

		//Step 2. Set Vertex Format, aka. FVF
		pD3DDEV->SetFVF(D3DFVF_COLOREDVERTEX);

		//Step 3(Optional). Set Index Buffer, only needed when using index buffer

		//Step 4. Draw
		pD3DDEV->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		//Finish Drawing
		pD3DDEV->EndScene();
	}

	//Present BackBuffer
	pD3DDEV->Present(NULL, NULL, NULL, NULL);

	return true;
}

void CleanUp()
{
	d3d::Release<IDirect3DVertexBuffer9>(pVB);
}
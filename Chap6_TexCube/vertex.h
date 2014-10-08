#ifndef _VERTEX_H_
#define _VERTEX_H_

struct Vertex
{
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	Vertex(float x, float y, float z,
		   float nx, float ny, float nz,
		   float u, float v) :
		   _x(x), _y(y), _z(z), _nx(nx), _ny(ny), _nz(nz),
		   _u(u), _v(v) {}

	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

#endif //_VERTEX_H_
#pragma once

class Texture
{
public:
	Texture() {}
	~Texture() {}
	Texture* LoadFromFile(HWND hwnd, const string& path);

	HDC GetDC() const { return _hdc; }
	VectorInt GetSize() const { return _size; }
	void SetSize(const VectorInt& size) { _size = size; }
	uint32 GetTransparent() const { return _transparent; }
	void SetTransparent(uint32 transparent) { _transparent = transparent; }

private:
	HDC _hdc = {};
	HBITMAP _bitmap = {};
	VectorInt _size;
	uint32 _transparent = RGB(255, 255, 0);
};


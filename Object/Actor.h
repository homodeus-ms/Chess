#pragma once

class Texture;

class Actor
{
public:
	Actor(LayerType type) : _layerType(type) {}
	virtual ~Actor();

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

	LayerType GetLayerType() const { return _layerType; }
	void SetTexture(Texture* texture) { _texture = texture; }
	Texture* GetTexture() const { return _texture; }
	VectorInt GetPos() const { return _pos; }
	void SetPos(const VectorInt& newPos) { _pos = newPos; }

private:
	LayerType _layerType;
	Texture* _texture = nullptr;
	VectorInt _pos = { 0, 0 };

};


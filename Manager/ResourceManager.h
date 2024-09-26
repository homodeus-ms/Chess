#pragma once

class Texture;
class Sprite;

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);

public:
	~ResourceManager();

	void Init(HWND hwnd, const string& resourcePath);

	Texture* LoadTexture(const string& nameKey, const string& path, uint32 transparent = RGB(255, 255, 0));
	Texture* GetTexture(const string& nameKey);
	Sprite* CreateSprite(const string& nameKey, Texture* texture, uint32 x = 0, uint32 y = 0, uint32 endX = 0, uint32 endY = 0);
	Sprite* GetSprite(const string& nameKey);
	void LoadSounds();
	void PlaySounds(const string& name, bool ascyn = true);

private:

	HWND _hwnd = {};
	string _resourcePath;

	unordered_map<string, Texture*> _textures;
	unordered_map<string, Sprite*> _sprites;
	unordered_map<string, pair<HGLOBAL, LPVOID>> _sounds;
};


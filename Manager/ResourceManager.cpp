#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"

ResourceManager::~ResourceManager()
{
    for (auto& a : _textures)
        SAFE_DELETE(a.second);
    for (auto& a : _sprites)
        SAFE_DELETE(a.second);
    for (auto& a : _sounds)
        GlobalFree(a.second.first);

    _textures.clear();
    _sprites.clear();
    _sounds.clear();
}

void ResourceManager::Init(HWND hwnd, const string& resourcePath)
{
    _hwnd = hwnd;
    _resourcePath = resourcePath;
    LoadSounds();
}

Texture* ResourceManager::LoadTexture(const string& nameKey, const string& path, uint32 transparent)
{
    if (_textures.find(nameKey) != _textures.end())
        return _textures[nameKey];

    Texture* texture = new Texture();
    const string fullPath = _resourcePath + path;

    texture->LoadFromFile(_hwnd, fullPath);
    texture->SetTransparent(transparent);
    _textures[nameKey] = texture;

    return texture;
}

Texture* ResourceManager::GetTexture(const string& nameKey)
{
    if (_textures.find(nameKey) == _textures.end())
        ASSERT_CRASH(false);

    return _textures[nameKey];
}

Sprite* ResourceManager::CreateSprite(const string& nameKey, Texture* texture, uint32 x, uint32 y, uint32 endX, uint32 endY)
{
    if (_sprites.find(nameKey) != _sprites.end())
        return _sprites[nameKey];

    Sprite* sprite = new Sprite();


    return nullptr;
}

Sprite* ResourceManager::GetSprite(const string& nameKey)
{
    if (_sprites.find(nameKey) == _sprites.end())
        ASSERT_CRASH(false);

    return _sprites[nameKey];
}

void ResourceManager::PlaySounds(const string& name, bool async)
{
    auto it = _sounds.find(name);
    if (it != _sounds.end()) {
        if (async)
            PlaySound(reinterpret_cast<LPCWSTR>(it->second.second), NULL, SND_MEMORY | SND_ASYNC);
        else
            PlaySound(reinterpret_cast<LPCWSTR>(it->second.second), NULL, SND_MEMORY | SND_SYNC);
    }
    else {
        ASSERT_CRASH(false);
    }
}

void ResourceManager::LoadSounds()
{
    // Move Sound1
    {
        const string filePath = _resourcePath + "Sounds\\PieceMove.wav";

        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
            ASSERT_CRASH(false);

        // 파일 크기를 계산하고 메모리에 할당
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileSize);
        if (hGlobal == NULL)
            ASSERT_CRASH(false);

        LPVOID pData = GlobalLock(hGlobal);
        if (pData == NULL)
            ASSERT_CRASH(false);

        // 파일 내용을 메모리에 복사
        file.read(reinterpret_cast<char*>(pData), fileSize);
        file.close();

        GlobalUnlock(hGlobal);

        _sounds["Move1"] = std::make_pair(hGlobal, pData);
    }

    // Move Sound2
    {
        const string filePath = _resourcePath + "Sounds\\PieceMove2.wav";

        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
            ASSERT_CRASH(false);

        // 파일 크기를 계산하고 메모리에 할당
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileSize);
        if (hGlobal == NULL)
            ASSERT_CRASH(false);

        LPVOID pData = GlobalLock(hGlobal);
        if (pData == NULL)
            ASSERT_CRASH(false);

        // 파일 내용을 메모리에 복사
        file.read(reinterpret_cast<char*>(pData), fileSize);
        file.close();

        GlobalUnlock(hGlobal);

        _sounds["Move2"] = std::make_pair(hGlobal, pData);
    }

    // Capture Sound
    {
        const string filePath = _resourcePath + "Sounds\\PieceCapture.wav";

        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
            ASSERT_CRASH(false);

        // 파일 크기를 계산하고 메모리에 할당
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileSize);
        if (hGlobal == NULL)
            ASSERT_CRASH(false);

        LPVOID pData = GlobalLock(hGlobal);
        if (pData == NULL)
            ASSERT_CRASH(false);

        // 파일 내용을 메모리에 복사
        file.read(reinterpret_cast<char*>(pData), fileSize);
        file.close();

        GlobalUnlock(hGlobal);

        _sounds["Capture"] = std::make_pair(hGlobal, pData);
    }
    // CheckMate Sound
    {
        const string filePath = _resourcePath + "Sounds\\Check.wav";

        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
            ASSERT_CRASH(false);

        // 파일 크기를 계산하고 메모리에 할당
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileSize);
        if (hGlobal == NULL)
            ASSERT_CRASH(false);

        LPVOID pData = GlobalLock(hGlobal);
        if (pData == NULL)
            ASSERT_CRASH(false);

        // 파일 내용을 메모리에 복사
        file.read(reinterpret_cast<char*>(pData), fileSize);
        file.close();

        GlobalUnlock(hGlobal);

        _sounds["CheckMate"] = std::make_pair(hGlobal, pData);
    }
    // Win Sound
    {
        const string filePath = _resourcePath + "Sounds\\Win.wav";

        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
            ASSERT_CRASH(false);

        // 파일 크기를 계산하고 메모리에 할당
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileSize);
        if (hGlobal == NULL)
            ASSERT_CRASH(false);

        LPVOID pData = GlobalLock(hGlobal);
        if (pData == NULL)
            ASSERT_CRASH(false);

        // 파일 내용을 메모리에 복사
        file.read(reinterpret_cast<char*>(pData), fileSize);
        file.close();

        GlobalUnlock(hGlobal);

        _sounds["Win"] = std::make_pair(hGlobal, pData);
    }
    // Lose Sound
    {
        const string filePath = _resourcePath + "Sounds\\Lose.wav";

        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
            ASSERT_CRASH(false);

        // 파일 크기를 계산하고 메모리에 할당
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileSize);
        if (hGlobal == NULL)
            ASSERT_CRASH(false);

        LPVOID pData = GlobalLock(hGlobal);
        if (pData == NULL)
            ASSERT_CRASH(false);

        // 파일 내용을 메모리에 복사
        file.read(reinterpret_cast<char*>(pData), fileSize);
        file.close();

        GlobalUnlock(hGlobal);

        _sounds["Lose"] = std::make_pair(hGlobal, pData);
    }

    // Promotion Sound
    {
        const string filePath = _resourcePath + "Sounds\\Promotion.wav";

        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
            ASSERT_CRASH(false);

        // 파일 크기를 계산하고 메모리에 할당
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileSize);
        if (hGlobal == NULL)
            ASSERT_CRASH(false);

        LPVOID pData = GlobalLock(hGlobal);
        if (pData == NULL)
            ASSERT_CRASH(false);

        // 파일 내용을 메모리에 복사
        file.read(reinterpret_cast<char*>(pData), fileSize);
        file.close();

        GlobalUnlock(hGlobal);

        _sounds["Promotion"] = std::make_pair(hGlobal, pData);
    }
}



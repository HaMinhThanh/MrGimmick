#include "Scene.h"

CScene::CScene(int id, LPCWSTR filePath)
{
    this->id = id;
    this->sceneFilePath = filePath;
}

D3DCOLOR CScene::GetBackColor()
{
    return backColor;
}

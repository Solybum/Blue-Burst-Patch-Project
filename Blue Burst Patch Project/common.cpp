#include <stdexcept>
#include <locale>
#include <codecvt>
#include "common.h"

Episode GetCurrentEpisode()
{
    return *reinterpret_cast<Episode*>(0x00a9b1c8);
}

bool IsSoloMode()
{
    return *reinterpret_cast<bool*>(0x00a9b1c4);
}

Map::MapType GetCurrentMap()
{
    return *reinterpret_cast<Map::MapType*>(0x00aafc9c);
}

Difficulty GetCurrentDifficulty()
{
    return *reinterpret_cast<Difficulty*>(0x00a9b1cc);
}

decltype(AddMinimapIcon) AddMinimapIcon = reinterpret_cast<decltype(AddMinimapIcon)>(0x00805608);

namespace Graphics
{
    decltype(EnableTexture) EnableTexture = reinterpret_cast<decltype(EnableTexture)>(0x00829cc8);
    decltype(Render3D) Render3D = reinterpret_cast<decltype(Render3D)>(0x007aa668);
};

namespace Transform
{
    const D3DMATRIX* identityMatrix = reinterpret_cast<D3DMATRIX*>(0x00a21740);

    decltype(PushTransformStackCopy) PushTransformStackCopy = reinterpret_cast<decltype(PushTransformStackCopy)>(0x0082d7b0);
    decltype(PushTransformStack) PushTransformStack = reinterpret_cast<decltype(PushTransformStack)>(0x0082d7b0);
    decltype(PopTransformStack) PopTransformStack = reinterpret_cast<decltype(PopTransformStack)>(0x0082da80);
    decltype(RotateMatrix) RotateMatrix = reinterpret_cast<decltype(RotateMatrix)>(0x0082d8bc);
    decltype(ScaleMatrix) ScaleMatrix = reinterpret_cast<decltype(ScaleMatrix)>(0x0082fa0c);
    decltype(TranslateTransformStackHead) TranslateTransformStackHead = reinterpret_cast<decltype(TranslateTransformStackHead)>(0x0082d838);
};

decltype(InitEnemyCollisionBoxes) InitEnemyCollisionBoxes = reinterpret_cast<decltype(InitEnemyCollisionBoxes)>(0x007b9a88);
decltype(InitEnemyAnimations) InitEnemyAnimations = reinterpret_cast<decltype(InitEnemyAnimations)>(0x007aa4c8);
decltype(AnimateEntity) AnimateEntity = reinterpret_cast<decltype(AnimateEntity)>(0x007aa094);
decltype(GetPlayer) GetPlayer = reinterpret_cast<decltype(GetPlayer)>(0x0068d5a8);
decltype(GetGround) GetGround = reinterpret_cast<decltype(GetGround)>(0x00780630);
decltype(CollideWithEntities) CollideWithEntities = reinterpret_cast<decltype(CollideWithEntities)>(0x007b6880);
decltype(EnableReticle) EnableReticle = reinterpret_cast<decltype(EnableReticle)>(0x006946a8);
decltype(FreeCollisionBoxes) FreeCollisionBoxes = reinterpret_cast<decltype(FreeCollisionBoxes)>(0x007b9c8c);
decltype(GetRandomFloat) GetRandomFloat = reinterpret_cast<decltype(GetRandomFloat)>(0x007751b0);
decltype(CheckAnimationDuration) CheckAnimationDuration = reinterpret_cast<decltype(CheckAnimationDuration)>(0x007aaa30);
decltype(PlaySoundEffect) PlaySoundEffect = reinterpret_cast<decltype(PlaySoundEffect)>(0x00814298);

IDirect3DDevice8** d3dDevice = reinterpret_cast<IDirect3DDevice8**>(0x00acd528);

std::string dirname(const std::string& path)
{
    auto pos = path.find_last_of(PATH_SEPARATOR);
    return std::string::npos == pos ? "" : path.substr(0, pos);
}

std::string joinPath(const std::initializer_list<std::string>& parts)
{
    std::string result = "";
    for (auto const& part : parts)
    {
        if (!part.empty())
        {
            if (!result.empty()) result += PATH_SEPARATOR;

            result += part;
        }
    }

    return result;
}

std::wstring ToWideString(const std::string& narrow)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(narrow);
}

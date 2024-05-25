#include "SceneActors/Markers/MenuMapMarker.h"

const TMap<FString,FString> AMenuMapMarker::GetMenuNavigation() const
{
    return MenuNavigationMap;
}

const FString& AMenuMapMarker::GetLevelPath() const
{
    return LevelPath;
}
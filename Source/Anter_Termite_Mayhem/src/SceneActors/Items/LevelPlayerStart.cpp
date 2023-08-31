#include "SceneActors/Items/LevelPlayerStart.h"
#include "Components/CapsuleComponent.h"

ALevelPlayerStart::ALevelPlayerStart(const FObjectInitializer& ObjectInitializer)
:
Super(ObjectInitializer)
{
//    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(FindComponentByClass<UCapsuleComponent>());
//   if(Capsule != nullptr)
   // {
        RootComponent->SetMobility(EComponentMobility::Type::Movable);
//    }
}
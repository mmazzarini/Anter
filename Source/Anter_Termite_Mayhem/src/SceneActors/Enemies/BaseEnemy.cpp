#include "SceneActors/Enemies/BaseEnemy.h"
#include "ActorComponents/BaseEnemyMovementComponent.h"

ABaseEnemy::ABaseEnemy()
{
    BaseEnemyMovement = CreateDefaultSubobject<UBaseEnemyMovementComponent>(TEXT("EnemyBaseMovement"));
    BaseEnemyMovement->SetupAttachment(RootComponent);
}

void ABaseEnemy::BeginPlay()
{
    if(BaseEnemyMovement != nullptr)
    {
        BaseEnemyMovement->Initialize();
    }

    SetBindings();
}

void ABaseEnemy::SetBindings()
{
    if(BaseEnemyMovement != nullptr)
    {
        BaseEnemyMovement->OnMovementUpdated.AddDynamic(this,&ABaseEnemy::HandleMovement);
    }
}

void ABaseEnemy::HandleMovement(FVector2D InMovementDirection, float InMovementSpeed)
{
    AddMovementInput(InMovementDirection,InMovementSpeed);
}
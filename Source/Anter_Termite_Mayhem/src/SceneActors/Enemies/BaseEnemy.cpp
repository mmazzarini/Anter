#include "SceneActors/Enemies/BaseEnemy.h"
#include "ActorComponents/BaseEnemyMovementComponent.h"

ABaseEnemy::ABaseEnemy()
{
    BaseEnemyMovement = CreateDefaultSubobject<UBaseEnemyMovementComponent>(TEXT("BaseEnemyMovement"));
    BaseEnemyMovement->SetupAttachment(RootComponent);
    
    BaseEnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseEnemyMesh"));
    BaseEnemyMesh->SetupAttachment(RootComponent);

    BaseEnemyHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("BaseEnemyHealth"));
    BaseEnemyHealth->SetupAttachment(RootComponent);

    BaseEnemyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BaseEnemyBox"));
    BaseEnemyBox->SetupAttachment(RootComponent);
}

void ABaseEnemy::BeginPlay()
{
    Super::BeginPlay();
    if(BaseEnemyMovement != nullptr)
    {
        BaseEnemyMovement->Initialize();
    }

    SetBindings();
}

void ABaseEnemy::SetBindings()
{
    if(BaseEnemyBox != nullptr && BaseEnemyMovement != nullptr)
    {
        BaseEnemyBox->OnComponentBeginOverlap.AddDynamic(BaseEnemyMovement,&UBaseEnemyMovementComponent::OnCollided);
    }
}

/*
void ABaseEnemy::HandleMovement(FVector2D InMovementDirection, float InMovementSpeed)
{
    FVector InputMovement3D = FVector(InMovementDirection.X,InMovementDirection.Y,0.0f);
    AddMovementInput(InputMovement3D,InMovementSpeed);
}
*/
#pragma once

#include "GameFramework/Actor.h"
#include "PaperCharacter.h"
#include "ActorComponents/AnterFSMComponent.h"
#include "ActorComponents/HealthComponent.h"
#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ActorComponents/CollisionSupportComponent.h"
#include "SceneActors/SceneActorInterface.h"

#include "BaseEnemy.generated.h"
/*
Base Enemy class
An Enemy that is able to move right/left or up/down according to what is 
set by user on Blueprint defaults.
Can be derived to obtain all the specified enemies.
*/

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API ABaseEnemy : public APaperCharacter, public ISceneActorInterface
{

    GENERATED_BODY()

public:

    ABaseEnemy();

    void BeginPlay() override;

    virtual void HandleCollision(const FCollisionGeometry& CollisionGeometry, AActor* OtherActor) override; //float InMovementSpeed){} 

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UBaseEnemyMovementComponent* BaseEnemyMovement;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UStaticMeshComponent* BaseEnemyMesh;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UHealthComponent* BaseEnemyHealth;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UBoxComponent* BaseEnemyBox;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UCollisionSupportComponent* BaseEnemyCollisionSupport;

protected:

    /* Enemy components */

    void SetBindings();

};
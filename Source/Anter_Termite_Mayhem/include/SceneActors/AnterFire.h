#pragma once

#include "GameFramework/Actor.h"
//#include "PaperSpriteActor.h"
#include "PaperCharacter.h"

#include "PlayerControllers/AnterPlayerController.h"

#include "AnterFire.generated.h"


UCLASS(BlueprintType,Blueprintable)
class AAnterFire : public APaperCharacter
{
    GENERATED_BODY()

    public:

    AAnterFire();

    void Tick(float DeltaSeconds) override;

    virtual void BeginPlay() override;

    virtual void UpdatePosition();

    void CheckScreenLocation();

    protected:

    UPROPERTY(EditDefaultsOnly)
    FVector MovementVector;

    UPROPERTY(EditDefaultsOnly, Category="Laser Movement")
    float MovementX = 10.0f;

    UPROPERTY(EditDefaultsOnly, Category="Laser Movement")
    float MovementY = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category="Laser Movement")
    float MovementZ = 0.0f;

    AAnterPlayerController* PlayerController; 

};

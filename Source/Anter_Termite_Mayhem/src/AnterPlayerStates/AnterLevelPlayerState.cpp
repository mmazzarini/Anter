#include "AnterPlayerStates/AnterLevelPlayerState.h"
#include "Pawn/AnterPaperCharacter.h"
#include "ActorComponents/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SceneActors/Items/LevelGoal.h"

void AAnterLevelPlayerState::BindToPawnDelegates()
{
    if(GetPawn() != nullptr)
    {
        if(AAnterPaperCharacter* AnterPawn = Cast<AAnterPaperCharacter>(GetPawn()))
        {
            UHealthComponent* AnterHealth = Cast<UHealthComponent>(AnterPawn->FindComponentByClass(UHealthComponent::StaticClass()));
            if(AnterHealth != nullptr)
            {
                AnterHealth->OnHealthUpdated.AddDynamic(this,&AAnterLevelPlayerState::OnHealthUpdated);
                AnterHealth->OnDeathReached.AddDynamic(this,&AAnterLevelPlayerState::OnDeathReached);
            }
            //Do bindings to pawn delegates
        }
    }

    TArray<AActor*> LevelGoals;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelGoal::StaticClass(), LevelGoals);
    if(LevelGoals.Num() > 0)
    {
        ALevelGoal* LevelGoal = Cast<ALevelGoal>(LevelGoals[0]);
        if(LevelGoal != nullptr)
        {
            LevelGoal->LevelGoalReached.AddDynamic(this,&AAnterLevelPlayerState::OnLevelGoalReached);
        }

    }

}

void AAnterLevelPlayerState::OnDeathReached()
{
    // code to implement
    OnPlayerStateDeathDelegate.Broadcast();
}

void AAnterLevelPlayerState::OnHealthUpdated(float InNewHealth)
{
    // code to implement
}

void AAnterLevelPlayerState::OnLevelGoalReached()
{
    OnPlayerLevelGoalReachedDelegate.Broadcast();
}
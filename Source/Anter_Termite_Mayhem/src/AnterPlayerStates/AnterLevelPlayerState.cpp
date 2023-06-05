#include "AnterPlayerStates/AnterLevelPlayerState.h"
#include "Pawn/AnterPaperCharacter.h"
#include "ActorComponents/HealthComponent.h"

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
}

void AAnterLevelPlayerState::OnDeathReached()
{
    // code to implement
}

void AAnterLevelPlayerState::OnHealthUpdated(float InNewHealth)
{
    // code to implement
}


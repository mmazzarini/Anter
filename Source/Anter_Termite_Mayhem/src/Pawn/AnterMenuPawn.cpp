#include "Pawn/AnterMenuPawn.h"
#include "Components/InputComponent.h"

AAnterMenuPawn::AAnterMenuPawn()
:
Super()
{

}

void AAnterMenuPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AAnterMenuPawn::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
    Super::SetupPlayerInputComponent(InInputComponent);
    if(InInputComponent != nullptr)
    {
        InInputComponent->BindAction("MapDown",IE_Pressed,this,&ThisClass::HandleMoveDown);
        InInputComponent->BindAction("MapUp",IE_Pressed,this,&ThisClass::HandleMoveUp);
        InInputComponent->BindAction("MapLeft",IE_Pressed,this,&ThisClass::HandleMoveLeft);
        InInputComponent->BindAction("MapRight",IE_Pressed,this,&ThisClass::HandleMoveRight);
    }
}

void AAnterMenuPawn::MoveToLevelPivot()
{

}

void AAnterMenuPawn::GetTargetPivot()
{

}

void AAnterMenuPawn::HandleMoveDown()
{

}

void AAnterMenuPawn::HandleMoveUp()
{
    
}

void AAnterMenuPawn::HandleMoveLeft()
{
    
}

void AAnterMenuPawn::HandleMoveRight()
{
    
}
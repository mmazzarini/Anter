#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "SceneActors/Enemies/BaseEnemy.h"

UBaseEnemyMovementComponent::UBaseEnemyMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;  
    RegisterComponent();
}

void UBaseEnemyMovementComponent::Initialize()
{
    SetMovement(InitialMovementGeometry, InitialMovementSpeed);
    OwnerEnemy = Cast<ABaseEnemy>(GetOwner());
}

void UBaseEnemyMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    UpdateMovement();
}

void UBaseEnemyMovementComponent::SetMovement(FVector2D InGeometryVector, float InSpeed)
{
    InternalMovementGeometry.X = InGeometryVector.X;
    InternalMovementGeometry.Y = InGeometryVector.Y;
    InternalMovementSpeed = InSpeed;
}

void UBaseEnemyMovementComponent::UpdateMovement()
{
    if(OwnerEnemy != nullptr)
    {
        FVector InputMovement3D = FVector(InternalMovementGeometry.X,0.0f,InternalMovementGeometry.Y);
        OwnerEnemy->AddMovementInput(InputMovement3D,InternalMovementSpeed);
    }
    //    OnMovementUpdated.Broadcast(InternalMovementGeometry,InternalMovementSpeed);
}

void UBaseEnemyMovementComponent::OnCollided(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    FString OtherName = OtherActor->GetName();
    if(OtherName.Contains("Floor"))
    {

        FVector PlatformCentre = FVector(0.0f,0.0f,0.0f);
        FVector PlatformSize = FVector(0.0f,0.0f,0.0f);
        OtherActor->GetActorBounds(true,PlatformCentre,PlatformSize,false);
        
        UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(OtherActor->FindComponentByClass<UStaticMeshComponent>());
        float PlatformLength = 0.0f;
        float PlatformHeight = 0.0f;

        if(MeshComponent != nullptr)
        {
            UStaticMesh* StaticMesh = MeshComponent->GetStaticMesh();
            if(StaticMesh != nullptr)
            {
                FVector IntrinsicSize = StaticMesh->GetBounds().GetBox().GetExtent();
                FVector PlatformScale = OtherActor->GetTransform().GetScale3D();
                PlatformLength = IntrinsicSize.X*PlatformScale.X;   
                PlatformHeight =  IntrinsicSize.Z*PlatformScale.Z;    
            }
        }

        FVector PlatformSurfaceCentre = PlatformCentre + OtherActor->GetActorRotation().RotateVector(FVector::UpVector*PlatformHeight); 
        FVector PlatformBottomCentre =  PlatformCentre - OtherActor->GetActorRotation().RotateVector(FVector::UpVector*PlatformHeight);
        float PlatformAngle = OtherActor->GetTransform().GetRotation().GetAngle();
        float PlatformCosine = 1.0f;
        float PlatformSine = 0.0f;
        FMath::SinCos(&PlatformSine,&PlatformCosine,PlatformAngle);
        //Pawn geometrical references
        FVector EnemySize = FVector(0.0f,0.0f,0.0f);
        FVector EnemyCentre = FVector(0.0f,0.0f,0.0f);
        this->GetActorBounds(true,AnterCentre,Size,false);
        FVector TopDist = this->GetActorLocation()-PlatformSurfaceCentre;
        FVector BottomDist = this->GetActorLocation()-PlatformBottomCentre;
        //Check geometry of collision to decide which impact     
        FVector RotatedNormal = OtherActor->GetTransform().GetRotation().RotateVector(FVector::UpVector);
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Platform angle: ") +  FString::SanitizeFloat(PlatformAngle));
        if(FVector::DotProduct(TopDist,RotatedNormal) >= VerticalTolerance)
        {
            /* Impact was from top: pawn is standing on platform. */ 

        } 
        else
        {
            /* Impact was from side or bottom */
            InvertSpeed();
        }
    }
}

void UBaseEnemyMovementComponent::InvertSpeed()
{
    InternalMovementSpeed = -InternalMovementSpeed;
}
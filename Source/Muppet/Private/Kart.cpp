#include "Kart.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AKart::AKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetSimulatePhysics(true);
	Box->SetEnableGravity(true);
	Box->SetLinearDamping(3.0f);
	Box->SetAngularDamping(5.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	KartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KartMesh"));
	KartMesh->SetupAttachment(RootComponent);

	Wheel_FL = CreateDefaultSubobject<USceneComponent>(TEXT("Wheel_FL"));
	Wheel_FL->SetupAttachment(RootComponent);

	Wheel_FR = CreateDefaultSubobject<USceneComponent>(TEXT("Wheel_FR"));
	Wheel_FR->SetupAttachment(RootComponent);

	Wheel_BL = CreateDefaultSubobject<USceneComponent>(TEXT("Wheel_BL"));
	Wheel_BL->SetupAttachment(RootComponent);

	Wheel_BR = CreateDefaultSubobject<USceneComponent>(TEXT("Wheel_BR"));
	Wheel_BR->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKart::BeginPlay()
{
	Super::BeginPlay();

	Wheels.Add(Wheel_FL);
	Wheels.Add(Wheel_FR);
	Wheels.Add(Wheel_BL);
	Wheels.Add(Wheel_BR);
}

// Called every frame
void AKart::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (const TObjectPtr<USceneComponent> Wheel : Wheels)
	{
		FVector WheelLocation = Wheel->GetComponentLocation();
		FVector UpLocationWheel = Wheel->GetUpVector();
		SuspensionCast(WheelLocation, UpLocationWheel);
	}
	SetAccelerationAndMass();
	bIsGrounded = IsGrounded();
	ApplySteeringStabilization();
}

void AKart::SuspensionCast(const FVector& WheelLocation, const FVector& UpLocationWheel) const
{
	FHitResult Hit;
	const FVector EndPoint = WheelLocation - (UpLocationWheel * SuspensionHeight);
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, WheelLocation, EndPoint, ECC_Visibility);
	FColor Color = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), WheelLocation, EndPoint, Color, false, -1, 0, 1);

	if (bHit)
	{
		const float Force = 1 - UKismetMathLibrary::NormalizeToRange(Hit.Distance, 0, SuspensionHeight);
		const FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(Hit.TraceEnd, Hit.TraceStart);
		const FVector ForceDirection = Force * Direction * ScaleForceSuspension;
		Box->AddForceAtLocation(ForceDirection, WheelLocation);
	}
}


void AKart::SetAccelerationAndMass()
{
	float TargetSpeed = AccelerationInput * (AccelerationInput > 0 ? MaxSpeedForward : MaxSpeedBackward);
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, GetWorld()->GetDeltaSeconds() * Acceleration,
	                                InterpolationSpeed);
	FVector Direction = CurrentSpeed * Box->GetForwardVector();
	FVector Velocity = Box->GetPhysicsLinearVelocity();
	Direction.Z = Velocity.Z;
	Box->SetPhysicsLinearVelocity(Direction);
	Box->SetCenterOfMass(Mass * AccelerationInput);
}

void AKart::ApplySteeringStabilization()
{
	if (!IsGrounded())
		return;

	SteeringInput = FMath::FInterpTo(SteeringInput, 0.0f, GetWorld()->GetDeltaSeconds(), 5.0f);
	// Appliquer la rotation en fonction de l'input
	float Direction = SteeringInput * DirectionForce;
	Box->AddTorqueInRadians(FVector(0, 0, Direction));

	FVector Velocity = Box->GetPhysicsLinearVelocity();
	FVector RightVector = GetActorRightVector();

	// Récupérer la vitesse latérale (côté)
	float LateralSpeed = FVector::DotProduct(Velocity, RightVector);

	// Appliquer une force opposée pour réduire le drift
	FVector LateralForce = -RightVector * LateralSpeed * DriftReductionForce;
	Box->AddForce(LateralForce);
}

void AKart::Accelerate(const float Value)
{
	AccelerationInput = Value;
}

void AKart::Steering(float Value)
{
	if (IsGrounded())
	{
		SteeringInput = Value;
	}
	else
	{
		SteeringInput = 0;
	}
}

void AKart::Jump(bool Value)
{
	JumpInput = Value;
	if (JumpInput && bIsGrounded)
	{
		Box->AddImpulse(FVector(0, 0, ImpulseJump), "None", true);
	}
}

bool AKart::IsGrounded()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 50.0f); // 50 unités sous le kart

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(10.0f); // Rayon de la sphère
	FHitResult Hit;

	bool bHit = GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Visibility, Sphere, QueryParams);

	// 🎨 Debug : Vert = sol, Bleu = rebond sur kart, Rouge = en l'air
	FColor Color = FColor::Red;

	if (bHit)
	{
		Color = FColor::Green; // Sol normal

		// Vérifier si c'est un autre kart
		if (AKart* OtherKart = Cast<AKart>(Hit.GetActor()))
		{
			Color = FColor::Blue; // Indiquer un rebond sur un kart
			BounceOnKart(Hit);
		}
	}

	DrawDebugSphere(GetWorld(), End, Sphere.GetSphereRadius(), 12, Color, false, 0.1f, 0, 2.0f);
	return bHit;
}

void AKart::BounceOnKart(const FHitResult& Hit) const
{
	FVector BounceForce = FVector(0, 0, BounceStrength); // Force vers le haut
	Box->AddImpulse(BounceForce, NAME_None, true); // Appliquer une impulsion

	UE_LOG(LogTemp, Warning, TEXT("Bounced on another kart!"));
}

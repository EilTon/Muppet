#include "KartController.h"

void AKartController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	ControllerKart = Cast<AKart>(GetPawn());
}

void AKartController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(IA_Accelerate, ETriggerEvent::Triggered, this, &AKartController::AccelerateAction);
		EnhancedInputComponent->BindAction(IA_Steering, ETriggerEvent::Triggered, this, &AKartController::SteeringAction);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AKartController::JumpAction);
	}
}

void AKartController::AccelerateAction(const FInputActionValue& Value)
{
	if (ControllerKart)
	{
		ControllerKart->Accelerate(Value.Get<float>());
	}
}

void AKartController::SteeringAction(const FInputActionValue& Value)
{
	if (ControllerKart)
	{
		ControllerKart->Steering(Value.Get<float>());
	}
}

void AKartController::JumpAction(const FInputActionValue& Value)
{
	if (ControllerKart)
	{
		ControllerKart->Jump(Value.Get<bool>());
	}
}

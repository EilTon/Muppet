// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kart.h"
#include "KartController.generated.h"

class UInputMappingContext;
class UInputAction;
class AKart;

UCLASS()
class MUPPET_API AKartController : public APlayerController
{
	GENERATED_BODY()
	

	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	

private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly ,Category = "Enhanced Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly ,Category = "Enhanced Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_Accelerate;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly ,Category = "Enhanced Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_Steering;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly ,Category = "Enhanced Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly ,Category = "Enhanced Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> IA_FireItem1;

	TObjectPtr<AKart> ControllerKart;

	void AccelerateAction(const FInputActionValue& Value);
	void SteeringAction(const FInputActionValue& Value);
	void JumpAction(const FInputActionValue& Value);
	void FireItem1Action(const FInputActionValue& Value);
};

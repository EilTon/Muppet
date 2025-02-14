// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Pawn.h"
#include "Kart.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
class UTimelineComponent;
class UCurveFloat;

UCLASS()
class MUPPET_API AKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKart();

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UCurveFloat> BounceCurve;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> BounceTimeline; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Kart", meta=(AllowPrivateAccess=true))
	TObjectPtr<UStaticMeshComponent> KartMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Collision", meta=(AllowPrivateAccess=true))
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera Settings", meta=(AllowPrivateAccess=true))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera Settings", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Wheel", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> Wheel_FL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Wheel", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> Wheel_FR;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Wheel", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> Wheel_BL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Wheel", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> Wheel_BR;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Suspension", meta=(AllowPrivateAccess=true))
	float ScaleForceSuspension = 90000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Suspension", meta=(AllowPrivateAccess=true))
	float SuspensionHeight = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess=true))
	FVector Mass = FVector(50, 0, 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess=true))
	float ScaleAcceleration = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess=true))
	float MaxSpeedForward = 15000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess=true))
	float MaxSpeedBackward = 15000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess=true))
	float Acceleration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Acceleration", meta=(AllowPrivateAccess=true))
	float InterpolationSpeed = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Direction", meta=(AllowPrivateAccess=true))
	float DirectionForce = 10000000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Jump", meta=(AllowPrivateAccess=true))
	float ImpulseJump = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Steering", meta=(AllowPrivateAccess=true))
	float DriftReductionForce = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Bounce")
	float BounceStrength = 500.0f;

	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SuspensionCast(const FVector&, const FVector&) const;
	void SetAccelerationAndMass();
	void ApplySteeringStabilization();
	void PlayHitKart();

	UFUNCTION()
	void UpdateHitKart(float Value);

	UFUNCTION()
	void FinishHitKart();

	TArray<TObjectPtr<USceneComponent>> Wheels;
	float AccelerationInput;
	float SteeringInput;
	float CurrentSpeed;
	float InitialYaw;
	bool JumpInput;
	bool bIsGrounded;
	FVector StartLocation;
	FRotator StartRotation;
	FVector CameraSavedLocation;
	FRotator CameraSavedRotation;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Accelerate(float Value);
	void Steering(float Value);
	void Jump(bool Value);
	void FireFirstItem(bool Value);
	bool IsGrounded();
	void BounceOnKart(const FHitResult&) const;
	

	
};

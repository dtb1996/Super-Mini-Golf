// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "Sound/SoundCue.h"
#include "GolfPawn.generated.h"

UCLASS(Abstract)
class SUPERMINIGOLF_API AGolfPawn : public APawn
{
	GENERATED_BODY()

public:
	AGolfPawn();

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	
	UPROPERTY(BlueprintReadWrite)
	AActor* SkySphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollTorque = 5000000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TiltAngularDamping = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalAngularDamping = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAngularVelocity = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TiltFactorX = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TiltFactorY = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitImpulseMin = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TiltAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnMyComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	void HandleTiltInputTriggered(const FInputActionValue& Value);

	void HandleTiltInputStarted(const FInputActionValue& Value);

	void HandleTiltInputCompleted(const FInputActionValue& Value);

	void HandleLookInputTriggered(const FInputActionValue& Value);

	void UpdateSpringArmRotation();

	void UpdateSpringArmTargetLength();

	void UpdateSkyRotation();

	FVector2D TiltValue;

};

// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "GolfPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

AGolfPawn::AGolfPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(SphereCollision);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SphereCollision);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AGolfPawn::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentHit.AddDynamic(this, &AGolfPawn::OnMyComponentHit);
	
	SphereCollision->SetPhysicsMaxAngularVelocityInRadians(MaxAngularVelocity);
}

void AGolfPawn::Destroyed()
{
	SphereCollision->OnComponentHit.RemoveDynamic(this, &AGolfPawn::OnMyComponentHit);
}

void AGolfPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateIsOnGround();

	UpdateSpringArmRotation();

	UpdateSpringArmTargetLength();

	UpdateSkyRotation();

	ApplyRedirectForce();
}

void AGolfPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(TiltAction, ETriggerEvent::Triggered, this, &AGolfPawn::HandleTiltInputTriggered);
		EnhancedInputComponent->BindAction(TiltAction, ETriggerEvent::Started, this, &AGolfPawn::HandleTiltInputStarted);
		EnhancedInputComponent->BindAction(TiltAction, ETriggerEvent::Completed, this, &AGolfPawn::HandleTiltInputCompleted);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGolfPawn::HandleLookInputTriggered);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Started, this, &AGolfPawn::HandleLookInputStarted);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &AGolfPawn::HandleLookInputCompleted);
	}
}

void AGolfPawn::OnMyComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (NormalImpulse.Size() < HitImpulseMin)
	{
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactNiagaraEffect, Hit.ImpactPoint, FRotator(0.0f, 0.0f, 0.0f), SphereCollision->GetComponentScale(), true, true, ENCPoolMethod::None, false);

	UGameplayStatics::PlaySound2D(GetWorld(), ImpactSound, 1.0f, 1.0f, 0.0f, ImpactConcurrencySettings, nullptr, false);
}

void AGolfPawn::HandleTiltInputTriggered(const FInputActionValue& Value)
{
	TiltValue = Value.Get<FVector2D>();

	FVector ControlRightVector = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
	FVector ForwardTorque = FVector(ControlRightVector.X, ControlRightVector.Y, 0.0f) * RollTorque;

	FVector ControlForwardVector = GetControlRotation().Vector();
	FVector RightTorque = FVector(ControlForwardVector.X, ControlForwardVector.Y, 0.0f) * RollTorque;

	FVector Torque = (ForwardTorque * TiltValue.Y) + (RightTorque * -TiltValue.X);

	SphereCollision->AddTorqueInRadians(Torque, NAME_None, true);

	if (!bIsLookInputActive && bIsTiltTurnEnabled)
	{
		AddControllerYawInput(TiltValue.X * TiltYawInputFactorX);
	}

	FRotator Rotation(0.0f, 90.0f, 0.0f); // Rotate 90 degrees around Z-axis (Yaw)
	DesiredVector = -(Rotation.RotateVector(Torque).GetSafeNormal(0.0001f));
}

void AGolfPawn::HandleTiltInputStarted(const FInputActionValue& Value)
{
	SphereCollision->SetAngularDamping(TiltAngularDamping);
}

void AGolfPawn::HandleTiltInputCompleted(const FInputActionValue& Value)
{
	TiltValue = FVector2D();
	SphereCollision->SetAngularDamping(NormalAngularDamping);

	DesiredVector = FVector(0.0f, 0.0f, 0.0f);
}

void AGolfPawn::HandleLookInputTriggered(const FInputActionValue& Value)
{
	FVector2D LookValue = Value.Get<FVector2D>();

	AddControllerYawInput(LookValue.X);
}

void AGolfPawn::HandleLookInputStarted(const FInputActionValue& Value)
{
	bIsLookInputActive = true;
}

void AGolfPawn::HandleLookInputCompleted(const FInputActionValue& Value)
{
	bIsLookInputActive = false;
}

void AGolfPawn::UpdateIsOnGround()
{
	FHitResult Hit = FHitResult();

	FVector Start = GetActorLocation();
	FVector End = FVector(Start.X, Start.Y, Start.Z - 55.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, QueryParams);

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 2.0f, 0, 2.0f);
	}

	bIsOnGround = bHit;
}

void AGolfPawn::UpdateSpringArmRotation()
{
	if (!SpringArm)
	{
		return;
	}
	
	FRotator CurrentRotation = SpringArm->GetRelativeRotation();

	float TargetX = TiltValue.X * -TiltFactorX;
	float TargetY = TiltValue.Y * TiltFactorY;
	
	FRotator TargetRotation = FRotator(TargetY, CurrentRotation.Yaw, TargetX);

	float Alpha = 5.0f;

	FRotator InterpolatedRotation = FMath::Lerp(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds() * Alpha);

	SpringArm->SetRelativeRotation(InterpolatedRotation);
}

void AGolfPawn::UpdateSpringArmTargetLength()
{
	if (!SphereCollision)
	{
		return;
	}

	float Velocity = SphereCollision->GetComponentVelocity().Size();

	float NewTargetLength = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1500.0f), FVector2D(500.0f, 800.0f), Velocity);

	float Alpha = 5.0f;

	SpringArm->TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, NewTargetLength, GetWorld()->GetDeltaSeconds() * Alpha);
}

void AGolfPawn::UpdateSkyRotation()
{
	if (!SkySphere)
	{
		return;
	}

	SkySphere->SetActorRelativeRotation(FRotator(0.0f, -GetControlRotation().Yaw, 0.0f));
}

void AGolfPawn::ApplyRedirectForce()
{
	if (!SphereCollision)
	{
		return;
	}

	FVector ActualVector = SphereCollision->GetComponentVelocity().GetSafeNormal(0.0001f);

	// Compute dot product and angle in degrees
	float DotProduct = FVector::DotProduct(DesiredVector, ActualVector);
	float AngleInDegrees = FMath::Acos(DotProduct) * (180.0f / UE_DOUBLE_PI);

	float ForceScale = (AngleInDegrees / 180.0f) * RedirectForce;

	// Select float based on whether the actor is on the ground
	float GroundMultiplier = bIsOnGround ? 1.0f : 0.5f;
	
	FVector ForceToApply = DesiredVector * (ForceScale * GroundMultiplier);

	SphereCollision->AddForce(ForceToApply);
}

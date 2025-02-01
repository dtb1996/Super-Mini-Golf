// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "GolfPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

AGolfPawn::AGolfPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(SphereCollision);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(BallMesh);
	Arrow->SetUsingAbsoluteRotation(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SphereCollision);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AGolfPawn::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->SetPhysicsMaxAngularVelocityInRadians(MaxAngularVelocity);
}

void AGolfPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSpringArmRotation();

	UpdateSkyRotation();

	UpdateArrow();
}

void AGolfPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(TiltAction, ETriggerEvent::Triggered, this, &AGolfPawn::HandleTiltInputTriggered);
	EnhancedInputComponent->BindAction(TiltAction, ETriggerEvent::Started, this, &AGolfPawn::HandleTiltInputStarted);
	EnhancedInputComponent->BindAction(TiltAction, ETriggerEvent::Completed, this, &AGolfPawn::HandleTiltInputCompleted);

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &AGolfPawn::HandleLookInputTriggered);
}

void AGolfPawn::HandleTiltInputTriggered(const FInputActionValue& Value)
{
	TiltValue = Value.Get<FVector2D>();

	FVector ControlRightVector = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
	ControlRightVector = FVector(ControlRightVector.X, ControlRightVector.Y, 0.0f) * RollTorque;

	FVector ControlForwardVector = GetControlRotation().Vector();
	ControlForwardVector = FVector(ControlForwardVector.X, ControlForwardVector.Y, 0.0f) * RollTorque;

	FVector Torque = (ControlRightVector * TiltValue.Y) + (ControlForwardVector * -TiltValue.Y);

	SphereCollision->AddTorqueInRadians(Torque, NAME_None, true);
}

void AGolfPawn::HandleTiltInputStarted(const FInputActionValue& Value)
{
	SphereCollision->SetAngularDamping(TiltAngularDamping);
}

void AGolfPawn::HandleTiltInputCompleted(const FInputActionValue& Value)
{
	SphereCollision->SetAngularDamping(NormalAngularDamping);
}

void AGolfPawn::HandleLookInputTriggered(const FInputActionValue& Value)
{
	FVector2D LookValue = Value.Get<FVector2D>();

	AddControllerYawInput(LookValue.X);
}

void AGolfPawn::UpdateSpringArmRotation()
{
	FRotator CurrentRotation = SpringArm->GetRelativeRotation();

	float TargetX = TiltValue.X * -TiltFactorX;
	float TargetY = TiltValue.Y * TiltFactorX;

	FRotator TargetRotation = FRotator(TargetX, TargetY, CurrentRotation.Yaw);

	float Alpha = 5.0f;

	FRotator InterpolatedRotation = FMath::Lerp(CurrentRotation, TargetRotation, Alpha);

	SpringArm->SetRelativeRotation(InterpolatedRotation);
}

void AGolfPawn::UpdateSkyRotation()
{
}

void AGolfPawn::UpdateArrow()
{
	FVector SphereVelocity = SphereCollision->GetComponentVelocity();

	FVector ForwardVector = (SphereVelocity - Arrow->GetRelativeLocation()).GetSafeNormal();

	FRotator NewRotation = UKismetMathLibrary::MakeRotationFromAxes(ForwardVector, FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f));

	Arrow->SetWorldRotation(NewRotation);

	Arrow->ArrowLength = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1500.0f), FVector2D(150.0f, 300.0f), SphereVelocity.Size());
}


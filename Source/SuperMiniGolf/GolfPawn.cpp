// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "GolfPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

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

	UpdateSpringArmRotation();

	UpdateSpringArmTargetLength();

	UpdateSkyRotation();
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
	}
}

void AGolfPawn::OnMyComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (NormalImpulse.Size() < HitImpulseMin)
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), ImpactSound);
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
}

void AGolfPawn::HandleTiltInputStarted(const FInputActionValue& Value)
{
	SphereCollision->SetAngularDamping(TiltAngularDamping);
}

void AGolfPawn::HandleTiltInputCompleted(const FInputActionValue& Value)
{
	TiltValue = FVector2D();
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
	float TargetY = TiltValue.Y * TiltFactorY;
	
	FRotator TargetRotation = FRotator(TargetY, CurrentRotation.Yaw, TargetX);

	float Alpha = 5.0f;

	FRotator InterpolatedRotation = FMath::Lerp(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds() * Alpha);

	SpringArm->SetRelativeRotation(InterpolatedRotation);
}

void AGolfPawn::UpdateSpringArmTargetLength()
{
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

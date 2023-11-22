// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"
#include "JCharacter.h"

// Sets default values
AJCharacter::AJCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������͵��ɱ�������������ɫ��
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

//ʱ���ắ����
void AJCharacter::CWTimelineTickCallBack(float value)
{
	SpringArmComp->SocketOffset.X = 0.0f;
	SpringArmComp->SocketOffset.Y = SpringArmComp->SocketOffset.Y;
	SpringArmComp->SocketOffset.Z = FMath::Lerp(0.0f, -40.0f,value);
}

void AJCharacter::CWTimelineFinishedCallBack()
{
}

//Delay������
void AJCharacter::ResetbIsJump()
{
	bIsJump = false;
	bInJump = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

// Called when the game starts or when spawned
void AJCharacter::BeginPlay()
{
	Super::BeginPlay();

	CWTimeline = NewObject<UTimelineComponent>(this,"CrouchTimeLine");
	//����ʱ����
	OnCWTimelineTickCallBack.BindDynamic(this, &AJCharacter::CWTimelineTickCallBack);
	//OnCWTimelineTickCallBack.BindUFunction(this, "CWTimelineTickCallBack");
	CWTimeline->AddInterpFloat(CWFloatCurve, OnCWTimelineTickCallBack);

	//����
	OnCWTimelineFinishedCallBack.BindUFunction(this, "CWTimelineFinishedCallBack");
	CWTimeline->SetTimelineFinishedFunc(OnCWTimelineFinishedCallBack);

	//CWTimeline->SetTimelineLength(1.0f);
	//CWTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

	//CWTimeline->SetLooping(false);
	CWTimeline->RegisterComponent();
}

//����ǰ���ƶ�
void AJCharacter::MoveForward(float value)
{
	FRotator ControlRota = GetControlRotation();
	ControlRota.Pitch = 0.0f;
	ControlRota.Roll = 0.0f;

	//��Ӧ��ͼ�е�make Rotator + GetForwardVector
	FVector ForwardVector = FRotationMatrix(ControlRota).GetScaledAxis(EAxis::X);

	AddMovementInput(ForwardVector, value);
}

void AJCharacter::MoveRight(float value)
{
	FRotator ControlRota = GetControlRotation();
	ControlRota.Pitch = 0.0f;
	ControlRota.Roll = 0.0f;

	//��Ӧ��ͼ�е�make Rotator + GetRightVector
	FVector RightVector = FRotationMatrix(ControlRota).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}

void AJCharacter::WalkControl()
{
	GetCharacterMovement()->MaxWalkSpeed = 120.0f;
	GetCharacterMovement()->JumpZVelocity = 200.0f;
	
}

void AJCharacter::RunControl()
{
	GetCharacterMovement()->MaxWalkSpeed = 660.0f;
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	

}

void AJCharacter::JogControl()
{
	GetCharacterMovement()->MaxWalkSpeed = 420.0f;
	GetCharacterMovement()->JumpZVelocity = 300.0f;
	
}

void AJCharacter::Jump()
{
	Super::Jump();
	bIsJump = true;
	float Velocity = GetCharacterMovement()->MaxWalkSpeed;
	if (!bInJump) {
		bInJump = true;
		if (Velocity == 120.0f) {
			PlayAnimMontage(WalkJump, 1.f);
		}
		else if (Velocity == 420.0f) {
			PlayAnimMontage(JogJump, 1.f);
		}
		else if (Velocity == 660.0f) {
			PlayAnimMontage(RunJump);
		}
	}
	//��ʱִ��
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AJCharacter::ResetbIsJump, 1.0f, true);
}

void AJCharacter::JCrouch()
{
	if (!GetCharacterMovement()->IsCrouching()) {
		Crouch();
		bIsCrouch = true;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 135.0f;
		CWTimeline->Play();
	}
	else {
		UnCrouch();
		bIsCrouch = false;
		GetCharacterMovement()->MaxWalkSpeed = 420.0f;
		CWTimeline->Reverse();
	}
	
}

// Called every frame
void AJCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CWTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);

}

// Called to bind functionality to input
void AJCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//����Ϊ
	PlayerInputComponent->BindAction("WalkControl", IE_Pressed, this, &AJCharacter::WalkControl);
	PlayerInputComponent->BindAction("RunControl", IE_Pressed, this, &AJCharacter::RunControl);
	PlayerInputComponent->BindAction("WalkControl", IE_Released, this, &AJCharacter::JogControl);
	PlayerInputComponent->BindAction("RunControl", IE_Released, this, &AJCharacter::JogControl);
	PlayerInputComponent->BindAction("JumpControl", IE_Pressed, this, &AJCharacter::Jump);
	PlayerInputComponent->BindAction("CrouchControl", IE_Pressed, this, &AJCharacter::JCrouch);

	//�󶨼���ӳ��
	//Yaw��ӦZ�ᣬPitch��ӦY�ᣬRoll��ӦX��
	PlayerInputComponent->BindAxis("MoveForward", this, &AJCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

}



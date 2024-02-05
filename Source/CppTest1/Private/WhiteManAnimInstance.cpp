// Fill out your copyright notice in the Description page of Project Settings.


#include "WhiteManAnimInstance.h"
#include "JCharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UWhiteManAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	JCharacter_Ins = Cast<AJCharacter>(TryGetPawnOwner());
}

void UWhiteManAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (JCharacter_Ins == nullptr)
	{
		return;
	}
	GetVariable();
	SetMoveVector();
	SetState(DeltaTime);

}

void UWhiteManAnimInstance::GetVariable()
{
	ForwardInput = JCharacter_Ins->ForwardInput;
	RightInput = JCharacter_Ins->RightInput;
	bIsCrouch = JCharacter_Ins->bIsCrouch;
	bIsRun = JCharacter_Ins->bInRun;
	bCanJump = JCharacter_Ins->bCanJump;
	bHaveWeapon = JCharacter_Ins->bSetWeapon;
	bIsFalling = JCharacter_Ins->GetCharacterMovement()->IsFalling();
	Speed = JCharacter_Ins->GetVelocity().Size();
}

void UWhiteManAnimInstance::SetState(float DeltaTime)
{
	if (bIsRun)
	{
		if (bIsCrouch)
		{
			DesireState = 1.0f;
		}
		DesireState = 3.0f;
	}
	else if (!bIsRun && bIsCrouch)
	{
		DesireState = 1.0f;
	}
	else 
	{
		DesireState = 2.0f;
	}

	CurrentMoveState = FMath::Lerp(CurrentMoveState, DesireState, DeltaTime * 10.f);

	//设置是否能够移动
	if (InputVelocityVec.Size() > 0.0f)
	{
		bCanMove = true;
	}
	else
	{
		bCanMove = false;
	}
}

void UWhiteManAnimInstance::SetMoveVector()
{
	Yaw = JCharacter_Ins->GetActorRotation().Yaw;
	Pitch = JCharacter_Ins->GetActorRotation().Pitch;
	//caculate the movevector , Yaw 
	//use Forwardvector&ForwardInput + RightVector&RightInput
	YawRotator = JCharacter_Ins->GetControlRotation();
	YawRotator.Roll = 0.0f;
	YawRotator.Pitch = 0.0f;
	ForwardVec = UKismetMathLibrary::GetForwardVector(YawRotator);
	RightVec = UKismetMathLibrary::GetRightVector(YawRotator);
	//UKismetMathLibrary::Add_VectorVector(UKismetMathLibrary::Multiply_VectorFloat(ForwardVec, ForwardInput), UKismetMathLibrary::Multiply_VectorFloat(RightVec, RightInput));
	InputVelocityVec = ForwardVec * ForwardInput + RightVec * RightInput;
	MoveLoopDirection = CalculateDirection(InputVelocityVec, JCharacter_Ins->GetActorRotation());

	//平滑切换视角转向
	if (Speed >= 150.f)
	{
		FRotator ClampRot = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, UKismetMathLibrary::FWrap(MoveLoopDirection, -1.0f, 1.0f));
		JCharacter_Ins->AddActorLocalRotation(ClampRot);
	}
}

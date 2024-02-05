// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JCharacter.h"
#include "WhiteManAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPPTEST1_API UWhiteManAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	//������ͼ��ʼ��
	virtual void NativeInitializeAnimation() override;

	//event tick: log in main Func 
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	friend class AJCharacter;

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		 AJCharacter* JCharacter_Ins;

	//��������Ҫ�õ��Ĳ���
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float ForwardInput;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float RightInput;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		bool bIsCrouch;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		bool bIsRun;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		bool bCanJump;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		bool bCanMove;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		bool bIsFalling;
	UPROPERTY(BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		bool bHaveWeapon;
	//���ƶף��ߣ����л�
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float DesireState;
	//�����Ƿ�ֹͣʱ��Idle
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float StopFlag;
	//�����л�
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float CurrentMoveState;
	//�ƶ���������
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		FVector InputVelocityVec;

	//Yaw �� Pitch
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float Yaw;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float Pitch;
	//�ƶ�ʱ�ķ���-180~180
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float MoveLoopDirection;
	//�ƶ��ٶ�
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float Speed;

	//��ͼ�п��޸ĵı���
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		float UpFoot;
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		float JumpState;
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		bool bLand;
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		float MoveStartDirection;

	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		FRotator YawRotator;
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		FVector ForwardVec;
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		FVector RightVec;


public:
	//��ȡ��ɫ�еĲ���
	UFUNCTION()
		void GetVariable();

	//�����ƶ�״̬
	UFUNCTION()
		void SetState(float DeltaTime);
	void SetMoveVector();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Components/TimelineComponent.h"
#include "JCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class UTimelineComponen;

UCLASS()
class CPPTEST1_API AJCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJCharacter();

	//�ж��Ƿ����¶�
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsCrouch = false;

	//����һ��ʱ���ᣬʵ���¶׺�����������ӽ��л�
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UTimelineComponent*  CWTimeline;

	//����һ����������
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		 UCurveFloat* CWFloatCurve;

	//�������߸����¼�
	FOnTimelineFloat OnCWTimelineTickCallBack;
	//�������߲�������¼�
	FOnTimelineEvent OnCWTimelineFinishedCallBack;

	UFUNCTION()
		void CWTimelineTickCallBack(float value);
	UFUNCTION()
		void CWTimelineFinishedCallBack();

	//ʵ��Delay
	FTimerHandle TimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsJump = false;
	//����Jump����ʱ�����ģ�Ȼ����ʱ����ʱ��֮���޸Ļ���
	void ResetbIsJump();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//���+���ɱ����
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComp;

	//������̫��
	UPROPERTY(EditAnywhere, Category = "Jump")
		UAnimMontage* WalkJump;
	UPROPERTY(EditAnywhere, Category = "Jump")
		UAnimMontage* JogJump;
	UPROPERTY(EditAnywhere, Category = "Jump")
		UAnimMontage* RunJump;

	//����ǰ���ƶ�����
	void MoveForward(float value);
	void MoveRight(float value);
	
	//�����ٶ��л�
	void WalkControl();
	void RunControl();
	void JogControl();

	//��дJump,��ʵ�ֲ�����̫��
	virtual void Jump() override;
	//�������֮����ܼ�����Ծ
	bool bInJump = false;

	//ʵ�ֶ׷�����
	void JCrouch();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

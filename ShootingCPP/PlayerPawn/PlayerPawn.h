// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class SHOOTINGCPP_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// 이 폰의 속성에 대한 기본 값을 설정(Sets default values for this pawn's properties)
	APlayerPawn();

protected:
	// 게임이 시작될 때 또는 스폰될 때 호출됨(Called when the game starts or when spawned)
	virtual void BeginPlay() override;

public:
	// 매 프레임마다 호출됨(Called every frame)
	virtual void Tick(float DeltaTime) override;

	// 기능을 입력에 연결하기 위해 호출됨(Called to bind functionality to input)
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 박스 충돌체 컴포넌트
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	// 스태틱 메시 컴포넌트
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	// 속력 변수
	UPROPERTY(EditAnywhere)
	float moveSpeed = 500;

	// 총구 위치
	UPROPERTY(EditAnywhere)
	class UArrowComponent* firePosition;

	// 총알 블루프린트
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;

	// 총알 발사 효과음 파일
	UPROPERTY(EditAnywhere)
	class USoundBase* fireSound;
	
private:
	// 사용자의 키 입력 값을 받을 변수
	float h;
	float v;

	// 사용자 입력 처리 함수
	void MoveHorizontal(float vlaue);
	void MoveVertical(float value);

	// 총알 발사 입력 처리 함수
	void Fire();
};

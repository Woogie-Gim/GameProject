// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "PlayerPawn.h"
#include "ShootingGameModeBase.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
	meshComp->SetupAttachment(boxComp);

	// Collision presets을 Enemy 프리셋으로 변경한다.
	boxComp->SetCollisionProfileName(TEXT("Enemy"));

}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 1~100 사이의 임의의 정수 값을 추첨한다.
	int32 drawResult = FMath::RandRange(1, 100);

	// 만일, 추첨된 값이 추적 확률 변수보다 작거나 같다면
	if (drawResult <= traceRate)
	{
		// 월드 공간에 APlayerPawn 클래스로 된 액터를 모두 검색한다.
		for (TActorIterator<APlayerPawn> player(GetWorld()); player; ++player)
		{
			// 만일 검색된 액터의 이름에 'BP_PlayerPawn'이란 문구가 포함되어 있다면
			if (player->GetName().Contains(TEXT("BP_PlayerPawn")))
			{
				// 플레이어 액터의 위치 - 자신의 위치
				dir = player->GetActorLocation() - GetActorLocation();
				dir.Normalize();
			}
		}
	}
	// 그렇지 않다면 정면 방향 벡터를 생성한다.
	else
	{
		dir = GetActorForwardVector();
	}

	// 박스 컴포넌트의 충돌 오버랩 이벤트에 BulletOverlap 함수를 연결한다.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnEnemyOverlap);
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// BeginPlay()에서 결정된 방향으로 이동한다.
	FVector newLocation = GetActorLocation() + dir * moveSpeed * DeltaTime;
	SetActorLocation(newLocation);

}

void AEnemyActor::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌할 액터를 APlayerPawn 클래스로 변환을 시도
	APlayerPawn* player = Cast<APlayerPawn>(OtherActor);

	// 만일, 캐스팅이 정상적으로 되어서 AEnemy Actor 포인터 변수에 값이 있다면
	if (player != nullptr)
	{
		// 충돌할 액터를 제거한다.
		OtherActor->Destroy();

		// 현재 게임 모드를 가져온다.
		AShootingGameModeBase* currentGameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());

		if (currentGameMode != nullptr)
		{
			// 메뉴 UI 생성 함수를 호출한다.
			currentGameMode->ShowMenu();
		}
	}

	// 자기 자신을 제거한다.
	Destroy();
}

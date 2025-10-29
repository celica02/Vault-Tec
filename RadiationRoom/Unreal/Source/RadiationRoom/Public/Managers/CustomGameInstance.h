// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CustomGameInstance.generated.h"

/**
 * 
 */

//UENUM(BlueprintType)
//enum class EGameMode : uint8
//{
//	// examples for game modes (i.e. rule sets) of your game
//	EveryManForHimself UMETA(DisplayName = "every man for himself"),
//	Teams UMETA(DisplayName = "teams"),
//	Coop UMETA(DisplayName = "coop"),
//};
//
//USTRUCT(BlueprintType)
//struct FHostSessionConfig
//{
//	GENERATED_BODY()
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	FString CustomName;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	int32 NumMaxPlayers;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	bool bPrivate;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	bool bEnableLAN;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	EGameMode GameMode;
//};


UCLASS()
class RADIATIONROOM_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()


protected:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;

	virtual void Init()override;

	virtual void OnCreateSessionComplete(FName SessionName, bool bSucceeded);
	virtual void OnFindSessionComplete(bool bSucceeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
	void CreateServer();
	UFUNCTION(BlueprintCallable)
	void JoinServer();

	//UFUNCTION(BlueprintCallable)
	//void HostGame(int32 LocalPlayerIndex);
	//UFUNCTION(BlueprintCallable)
	//void JoinGame(int32 LocalPlayerIndex);
	//
	//UFUNCTION(NetMulticast, Reliable)
	//void LeaveGame();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//FHostSessionConfig SessionConfig = // default values
	//{ ""
	//, 4
	//, false
	//, true
	//, EGameMode::EveryManForHimself
	//};
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/CustomGameInstance.h"
#include "Managers/SessionManager.h"
#include "Managers/CustomLocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
//void UCustomGameInstance::HostGame(int32 LocalPlayerIndex)
//{
//	USessionManager* GISub = GetSubsystem<USessionManager>();
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hosting game 1"));
//	}
//	GISub->CreateSession(LocalPlayerIndex, SessionConfig, [this, LocalPlayerIndex](FName SessionName, bool bSuccess)
//		{
//			if (bSuccess)
//			{
//				Cast<UCustomLocalPlayer>(GetLocalPlayerByIndex(LocalPlayerIndex))->CurrentLevel = ECurrentLevel::StartLevel;
//				GetWorld()->ServerTravel("/Game/Experiment/Maps/ExperimentSetUpLevel");
//				if (GEngine)
//				{
//					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hosting game success"));
//				}
//			}
//			else
//			{
//				UE_LOG(LogNet, Error, TEXT("%s: create session call returned with failure"), *GetFullName())
//			}
//		});
//}
//
//void UCustomGameInstance::JoinGame(int32 LocalPlayerIndex)
//{
//	Cast<UCustomLocalPlayer>(GetLocalPlayerByIndex(LocalPlayerIndex))->IsMultiplayer = true;
//	GetSubsystem<USessionManager>()->JoinSession(LocalPlayerIndex, [this, LocalPlayerIndex](ECurrentLevel NewLevel, EOnJoinSessionCompleteResult::Type Result)
//		{
//			switch (Result)
//			{
//				using namespace EOnJoinSessionCompleteResult;
//			case SessionIsFull:
//				UE_LOG(LogNet, Error, TEXT("%s: Join session: session is full"), *GetFullName())
//					break;
//			case SessionDoesNotExist:
//				UE_LOG(LogNet, Error, TEXT("%s: Join session: session does not exist"), *GetFullName())
//					break;
//			case CouldNotRetrieveAddress:
//				UE_LOG(LogNet, Error, TEXT("%s: Join session: could not retrieve address"), *GetFullName())
//					break;
//			case AlreadyInSession:
//				UE_LOG(LogNet, Error, TEXT("%s: Join session: alreayd in session"), *GetFullName())
//					break;
//			case UnknownError:
//				UE_LOG(LogNet, Error, TEXT("%s: Join session: unknown error"), *GetFullName())
//					break;
//			case Success:
//				if (ClientTravelToSession(LocalPlayerIndex, NAME_GameSession))
//				{
//
//					Cast<UCustomLocalPlayer>(GetLocalPlayerByIndex(LocalPlayerIndex))->CurrentLevel = NewLevel;
//				}
//				else
//				{
//					UE_LOG(LogNet, Error, TEXT("%s: travel to session failed"), *GetFullName())
//				}
//				break;
//			default:;
//			}
//		});
//}
//
//void UCustomGameInstance::LeaveGame_Implementation()
//{
//	GetSubsystem<USessionManager>()->LeaveSession();
//}


void UCustomGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCustomGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCustomGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCustomGameInstance::OnJoinSessionComplete);
		}
	}
}

void UCustomGameInstance::OnCreateSessionComplete(FName SessionName, bool bSucceeded)
{
	if (bSucceeded)
	{
		GetWorld()->ServerTravel("/Game/Experiment/Maps/ExperimentSetUpLevel?listen");
	}
}

void UCustomGameInstance::OnFindSessionComplete(bool bSucceeded)
{
	if (bSucceeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		if (SearchResults.Num())
		{
			SessionInterface->JoinSession(0, FName("Custom Session"), SearchResults[0]);
		}
	}
}

void UCustomGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PController->ClientTravel(JoinAddress, TRAVEL_Absolute);
		}
	}
}

void UCustomGameInstance::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL") ? false : true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;

	SessionInterface->CreateSession(0, FName("Custom Session"), SessionSettings);
}

void UCustomGameInstance::JoinServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 1000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

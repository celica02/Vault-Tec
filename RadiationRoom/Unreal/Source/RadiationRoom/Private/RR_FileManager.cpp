// Fill out your copyright notice in the Description page of Project Settings.


#include "RR_FileManager.h"

#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"


bool URR_FileManager::DirectoryExists(FString Path, FString DirectoryName)
{
	FString fullPath = Path + "/" + DirectoryName;
	return FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*fullPath);
}

bool URR_FileManager::FileExists(FString Path, FString Filename)
{
	FString fullPath = Path + "/" + Filename;
	return FPlatformFileManager::Get().GetPlatformFile().FileExists(*fullPath);
}

void URR_FileManager::CreateDirectory(FString Path, FString DirectoryName)
{
	FString fullPath = Path + "/" + DirectoryName;
	if (!DirectoryExists(Path, DirectoryName))
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*fullPath);
}

FString URR_FileManager::ReadFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage) {
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath)) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read File Failed - File doesn't exist - '%s'"), *FilePath);
		return "";
	}

	FString RetString = "";
	if (!FFileHelper::LoadFileToString(RetString, *FilePath)) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read File Failed - Was not able to read file. Is this a text file? - '%s'"), *FilePath);
		return "";
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read File Succeded - '%s'"), *FilePath);
	return RetString;
}

void URR_FileManager::WriteFile(FString FilePath, FString String, bool& bOutSuccess, FString& OutInfoMessage) {
	if (!FFileHelper::SaveStringToFile(String, *FilePath)) {
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write File Failed - Was not able to write to file. Is your file read only? Is the path valid? - '%s'"), *FilePath);
		return;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write File Succeded - '%s'"), *FilePath);
}

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define MYLOG(format, ...) \
{ \
	UE_LOG(LogTemp, Warning, TEXT(format), ##__VA_ARGS__); \
}

#define MYLOG_ERROR(format, ...) \
{ \
	UE_LOG(LogTemp, Error, TEXT(format), ##__VA_ARGS__); \
}

#define MYSCREENLOG(format, ...) \
{ \
   	if (GEngine != nullptr) \
	{ \
		FString msg = FString::Printf(TEXT(format), ##__VA_ARGS__); \
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, msg); \
	} \
}

#define MYSCREENLOG_ERROR(format, ...) \
{ \
   	if (GEngine != nullptr) \
	{ \
		FString msg = FString::Printf(TEXT(format), ##__VA_ARGS__); \
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, msg); \
	} \
}

#define MYGLOG(format, ...) \
{ \
	if (GLog != nullptr) \
	{ \
		FString msg = FString::Printf(TEXT(format), ##__VA_ARGS__); \
		GLog->Log(msg); \
	} \
}

#define MYLOGTRACE(format, ...) \
{ \
    UE_LOG(LogTemp, Warning, TEXT("[%s:%d] " format), \
        TEXT(__FILE__), __LINE__, ##__VA_ARGS__); \
}
// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


// Module includes
#include "AndroidFacebookPluginPrivatePCH.h"
#include "OnlineIdentityFacebook.h"


///////////////////////////////////////////////////////////////////////////////////////
// FUserOnlineAccountFacebook implementation


FUserOnlineAccountFacebook::FUserOnlineAccountFacebook(const FString& InUserId, const FString& InAuthTicket) 
		: AuthTicket(InAuthTicket)
		, UserId(new FUniqueNetIdString(InUserId))
{
	GConfig->GetString(TEXT("OnlineSubsystemFacebook.Login"), TEXT("AuthToken"), AuthTicket, GEngineIni);
}

TSharedRef<const FUniqueNetId> FUserOnlineAccountFacebook::GetUserId() const
{
	return UserId;
}

FString FUserOnlineAccountFacebook::GetRealName() const
{
	return UserName;
}

FString FUserOnlineAccountFacebook::GetDisplayName() const
{
	return UserName;
}

bool FUserOnlineAccountFacebook::GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const
{
	return false;
}

FString FUserOnlineAccountFacebook::GetAccessToken() const
{
	return AuthTicket;
}

bool FUserOnlineAccountFacebook::GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////
// FOnlineIdentityFacebook implementation


FOnlineIdentityFacebook::FOnlineIdentityFacebook()
	: UserAccount( MakeShareable(new FUserOnlineAccountFacebook()) )
	, LoginStatus( ELoginStatus::NotLoggedIn )
{
	
}


TSharedPtr<FUserOnlineAccount> FOnlineIdentityFacebook::GetUserAccount(const FUniqueNetId& UserId) const
{
	return UserAccount;
}


TArray<TSharedPtr<FUserOnlineAccount> > FOnlineIdentityFacebook::GetAllUserAccounts() const
{
	TArray<TSharedPtr<FUserOnlineAccount> > Result;

	Result.Add( UserAccount );

	return Result;
}


TSharedPtr<const FUniqueNetId> FOnlineIdentityFacebook::GetUniquePlayerId(int32 LocalUserNum) const
{
	return UserAccount->GetUserId();
}


bool FOnlineIdentityFacebook::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	bool bTriggeredLogin = true;
	/*
	dispatch_async(dispatch_get_main_queue(),^ 
		{
			// Retrieve the user info when logged in
			void(^CompletionBlock)(bool, const FUniqueNetId&) = ^ (bool bWasSucessful, const FUniqueNetId& UserId)
			{
				[[[FBSDKGraphRequest alloc] initWithGraphPath:@"me" parameters:nil]
				startWithCompletionHandler:^ (FBSDKGraphRequestConnection *connection, id result, NSError *error) {
					if (!error) {
						const FString RealName(result[@"name"]);
						UserAccount->UserName = RealName;
						UE_LOG(LogOnline, Display, TEXT("Got Facebook name: %s"), *RealName);
					}

					[FIOSAsyncTask CreateTaskWithBlock : ^ bool(void)
					{						
						TriggerOnLoginCompleteDelegates(LocalUserNum, bWasSucessful, UserId, TEXT(""));
						return true;
					}];
				}];
			};


			FBSDKAccessToken *accessToken = [FBSDKAccessToken currentAccessToken];
			if (accessToken == nil)
			{
				FBSDKLoginManager* loginManager = [[FBSDKLoginManager alloc] init];
				[loginManager logInWithReadPermissions : @[@"public_profile"]
					fromViewController: nil
					handler: ^(FBSDKLoginManagerLoginResult* result, NSError* error)
					{
						UE_LOG(LogOnline, Display, TEXT("[FBSDKLoginManager logInWithReadPermissions]"));
						bool bSuccessfulLogin = false;
				 
						if(error)
						{
							UE_LOG(LogOnline, Display, TEXT("[FBSDKLoginManager logInWithReadPermissions = error[%d]]"), [error code]);
						}
						else if(result.isCancelled)
						{
							UE_LOG(LogOnline, Display, TEXT("[FBSDKLoginManager logInWithReadPermissions = cancelled"));
						}
						else
						{
							UE_LOG(LogOnline, Display, TEXT("[FBSDKLoginManager logInWithReadPermissions = true]"));
				 
							FString Token([result token].tokenString);
							UserAccount->AuthTicket = Token;
							GConfig->SetString(TEXT("OnlineSubsystemFacebook.Login"), TEXT("AuthToken"), *Token, GEngineIni);
				 
							const FString UserId([result token].userID);
							UserAccount->UserId = MakeShareable(new FUniqueNetIdString(UserId));

							const FString RealName([FBSDKProfile currentProfile].name);
							UserAccount->UserName = RealName;
							UE_LOG(LogOnline, Display, TEXT("Got Facebook name: %s"), *RealName);
				 
							bSuccessfulLogin = true;
						}
				 
						LoginStatus = bSuccessfulLogin ? ELoginStatus::LoggedIn : ELoginStatus::NotLoggedIn;
												
						FUniqueNetIdString TempId;
						CompletionBlock(bSuccessfulLogin, (bSuccessfulLogin ? UserAccount->UserId.Get() : TempId));
				 
						UE_LOG(LogOnline, Display, TEXT("Facebook login was successful? - %d"), bSuccessfulLogin);
					}
				];
			}
			else
			{
				LoginStatus = ELoginStatus::LoggedIn;
				const FString UserId([accessToken userID]);
				UserAccount->UserId = MakeShareable(new FUniqueNetIdString(UserId));

				FString Token([accessToken tokenString]);
				UserAccount->AuthTicket = Token;
				
				CompletionBlock(true, UserAccount->UserId.Get());

				UE_LOG(LogOnline, Display, TEXT("Facebook login was successful? - Already had token!"));
			}
		}
	);
	*/
	
	return bTriggeredLogin;	
}


TSharedPtr<const FUniqueNetId> FOnlineIdentityFacebook::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	if (Bytes != NULL && Size > 0)
	{
		FString StrId(Size, (TCHAR*)Bytes);
		return MakeShareable(new FUniqueNetIdString(StrId));
	}
	return NULL;
}


TSharedPtr<const FUniqueNetId> FOnlineIdentityFacebook::CreateUniquePlayerId(const FString& Str)
{
	return MakeShareable(new FUniqueNetIdString(Str));
}


bool FOnlineIdentityFacebook::Logout(int32 LocalUserNum)
{
	//@todo samz - login status change delegate
	/*
	dispatch_async(dispatch_get_main_queue(),^ 
		{
			if ([FBSDKAccessToken currentAccessToken])
			{
				FBSDKLoginManager *loginManager = [[FBSDKLoginManager alloc] init];
				[loginManager logOut];
				
				LoginStatus = ELoginStatus::NotLoggedIn;
			}
		}
	);
	*/

	return true;
}


bool FOnlineIdentityFacebook::AutoLogin(int32 LocalUserNum)
{
	return false;
}


ELoginStatus::Type FOnlineIdentityFacebook::GetLoginStatus(int32 LocalUserNum) const
{
	return LoginStatus;
}

ELoginStatus::Type FOnlineIdentityFacebook::GetLoginStatus(const FUniqueNetId& UserId) const 
{
	return LoginStatus;
}


FString FOnlineIdentityFacebook::GetPlayerNickname(int32 LocalUserNum) const
{
	/*if ([FBSDKAccessToken currentAccessToken])
	{		
		return[FBSDKProfile currentProfile].name;
	}*/
	return TEXT("");
}

FString FOnlineIdentityFacebook::GetPlayerNickname(const FUniqueNetId& UserId) const 
{
	/*if ([FBSDKAccessToken currentAccessToken])
	{
		return[FBSDKProfile currentProfile].name;
	}*/
	return TEXT("");
}


FString FOnlineIdentityFacebook::GetAuthToken(int32 LocalUserNum) const
{
	return UserAccount->GetAccessToken();
}

void FOnlineIdentityFacebook::GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(UserId, Privilege, (uint32)EPrivilegeResults::NoFailures);
}

FPlatformUserId FOnlineIdentityFacebook::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId)
{
	for (int i = 0; i < MAX_LOCAL_PLAYERS; ++i)
	{
		auto CurrentUniqueId = GetUniquePlayerId(i);
		if (CurrentUniqueId.IsValid() && (*CurrentUniqueId == UniqueNetId))
		{
			return i;
		}
	}

	return PLATFORMUSERID_NONE;
}

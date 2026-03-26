# DLL Project – Operating Systems Task

## Description
This project implements a dynamic-link library (DLL) in C++ and a console application that uses it.

The DLL contains functions for:
- Time measurement
- Opening Windows system components
- Creating and deleting folders
- Creating and managing text files
- Calculating Tschirnhausen cubic points
- Merging data from multiple files

The application loads the DLL dynamically using:
- LoadLibrary
- GetProcAddress

No .h or .lib files are used in the main application.

## Features
- Dynamic DLL usage
- File and directory manipulation
- Mathematical computation
- Windows system interaction
- Setup installer support (.msi / .exe)

## How to run
1. Build the solution in Visual Studio
2. Make sure `MyDll.dll` is in the same folder as `MyApp.exe`
3. Run `MyApp.exe`

OR

Use the provided setup installer to install and run the program.

## Technologies
- C++
- Windows API
- Visual Studio 2022

## Author
Miglė Činčikaitė

// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <chrono>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

static std::chrono::high_resolution_clock::time_point startTime;

static std::vector<std::string> GetWorkingFiles()
{
    return {
        "C:\\Cincikaite\\Migle1\\Migle1Migle1\\duomenys.txt",
        "C:\\Cincikaite\\Migle1\\Migle1Migle2\\duomenys.txt",
        "C:\\Cincikaite\\Migle1\\Migle1Migle3\\duomenys.txt",
        "C:\\Cincikaite\\Migle2\\Migle2Migle1\\duomenys.txt",
        "C:\\Cincikaite\\Migle2\\Migle2Migle2\\duomenys.txt",
        "C:\\Cincikaite\\Migle2\\Migle2Migle3\\duomenys.txt",
        "C:\\Cincikaite\\Migle3\\Migle3Migle1\\duomenys.txt",
        "C:\\Cincikaite\\Migle3\\Migle3Migle2\\duomenys.txt",
        "C:\\Cincikaite\\Migle3\\Migle3Migle3\\duomenys.txt"
    };
}

extern "C" __declspec(dllexport) void StartTimer()
{
    startTime = std::chrono::high_resolution_clock::now();
}

extern "C" __declspec(dllexport) double StopTimer()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    return elapsed.count();
}

extern "C" __declspec(dllexport) int OpenWindowsComponent(const char* component)
{
    if (component == nullptr)
        return -1;

    std::string c = component;

    if (c == "display")
        return system("start ms-settings:display");
    else if (c == "security")
        return system("start windowsdefender:");
    else if (c == "power")
        return system("start ms-settings:powersleep");
    else if (c == "storage")
        return system("start ms-settings:storagesense");
    else if (c == "update")
        return system("start ms-settings:windowsupdate");

    return -2;
}

extern "C" __declspec(dllexport) int CreateFolderTree()
{
    // Isvalome sena medi, jei buvo likes
    system("rmdir /S /Q C:\\Cincikaite >nul 2>nul");

    system("mkdir C:\\Cincikaite >nul 2>nul");
    system("mkdir C:\\Cincikaite\\Migle1 >nul 2>nul");
    system("mkdir C:\\Cincikaite\\Migle2 >nul 2>nul");
    system("mkdir C:\\Cincikaite\\Migle3 >nul 2>nul");

    system("mkdir C:\\Cincikaite\\Migle1\\Migle1Migle1 >nul 2>nul");
    system("mkdir C:\\Cincikaite\\Migle1\\Migle1Migle2 >nul 2>nul");
    system("mkdir C:\\Cincikaite\\Migle1\\Migle1Migle3 >nul 2>nul");

    system("mkdir C:\\Cincikaite\\Migle2\\Migle2Migle1 >nul 2>nul");
    system("mkdir C:\\Cincikaite\\Migle2\\Migle2Migle2 >nul 2>nul");
    system("mkdir C:\\Cincikaite\\Migle2\\Migle2Migle3 >nul 2>nul");

    system("mkdir C:\\Cincikaite\\Migle3\\Migle3Migle1 >nul 2>nul");
    system("mkdir C:\\Cincikaite\\Migle3\\Migle3Migle2 >nul 2>nul");
    system("mkdir C:\\Cincikaite\\Migle3\\Migle3Migle3 >nul 2>nul");

    return 0;
}

extern "C" __declspec(dllexport) int CreateTextFiles()
{
    int result = 0;

    result |= system("type nul > C:\\Cincikaite\\Migle1\\Migle1Migle1\\duomenys.txt");
    result |= system("type nul > C:\\Cincikaite\\Migle1\\Migle1Migle2\\duomenys.txt");
    result |= system("type nul > C:\\Cincikaite\\Migle1\\Migle1Migle3\\duomenys.txt");

    result |= system("type nul > C:\\Cincikaite\\Migle2\\Migle2Migle1\\duomenys.txt");
    result |= system("type nul > C:\\Cincikaite\\Migle2\\Migle2Migle2\\duomenys.txt");
    result |= system("type nul > C:\\Cincikaite\\Migle2\\Migle2Migle3\\duomenys.txt");

    result |= system("type nul > C:\\Cincikaite\\Migle3\\Migle3Migle1\\duomenys.txt");
    result |= system("type nul > C:\\Cincikaite\\Migle3\\Migle3Migle2\\duomenys.txt");
    result |= system("type nul > C:\\Cincikaite\\Migle3\\Migle3Migle3\\duomenys.txt");

    return result;
}

extern "C" __declspec(dllexport) int ClearTextFiles()
{
    std::vector<std::string> files = GetWorkingFiles();

    for (const auto& file : files)
    {
        std::ofstream fout(file, std::ios::trunc);
        if (!fout.is_open())
            return -1;
    }

    return 0;
}

extern "C" __declspec(dllexport) int WriteTschirnhausenPoints(double F, double x0, double xn, double dx)
{
    if (dx <= 0.0 || x0 > xn)
        return -2;

    std::vector<std::string> files = GetWorkingFiles();
    std::vector<std::ofstream> streams;
    streams.reserve(files.size());

    for (const auto& file : files)
    {
        streams.emplace_back(file, std::ios::app);
        if (!streams.back().is_open())
            return -1;
    }

    size_t fileIndex = 0;
    long long steps = static_cast<long long>((xn - x0) / dx);

    for (long long i = 0; i <= steps; ++i)
    {
        double x = x0 + i * dx;
        double expr = x * x * x + 3.0 * x * x - F;

        if (expr < 0.0)
            continue;

        double y = std::sqrt(expr);

        streams[fileIndex] << x << " " << y << '\n';
        if (!streams[fileIndex].good())
            return -3;

        fileIndex = (fileIndex + 1) % streams.size();

        if (y > 0.0)
        {
            streams[fileIndex] << x << " " << -y << '\n';
            if (!streams[fileIndex].good())
                return -4;

            fileIndex = (fileIndex + 1) % streams.size();
        }
    }

    return 0;
}

extern "C" __declspec(dllexport) int MergePointsByF(double F)
{
    std::vector<std::string> files = GetWorkingFiles();
    std::vector<std::pair<double, double>> points;

    for (const auto& file : files)
    {
        std::ifstream fin(file);
        if (!fin.is_open())
            return -1;

        double x, y;
        while (fin >> x >> y)
        {
            points.push_back({ x, y });
        }
    }

    std::sort(points.begin(), points.end(),
        [](const std::pair<double, double>& a, const std::pair<double, double>& b)
        {
            if (a.first < b.first) return true;
            if (a.first > b.first) return false;
            return a.second > b.second;
        });

    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    std::string path = exePath;
    size_t pos = path.find_last_of("\\/");
    std::string exeDir = (pos == std::string::npos) ? "." : path.substr(0, pos);

    std::string outFileName;
    if (F < 0)
        outFileName = exeDir + "\\F_m" + std::to_string((int)(-F)) + ".txt";
    else
        outFileName = exeDir + "\\F_" + std::to_string((int)F) + ".txt";

    std::ofstream fout(outFileName, std::ios::trunc);
    if (!fout.is_open())
        return -2;

    for (const auto& p : points)
    {
        fout << p.first << " " << p.second << '\n';
        if (!fout.good())
            return -3;
    }

    fout.close();

    // Tarpinius failus po suliejimo isvalome
    for (const auto& file : files)
    {
        std::ofstream clear(file, std::ios::trunc);
        if (!clear.is_open())
            return -4;
    }

    return 0;
}

extern "C" __declspec(dllexport) int DeleteFolderTree()
{
    return system("rmdir /S /Q C:\\Cincikaite");
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}



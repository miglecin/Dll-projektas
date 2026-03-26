// MyApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <iostream>
#include <string>

void PrintMenu()
{
    std::cout << "\n================ MENIU ================\n";
    std::cout << "1 - Vykdyti visa programa automatiskai\n";
    std::cout << "2 - Atidaryti Windows komponenta\n";
    std::cout << "3 - Sukurti aplankalu struktura\n";
    std::cout << "4 - Sukurti txt failus\n";
    std::cout << "5 - Isvalyti txt failus\n";
    std::cout << "6 - Skaiciuoti viena F reiksme\n";
    std::cout << "7 - Skaiciuoti visus F nuo -2 iki 2\n";
    std::cout << "8 - Sulieti viena F i viena faila\n";
    std::cout << "9 - Istrinti darbinius aplankalus\n";
    std::cout << "0 - Baigti darba\n";
    std::cout << "=======================================\n";
    std::cout << "Pasirinkimas: ";
}

const char* AskComponent()
{
    int c;
    std::cout << "\nPasirink Windows komponenta:\n";
    std::cout << "1 - display\n";
    std::cout << "2 - security\n";
    std::cout << "3 - power\n";
    std::cout << "4 - storage\n";
    std::cout << "5 - update\n";
    std::cout << "Pasirinkimas: ";
    std::cin >> c;

    switch (c)
    {
    case 1: return "display";
    case 2: return "security";
    case 3: return "power";
    case 4: return "storage";
    case 5: return "update";
    default: return nullptr;
    }
}

int RunFullProgram(
    void (*StartTimer)(),
    double (*StopTimer)(),
    int (*OpenWindowsComponent)(const char*),
    int (*CreateFolderTree)(),
    int (*CreateTextFiles)(),
    int (*ClearTextFiles)(),
    int (*WriteTschirnhausenPoints)(double, double, double, double),
    int (*MergePointsByF)(double),
    int (*DeleteFolderTree)())
{
    int r = 0;

    StartTimer();

    // Pagal mano varianta
    r = OpenWindowsComponent("display");
    if (r != 0)
        std::cout << "Demesio: nepavyko atidaryti Windows komponentes.\n";

    r = CreateFolderTree();
    if (r != 0)
    {
        std::cout << "Klaida kuriant aplankalu struktura.\n";
        return 1;
    }

    r = CreateTextFiles();
    if (r != 0)
    {
        std::cout << "Klaida kuriant txt failus.\n";
        return 1;
    }

    // Tikri parametrai pagal Stud_ID = 2412926
    double x0 = -2.0;
    double xn = 1.0;

    // Demonstracijai galima pakeisti i true
    bool demoMode = false;
    double dx = demoMode ? 0.001 : 0.000002412926;

    long long steps = static_cast<long long>((xn - x0) / dx);

    std::cout << "\nNaudojamas dx = " << dx << "\n";
    std::cout << "Zingsniu vienam F = " << steps << "\n";

    for (int F = -2; F <= 2; ++F)
    {
        std::cout << "\nSkaiciuojama F = " << F << "...\n";

        r = ClearTextFiles();
        if (r != 0)
        {
            std::cout << "Klaida valant tarpinius failus.\n";
            return 1;
        }

        r = WriteTschirnhausenPoints((double)F, x0, xn, dx);
        if (r != 0)
        {
            std::cout << "Klaida rasant taskus, F = " << F << ", kodas = " << r << "\n";
            return 1;
        }

        r = MergePointsByF((double)F);
        if (r != 0)
        {
            std::cout << "Klaida suliejant taskus, F = " << F << ", kodas = " << r << "\n";
            return 1;
        }

        std::cout << "F = " << F << " baigta.\n";
    }

    r = DeleteFolderTree();
    if (r != 0)
        std::cout << "Demesio: nepavyko istrinti darbiniu aplankalu.\n";

    double elapsed = StopTimer();
    std::cout << "\nGalutinis laikas: " << elapsed << " s\n";

    return 0;
}

int main()
{
    HINSTANCE hDll = LoadLibrary(L"MyDll.dll");

    if (!hDll)
    {
        std::cout << "DLL neuzkrauta.\n";
        return 1;
    }

    typedef void (*StartTimerFunc)();
    typedef double (*StopTimerFunc)();
    typedef int (*OpenWindowsComponentFunc)(const char*);
    typedef int (*CreateFolderTreeFunc)();
    typedef int (*CreateTextFilesFunc)();
    typedef int (*ClearTextFilesFunc)();
    typedef int (*WriteTschirnhausenPointsFunc)(double, double, double, double);
    typedef int (*MergePointsByFFunc)(double);
    typedef int (*DeleteFolderTreeFunc)();

    StartTimerFunc StartTimer =
        (StartTimerFunc)GetProcAddress(hDll, "StartTimer");
    StopTimerFunc StopTimer =
        (StopTimerFunc)GetProcAddress(hDll, "StopTimer");
    OpenWindowsComponentFunc OpenWindowsComponent =
        (OpenWindowsComponentFunc)GetProcAddress(hDll, "OpenWindowsComponent");
    CreateFolderTreeFunc CreateFolderTree =
        (CreateFolderTreeFunc)GetProcAddress(hDll, "CreateFolderTree");
    CreateTextFilesFunc CreateTextFiles =
        (CreateTextFilesFunc)GetProcAddress(hDll, "CreateTextFiles");
    ClearTextFilesFunc ClearTextFiles =
        (ClearTextFilesFunc)GetProcAddress(hDll, "ClearTextFiles");
    WriteTschirnhausenPointsFunc WriteTschirnhausenPoints =
        (WriteTschirnhausenPointsFunc)GetProcAddress(hDll, "WriteTschirnhausenPoints");
    MergePointsByFFunc MergePointsByF =
        (MergePointsByFFunc)GetProcAddress(hDll, "MergePointsByF");
    DeleteFolderTreeFunc DeleteFolderTree =
        (DeleteFolderTreeFunc)GetProcAddress(hDll, "DeleteFolderTree");

    if (!StartTimer || !StopTimer || !OpenWindowsComponent || !CreateFolderTree ||
        !CreateTextFiles || !ClearTextFiles || !WriteTschirnhausenPoints ||
        !MergePointsByF || !DeleteFolderTree)
    {
        std::cout << "Nepavyko gauti funkciju is DLL.\n";
        FreeLibrary(hDll);
        return 1;
    }

    int choice = -1;

    do
    {
        PrintMenu();
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Neteisingas pasirinkimas.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            int result = RunFullProgram(
                StartTimer,
                StopTimer,
                OpenWindowsComponent,
                CreateFolderTree,
                CreateTextFiles,
                ClearTextFiles,
                WriteTschirnhausenPoints,
                MergePointsByF,
                DeleteFolderTree);

            if (result == 0)
                std::cout << "\nVisa programa ivykdyta sekmingai.\n";
            break;
        }

        case 2:
        {
            const char* component = AskComponent();
            if (component == nullptr)
            {
                std::cout << "Neteisingas komponentes pasirinkimas.\n";
                break;
            }

            int r = OpenWindowsComponent(component);
            std::cout << "OpenWindowsComponent rezultatas: " << r << "\n";
            break;
        }

        case 3:
        {
            int r = CreateFolderTree();
            std::cout << "CreateFolderTree rezultatas: " << r << "\n";
            break;
        }

        case 4:
        {
            int r = CreateTextFiles();
            std::cout << "CreateTextFiles rezultatas: " << r << "\n";
            break;
        }

        case 5:
        {
            int r = ClearTextFiles();
            std::cout << "ClearTextFiles rezultatas: " << r << "\n";
            break;
        }

        case 6:
        {
            double F;
            std::cout << "Ivesk F reiksme: ";
            std::cin >> F;

            double x0 = -2.0;
            double xn = 1.0;
            bool demoMode = true;
            double dx = demoMode ? 0.001 : 0.000002412926;

            int r = WriteTschirnhausenPoints(F, x0, xn, dx);
            std::cout << "WriteTschirnhausenPoints rezultatas: " << r << "\n";
            break;
        }

        case 7:
        {
            double x0 = -2.0;
            double xn = 1.0;
            bool demoMode = true;
            double dx = demoMode ? 0.001 : 0.000002412926;

            for (int F = -2; F <= 2; ++F)
            {
                std::cout << "Skaiciuojama F = " << F << "...\n";

                int r1 = ClearTextFiles();
                if (r1 != 0)
                {
                    std::cout << "Klaida valant failus, F = " << F << "\n";
                    break;
                }

                int r2 = WriteTschirnhausenPoints((double)F, x0, xn, dx);
                if (r2 != 0)
                {
                    std::cout << "Klaida rasant taskus, F = " << F << ", kodas = " << r2 << "\n";
                    break;
                }

                int r3 = MergePointsByF((double)F);
                if (r3 != 0)
                {
                    std::cout << "Klaida suliejant taskus, F = " << F << ", kodas = " << r3 << "\n";
                    break;
                }

                std::cout << "F = " << F << " baigta.\n";
            }
            break;
        }

        case 8:
        {
            double F;
            std::cout << "Ivesk F reiksme suliejimui: ";
            std::cin >> F;

            int r = MergePointsByF(F);
            std::cout << "MergePointsByF rezultatas: " << r << "\n";
            break;
        }

        case 9:
        {
            int r = DeleteFolderTree();
            std::cout << "DeleteFolderTree rezultatas: " << r << "\n";
            break;
        }

        case 0:
            std::cout << "Programa baigiama.\n";
            break;

        default:
            std::cout << "Tokio pasirinkimo nera.\n";
            break;
        }

    } while (choice != 0);

    FreeLibrary(hDll);
    return 0;
}

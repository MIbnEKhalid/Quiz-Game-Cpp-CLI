#include <iostream>
#include <windows.h>
#include "QuizManager.h"
using namespace std;

int main()
{
    QuizManager quizManager;
    const char *folderName = "UserHistoryData";

    // Use CreateDirectory to create the folder
    if (CreateDirectory(folderName, NULL))
    {
        //    std::cout << "Folder created successfully: " << folderName << std::endl;
    }
    else
    {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS)
        {
            //    std::cout << "Folder already exists: " << folderName << std::endl;
        }
        else
        {
            std::cout << "Failed to create folder. Error code: " << error << std::endl;
        }
    }

    quizManager.SetColor(GREEN);
    quizManager.MainMenu();

    return 0;
}

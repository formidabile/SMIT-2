#pragma once

#define _WIN32_DCOM

#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <comdef.h>

#include <conio.h>

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "credui.lib")

#define AUTHOR_NAME L"Ivan"

//////////////////////////////////////////////////////////////////////////////////////////////////////


int Task_Manager_Print_All_Tasks(const bool running = false);

void Task_Manager_Traverse_Task_Folder(ITaskFolder* task_folder, const bool running = false);

void Task_Manager_CreateTask(
    const LPCWSTR wszTaskName,
    const LPCWSTR wszTaskPath,
    const LPCWSTR wszAuthorName,
    const LPCWSTR wszQuery,
    const bool putValueQueries,
    const LPCWSTR wszValueName,
    const LPCWSTR wszValueNum,
    const LPCWSTR wszArgs
);


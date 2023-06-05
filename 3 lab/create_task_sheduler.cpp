#include "task_scheduler.h"



int Task_Manager_Print_All_Tasks(const bool running)
{
    //  ------------------------------------------------------
    //  Initialize COM.
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        printf("\nCoInitializeEx failed: %x", hr);
        return 1;
    }

    //  Set general COM security levels.
    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        0,
        NULL);

    if (FAILED(hr))
    {
        printf("\nCoInitializeSecurity failed: %x", hr);
        CoUninitialize();
        return 1;
    }

    //  ------------------------------------------------------
    //  Create an instance of the Task Service. 
    ITaskService* pService = NULL;
    hr = CoCreateInstance(CLSID_TaskScheduler,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_ITaskService,
        (void**)&pService);
    if (FAILED(hr))
    {
        printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
        CoUninitialize();
        return 1;
    }

    //  Connect to the task service.
    hr = pService->Connect(_variant_t(), _variant_t(),
        _variant_t(), _variant_t());
    if (FAILED(hr))
    {
        printf("ITaskService::Connect failed: %x", hr);
        pService->Release();
        CoUninitialize();
        return 1;
    }

    //  ------------------------------------------------------
    //  Get the pointer to the root task folder.
    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);

    pService->Release();
    if (FAILED(hr))
    {
        printf("Cannot get Root Folder pointer: %x", hr);
        CoUninitialize();
        return 1;
    }

    Task_Manager_Traverse_Task_Folder(pRootFolder, running);

    CoUninitialize();
    return 0;
}


void Task_Manager_Traverse_Task_Folder(ITaskFolder* task_folder, const bool running)
{
    //Contains all the tasks that are registered.
    IRegisteredTaskCollection* pTaskCollection = nullptr;
    auto hr = task_folder->GetTasks(NULL, &pTaskCollection);
    //Provides information and control for a collection of folders that contain tasks
    ITaskFolderCollection* pFolderCollection = nullptr;

    hr = task_folder->GetFolders(NULL, &pFolderCollection);
    task_folder->Release();

    if (FAILED(hr))
    {
        std::cerr << "Cannot get any registered tasks or folder: " << hr << std::endl;
        CoUninitialize();

        return;
    }

    LONG numTasks, numFolders = 0;
    hr = pTaskCollection->get_Count(&numTasks);
    hr = pFolderCollection->get_Count(&numFolders);

    if (numTasks > 0)
    {
        TASK_STATE taskState;

        for (LONG i = 0; i < numTasks; i++)
        {
            IRegisteredTask* pRegisteredTask = nullptr;
            hr = pTaskCollection->get_Item(_variant_t(i + 1), &pRegisteredTask);

            if (SUCCEEDED(hr))
            {
                BSTR taskName = nullptr;
                hr = pRegisteredTask->get_Name(&taskName);

                if (SUCCEEDED(hr))
                {
                    hr = pRegisteredTask->get_State(&taskState);

                    if (SUCCEEDED(hr))
                    {
                        switch (taskState)
                        {
                        case TASK_STATE_UNKNOWN:
                            if (!running)
                                std::wcout << std::setw(90) << std::left << taskName << " | " << std::setw(20) << std::left << "Unknown" << std::endl;
                            break;
                        case TASK_STATE_DISABLED:
                            if (!running)
                                std::wcout << std::setw(90) << std::left << taskName << " | " << std::setw(20) << std::left << "Disabled" << std::endl;
                            break;
                        case TASK_STATE_QUEUED:
                            if (!running)
                                std::wcout << std::setw(90) << std::left << taskName << " | " << std::setw(20) << std::left << "Queued" << std::endl;
                            break;
                        case TASK_STATE_READY:
                            if (!running)
                                std::wcout << std::setw(90) << std::left << taskName << " | " << std::setw(20) << std::left << "Ready" << std::endl;
                            break;
                        case TASK_STATE_RUNNING:
                            std::wcout << std::setw(90) << std::left << taskName << " | " << std::setw(20) << std::left << "Running" << std::endl;
                            break;
                        }
                    }
                    else
                    {
                        std::cerr << "\tCannot get the registered task state: " << hr << std::endl;
                    }

                    SysFreeString(taskName);
                }
                else
                {
                    std::cerr << "\tCannot get the registered task name: " << hr << std::endl;
                }

                pRegisteredTask->Release();
            }
            else
            {
                std::cerr << "\tCannot get the registered task item at index=" << i + 1 << ": " << hr << std::endl;
            }
        }
    }

    pTaskCollection->Release();

    for (LONG i = 0; i < numFolders; i++)
    {
        ITaskFolder* taskSubFolder = nullptr;
        pFolderCollection->get_Item(_variant_t(i + 1), &taskSubFolder);
        Task_Manager_Traverse_Task_Folder(taskSubFolder, running);
    }

    pFolderCollection->Release();
}


void Task_Manager_CreateTask(
    const LPCWSTR wszTaskName,
    const LPCWSTR wszTaskPath,
    const LPCWSTR wszAuthorName,
    const LPCWSTR wszQuery,
    const bool putValueQueries,
    const LPCWSTR wszValueName,
    const LPCWSTR wszValueNum,
    const LPCWSTR wszArgs)
{
    //  ------------------------------------------------------
    //  Initialize COM.
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);//Инициализирует библиотеку COM для использования вызывающим потоком, устанавливает модель параллелизма потока 
    if (FAILED(hr))
    {
        printf("\nCoInitializeEx failed: %x", hr);
        return;
    }

    //  Set general COM security levels.
    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_PKT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        0,
        NULL);

    if (FAILED(hr))
    {
        printf("\nCoInitializeSecurity failed: %x", hr);
        CoUninitialize();
        return;
    }

    //  ------------------------------------------------------
    //  Create an COM-instance of the Task Service. 
    ITaskService* pService = NULL;
    hr = CoCreateInstance(CLSID_TaskScheduler,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_ITaskService,
        (void**)&pService);
    if (FAILED(hr))
    {
        printf("Failed to create an instance of ITaskService: %x", hr);
        CoUninitialize();
        return;
    }

    //  Connect to the task service.
    hr = pService->Connect(_variant_t(), _variant_t(),
        _variant_t(), _variant_t());
    if (FAILED(hr))
    {
        printf("ITaskService::Connect failed: %x", hr);
        pService->Release();
        CoUninitialize();
        return;
    }

    //  ------------------------------------------------------
    //  Get the pointer to the root task folder.  
    //  This folder will hold the new task that is registered.
    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr))
    {
        printf("Cannot get Root Folder pointer: %x", hr);
        pService->Release();
        CoUninitialize();
        return;
    }

    //  If the same task exists, remove it.
    pRootFolder->DeleteTask(_bstr_t(wszTaskName), 0);

    //  Create the task builder object to create the task.
    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);

    pService->Release();  // COM clean up.  Pointer is no longer used.
    if (FAILED(hr))
    {
        printf("Failed to create a task definition: %x", hr);
        pRootFolder->Release();
        CoUninitialize();
        return;
    }

    //  ------------------------------------------------------
    //  Get the registration info for setting the identification.
    IRegistrationInfo* pRegInfo = NULL;
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (FAILED(hr))
    {
        printf("\nCannot get identification pointer: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    hr = pRegInfo->put_Author(_bstr_t(wszAuthorName));
    pRegInfo->Release();  // COM clean up.  Pointer is no longer used.
    if (FAILED(hr))
    {
        printf("\nCannot put identification info: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    //  Create the settings for the task
    //Gets or sets the information that specifies how the Task Scheduler performs tasks when the computer is in an idle condition
    ITaskSettings* pSettings = NULL;
    hr = pTask->get_Settings(&pSettings);
    if (FAILED(hr))
    {
        std::cerr << "Cannot get settings pointer: " << hr << std::endl;
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    //  Set setting values for the task.
    hr = pSettings->put_StartWhenAvailable(VARIANT_TRUE);
    pSettings->Release();
    if (FAILED(hr))
    {
        std::cerr << "Cannot put setting info: " << hr << std::endl;
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    ITriggerCollection* pTriggerCollection = NULL;
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr))
    {
        std::cerr << "Cannot get trigger collection: " << hr << std::endl;
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    //  Add the event trigger to the task.
    ITrigger* pTrigger = NULL;
    hr = pTriggerCollection->Create(TASK_TRIGGER_EVENT, &pTrigger);
    pTriggerCollection->Release();
    if (FAILED(hr))
    {
        std::cerr << "Cannot create the trigger: " << hr << std::endl;
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    // Call ITask::QueryInterface to retrieve the IProvideTaskPage interface
    IEventTrigger* pEventTrigger = NULL;
    hr = pTrigger->QueryInterface(IID_IEventTrigger, (void**)&pEventTrigger);
    pTrigger->Release();
    if (FAILED(hr))
    {
        std::cerr << "QueryInterface call on IEventTrigger failed: " << hr << std::endl;
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    hr = pEventTrigger->put_Id(_bstr_t(L"Trigger_1"));
    if (FAILED(hr))
        std::cerr << "Cannot put the trigger ID: " << hr << std::endl;

    hr = pEventTrigger->put_Subscription(_bstr_t(wszQuery));
    if (FAILED(hr))
    {
        std::cerr << "Cannot put the event query: " << hr << std::endl;
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    if (putValueQueries == TRUE)
    {
        ITaskNamedValueCollection* pValueQueries = NULL;
        pEventTrigger->get_ValueQueries(&pValueQueries);
        pValueQueries->Create(_bstr_t(wszValueName), _bstr_t(wszValueNum), NULL);
        hr = pEventTrigger->put_ValueQueries(pValueQueries);
        if (FAILED(hr))
        {
            std::cerr << "Cannot put value queries: " << hr << std::endl;
            pRootFolder->Release();
            pTask->Release();
            CoUninitialize();
            return;
        }
    }

    //  Get the task action collection pointer
    pEventTrigger->Release();
    IActionCollection* pActionCollection = NULL;
    hr = pTask->get_Actions(&pActionCollection);
    if (FAILED(hr))
    {
        std::cerr << "Cannot get action collection pointer: " << hr << std::endl;
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    //  Create the action, specifying that it is an executable action.
    IAction* pAction = NULL;
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    pActionCollection->Release();
    if (FAILED(hr))
    {
        std::cerr << "Cannot create the action: " << hr << std::endl;
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    //  QI for the executable task pointer.
    IExecAction2* pExecAction = NULL;
    hr = pAction->QueryInterface(IID_IExecAction2, (void**)&pExecAction);
    pAction->Release();
    if (FAILED(hr))
    {
        std::cerr << "QueryInterface call failed for IEmailAction: " << std::hex << hr << std::endl;
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    //  Set the path of the executable
    hr = pExecAction->put_Path(_bstr_t(wszTaskPath));
    if (FAILED(hr))
    {
        std::cerr << "Cannot put path information: " << hr << std::endl;
        pRootFolder->Release();
        pExecAction->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    //  Set arguments to the executable
    hr = pExecAction->put_Arguments(_bstr_t(wszArgs));
    if (FAILED(hr))
    {
        std::cerr << "Cannot put arguments information: " << hr << std::endl;
        pRootFolder->Release();
        pExecAction->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    //  Save the task in the root folder
    pExecAction->Release();
    IRegisteredTask* pRegisteredTask = NULL;
    hr = pRootFolder->RegisterTaskDefinition(_bstr_t(wszTaskName), pTask,
        TASK_CREATE_OR_UPDATE, _variant_t(_bstr_t(L"")), _variant_t(_bstr_t(L"")),
        TASK_LOGON_INTERACTIVE_TOKEN, _variant_t(L""), &pRegisteredTask);
    if (FAILED(hr))
    {
        std::cerr << "Error saving the Task : " << std::hex << hr << std::endl;
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return;
    }

    std::wcout << "Task: " << wszTaskName << " succesfully registered" << std::endl;
    pRootFolder->Release();
    pTask->Release();
    pRegisteredTask->Release();
    CoUninitialize();
}

#include "task_scheduler.h"


void Main_Menu()
{
    std::cout << "Enter the task\n";
    std::cout << "1 - Show all tasks\n"
        << "2 - Defender task\n"
        << "3 - Firewall task\n"
        << "4 - Ping task\n"
        << "5 - Exit";

    int choice = 0;


    while (choice != 5)
    {
        std::cout << "\n>> ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            Task_Manager_Print_All_Tasks();
            break;
        }
        case 2:
        {
            Task_Manager_CreateTask(
                L"Windows_Defender_Task",
                L"C:\\defender_scenario.exe",
                AUTHOR_NAME,
                L"<QueryList>\
				    <Query Id=\"0\" Path=\"Microsoft-Windows-Windows Defender/Operational\">\
				    <Select Path=\"Microsoft-Windows-Windows Defender/Operational\">*[System[Provider[@Name='Microsoft-Windows-Windows Defender'] and (EventID = 5007)]]</Select>\
				    <Select Path=\"Microsoft-Windows-Windows Defender/WHC\">*[System[Provider[@Name='Microsoft-Windows-Windows Defender'] and (EventID = 5007)]]</Select>\
				    </Query>\
				    </QueryList>",
                FALSE,
                L"",
                L"",
                L"defender"
            );

            break;
        }
        case 3:
        {
            Task_Manager_CreateTask(
                L"Windows_Firewall_Task",
                L"C:\\firewall_scenario.exe",
                AUTHOR_NAME,
                L"<QueryList>\
				    <Query Id=\"0\" Path=\"Microsoft-Windows-Windows Firewall With Advanced Security/Firewall\">\
				    <Select Path=\"Microsoft-Windows-Windows Firewall With Advanced Security/Firewall\">*</Select>\
				    </Query>\
				    </QueryList>",
                FALSE,
                L"",
                L"",
                L"firewall");
            break;
        }
        case 4:
        {
            Task_Manager_CreateTask(
                L"Windows_Ping_Task",
                L"C:\\ping_scenario.exe",
                AUTHOR_NAME,
                L"<QueryList><Query Id='0' Path='Security'><Select Path='Security'>*[System[Provider[@Name='Microsoft-Windows-Security-Auditing'] and EventID=5152]]</Select></Query></QueryList>",
                TRUE,
                L"eventData",
                L"Event/EventData/Data[@Name='SourceAddress']",
                L"$(eventData)");
            break;
        }
        case 5:
        {
            system("pause");
            return;
        }
        default:
            std::cout << "Wrong command\n";
            break;
        }
    }
}


int main()
{
    Main_Menu();
    return 0;
}

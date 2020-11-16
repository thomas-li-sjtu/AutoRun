#define _WIN32_DCOM
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
//  Include the task header file.
#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

using namespace std;

void get_folders(ITaskFolder* root_folder, HRESULT hr);
void get_tasks(ITaskFolder* root_folder, HRESULT hr);

int __cdecl wmain()
{
	//COM 初始化
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		printf("\nCoInitializeEx failed: %x", hr);
		return 1;
	}

	//设置COM安全级别.
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

	//实例化Task Service
	ITaskService* pService = NULL;
	hr = CoCreateInstance(CLSID_TaskScheduler,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskService,
		(void**)&pService);
	if (FAILED(hr))
	{
		printf("\nFailed to CoCreate an instance of the TaskService class: %x", hr);
		CoUninitialize();
		return 1;
	}

	//连接task service
	hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
	if (FAILED(hr))
	{
		printf("\nITaskService::Connect failed: %x", hr);
		pService->Release();
		CoUninitialize();
		return 1;
	}

	//root指针
	ITaskFolder* pRootFolder = NULL;
	hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
	if (FAILED(hr))  //无法获得指针
		return 1;

	//打印文件夹中的注册任务
	get_folders(pRootFolder, hr);

	pRootFolder->Release();
	pService->Release();
	CoUninitialize();

	system("pause");
	return 0;
}

//递归搜索子文件夹
void get_folders(ITaskFolder* root_folder, HRESULT hr)
{
	ITaskFolderCollection* pFolders = NULL;
	hr = root_folder->GetFolders(0, &pFolders);
	get_tasks(root_folder, hr);  //获取本目录下的task
	if (SUCCEEDED(hr))
	{
		LONG count_folders = 0; //目录下文件夹数目
		hr = pFolders->get_Count(&count_folders);

		if (count_folders != 0) {
			for (LONG i = 0; i < count_folders; i++) {
				ITaskFolder* tmp_folder = NULL;
				hr = pFolders->get_Item(_variant_t(i + 1), &tmp_folder);
				if (SUCCEEDED(hr)) {
					BSTR folder_name = NULL;
					hr = tmp_folder->get_Name(&folder_name);
					if (FAILED(hr))
						return;
					get_folders(tmp_folder, hr); //递归
				}
			}
		}
	}
}

//文件夹中已注册的任务
void get_tasks(ITaskFolder* folder, HRESULT hr)
{
	IRegisteredTaskCollection* ptask_collection = NULL;
	hr = folder->GetTasks(NULL, &ptask_collection);
	if (FAILED(hr))  //无法获得任务
		return;

	LONG count_tasks = 0;
	hr = ptask_collection->get_Count(&count_tasks);
	if (!count_tasks)  //当前任务为0
		return;

	for (LONG i = 0; i < count_tasks; i++)
	{
		IRegisteredTask* pRegisteredTask = NULL;
		hr = ptask_collection->get_Item(_variant_t(i + 1), &pRegisteredTask);

		if (SUCCEEDED(hr))
		{
			BSTR task_name = NULL;
			hr = pRegisteredTask->get_Name(&task_name);
			if (SUCCEEDED(hr))
			{
				printf("\n\tTaskName: %S", task_name);  //任务名
				
				ITaskDefinition* defination = NULL;
				IActionCollection* actions = NULL;
				IAction* action = NULL;
				IExecAction* exec = NULL;
				BSTR image_path = NULL;
				hr = pRegisteredTask->get_Definition(&defination);
				if (SUCCEEDED(hr))
				{
					hr = defination->get_Actions(&actions);
					if (SUCCEEDED(hr))
					{
						hr = actions->get_Item(1, &action);
						if (SUCCEEDED(hr))
						{
							hr = action->QueryInterface(IID_IExecAction, (void**)&exec);
							if (SUCCEEDED(hr))
							{
								hr = exec->get_Path(&image_path);
								if (SUCCEEDED(hr))
									printf("\tImage Path: %S", image_path);
							}
							else 
								return;
						}
						else
							return;
					}
					else
						return;
				}
				else
					return;
			}
		}
	}
}
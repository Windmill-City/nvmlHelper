// NVML_Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <wtypes.h>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <windows.h>

typedef bool (*lpnv_init)();
typedef bool (*lpnv_shutdown)();
typedef bool (*lpnv_getCount)(unsigned int* count);
typedef bool (*lpnv_getNameByIndex)(int index, char* name, int* size);
typedef bool (*lpnv_getPowerUsageByIndex)(int index, unsigned int* power);
typedef bool (*lpnv_getTemperatureByIndex)(int index, unsigned int* temp);
typedef bool (*lpnv_getUtilizationRatesByIndex)(int index, unsigned int* u_memory, unsigned int* u_gpu);
typedef bool (*lpnv_getEncoderUtilizationByIndex)(int index, unsigned int* uiVidEncoderUtil, unsigned int* uiVideEncoderLastSample);
typedef bool (*lpnv_getDecoderUtilizationByIndex)(int index, unsigned int* uiVidEncoderUtil, unsigned int* uiVideEncoderLastSample);
typedef bool (*lpnv_getMemoryInfoByIndex)(int index, unsigned int* ulFrameBufferTotalMBytes, unsigned int* ulFrameBufferFreeMBytes);

int main()
{
	HINSTANCE hDll;//DLL 句柄

    lpnv_init _lpnv_init;
    lpnv_shutdown _lpnv_shutdown;
    lpnv_getCount _lpnv_getCount;
    lpnv_getNameByIndex _lpnv_getNameByIndex;
    lpnv_getPowerUsageByIndex _lpnv_getPowerUsageByIndex;
    lpnv_getTemperatureByIndex _lpnv_getTemperatureByIndex;
    lpnv_getUtilizationRatesByIndex _lpnv_getUtilizationRatesByIndex;
    lpnv_getEncoderUtilizationByIndex _lpnv_getEncoderUtilizationByIndex;
    lpnv_getDecoderUtilizationByIndex _lpnv_getDecoderUtilizationByIndex;
	lpnv_getMemoryInfoByIndex _lpnv_getMemoryInfoByIndex;

	hDll = LoadLibrary(L"NV_Helper.dll");//动态获取dll文件的路径
	if (hDll != NULL)
	{
		_lpnv_init = (lpnv_init)GetProcAddress(hDll, "nv_init");
		_lpnv_shutdown = (lpnv_shutdown)GetProcAddress(hDll, "nv_shutdown");
		_lpnv_getCount = (lpnv_getCount)GetProcAddress(hDll, "nv_getCount");
		_lpnv_getNameByIndex = (lpnv_getNameByIndex)GetProcAddress(hDll, "nv_getNameByIndex");
		_lpnv_getPowerUsageByIndex = (lpnv_getPowerUsageByIndex)GetProcAddress(hDll, "nv_getPowerUsageByIndex");
		_lpnv_getTemperatureByIndex = (lpnv_getTemperatureByIndex)GetProcAddress(hDll, "nv_getTemperatureByIndex");
		_lpnv_getUtilizationRatesByIndex = (lpnv_getUtilizationRatesByIndex)GetProcAddress(hDll, "nv_getUtilizationRatesByIndex");
		_lpnv_getEncoderUtilizationByIndex = (lpnv_getEncoderUtilizationByIndex)GetProcAddress(hDll, "nv_getEncoderUtilizationByIndex");
		_lpnv_getDecoderUtilizationByIndex = (lpnv_getDecoderUtilizationByIndex)GetProcAddress(hDll, "nv_getDecoderUtilizationByIndex");
		_lpnv_getMemoryInfoByIndex = (lpnv_getMemoryInfoByIndex)GetProcAddress(hDll, "nv_getMemoryInfoByIndex");
		if (_lpnv_init != NULL)
		{
			bool result = _lpnv_init();
			printf("Inited:%d\r\n", result);
			if (result) {
				//Get gpu count
				unsigned int count;
				_lpnv_getCount(&count);
				printf("Gpu Count:%d\r\n", count);
				//Get Name
				int size;
				_lpnv_getNameByIndex(0, NULL, &size);
				char* name = new char[size];
				_lpnv_getNameByIndex(0, name, NULL);
				printf("Gpu Name:%s\r\n", name);
				//Get Power
				unsigned int power;
				double start = GetTickCount();
				for (int i = 0; i < 1; i++)
				{
					_lpnv_getPowerUsageByIndex(0, &power);
				}
				double end = GetTickCount();
				double cost = end - start;
				printf("Cost:%f\r\n", cost);
				printf("Gpu Power:%u\r\n", power);
				//Get Temp
				unsigned int temp;
				_lpnv_getTemperatureByIndex(0, &temp);
				printf("Gpu Temp:%u\r\n", temp);
				//Get UtilRate
				unsigned int gpu;
				unsigned int memory;
				_lpnv_getUtilizationRatesByIndex(0, &memory, &gpu);
				printf("Gpu Usage:%u %% Memory:%u %%\r\n", gpu, memory);
				//Get Encoder usage
				unsigned int uiVidEncoderUtil;
				unsigned int uiVideEncoderLastSample;
				_lpnv_getEncoderUtilizationByIndex(0, &uiVidEncoderUtil, &uiVideEncoderLastSample);
				printf("Encoder Usage: %u %% LastSample: %u\r\n", uiVidEncoderUtil, uiVideEncoderLastSample);
				//Get Decoder usage
				unsigned int uiVidDecoderUtil;
				unsigned int uiVideDecoderLastSample;
				_lpnv_getDecoderUtilizationByIndex(0, &uiVidDecoderUtil, &uiVideDecoderLastSample);
				printf("Decoder Usage: %u %% LastSample: %u\r\n", uiVidDecoderUtil, uiVideDecoderLastSample);
				//Get Gpu Memory
				unsigned int ulFrameBufferTotalMBytes, ulFrameBufferFreeMBytes;
				_lpnv_getMemoryInfoByIndex(0, &ulFrameBufferTotalMBytes, &ulFrameBufferFreeMBytes);
				printf("Total:%d MB Free:%d MB\r\n", ulFrameBufferTotalMBytes, ulFrameBufferFreeMBytes);
				//Release res
				result = _lpnv_shutdown();
				printf("ShutDown:%d\r\n", result);
			}
		}

		FreeLibrary(hDll);
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

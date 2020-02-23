#pragma once

extern "C" {
	bool __declspec(dllexport) nv_init();
	bool __declspec(dllexport) nv_shutdown();
	bool __declspec(dllexport) nv_getCount(unsigned int* count);
	bool __declspec(dllexport) nv_getNameByIndex(int index, char* name, int* size);
	bool __declspec(dllexport) nv_getPowerUsageByIndex(int index, unsigned int* power);
	bool __declspec(dllexport) nv_getTemperatureByIndex(int index, unsigned int* temp);
	bool __declspec(dllexport) nv_getUtilizationRatesByIndex(int index, unsigned int* u_memory, unsigned int* u_gpu);
	bool __declspec(dllexport) nv_getEncoderUtilizationByIndex(int index, unsigned int* uiVidEncoderUtil, unsigned int* uiVideEncoderLastSamplePeriodUs);
	bool __declspec(dllexport) nv_getDecoderUtilizationByIndex(int index, unsigned int* uiVidDecoderUtil, unsigned int* uiVideDeoderLastSamplePeriodUs);
	bool __declspec(dllexport) nv_getMemoryInfoByIndex(int index, unsigned int* ulFrameBufferTotalMBytes, unsigned int* ulFrameBufferFreeMBytes);
}
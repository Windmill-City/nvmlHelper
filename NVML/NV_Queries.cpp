#include "pch.h"

#include <Windows.h>
#include <stdio.h>

#include "NV_Queries.h"

#include "nvml.h"

// NVML Function pointer prototypes
typedef nvmlReturn_t(*PFNnvmlInit)(void);
typedef nvmlReturn_t(*PFNnvmlShutdown)(void);
typedef char* (*PFNnvmlErrorString)(nvmlReturn_t result);
typedef nvmlReturn_t(*PFNnvmlDeviceGetCount)(unsigned int* deviceCount);
typedef nvmlReturn_t(*PFNnvmlDeviceGetHandleByIndex)(unsigned int index, nvmlDevice_t* device);
typedef nvmlReturn_t(*PFNnvmlDeviceGetUtilizationRates)(nvmlDevice_t device, nvmlUtilization_t* utilization);
typedef nvmlReturn_t(*PFNnvmlDeviceGetEncoderUtilization)(nvmlDevice_t device, unsigned int* utilization, unsigned int* samplingPeriodUs);
typedef nvmlReturn_t(*PFNnvmlDeviceGetDecoderUtilization)(nvmlDevice_t device, unsigned int* utilization, unsigned int* samplingPeriodUs);
typedef nvmlReturn_t(*PFNnvmlDeviceGetMemoryInfo)(nvmlDevice_t device, nvmlMemory_t* memory);
typedef nvmlReturn_t(*PFNnvmlDeviceGetName)(nvmlDevice_t device, char* name, unsigned int length);
typedef nvmlReturn_t(*PFNnvmlDeviceGetPowerUsage)(nvmlDevice_t device, unsigned int* power);
typedef nvmlReturn_t(*PFNnvmlDeviceGetTemperature)(nvmlDevice_t device, nvmlTemperatureSensors_t sensorType, unsigned int* temp);

// NVML Function pointer instances
PFNnvmlInit pfn_nvmlInit = NULL;
PFNnvmlShutdown pfn_nvmlShutdown = NULL;
PFNnvmlErrorString pfn_nvmlErrorString = NULL;
PFNnvmlDeviceGetCount pfn_nvmlDeviceGetCount = NULL;
PFNnvmlDeviceGetHandleByIndex pfn_nvmlDeviceGetHandleByIndex = NULL;
PFNnvmlDeviceGetUtilizationRates pfn_nvmlDeviceGetUtilizationRates = NULL;
PFNnvmlDeviceGetEncoderUtilization pfn_nvmlDeviceGetEncoderUtilization = NULL;
PFNnvmlDeviceGetDecoderUtilization pfn_nvmlDeviceGetDecoderUtilization = NULL;
PFNnvmlDeviceGetMemoryInfo pfn_nvmlDeviceGetMemoryInfo = NULL;
PFNnvmlDeviceGetName pfn_nvmlDeviceGetName = NULL;
PFNnvmlDeviceGetPowerUsage pfn_nvmlDeviceGetPowerUsage = NULL;
PFNnvmlDeviceGetTemperature pfn_nvmlDeviceGetTemperature = NULL;

HMODULE hDLLhandle;
nvmlDevice_t *devices;

bool nv_init() {
	bool ret = false;
	nvmlReturn_t nvRetValue = NVML_ERROR_UNINITIALIZED;
	hDLLhandle = LoadLibrary(L"NVML.DLL");

	if (hDLLhandle != NULL) {
		// Get the function pointers from the DLL
		pfn_nvmlInit = (PFNnvmlInit)GetProcAddress(hDLLhandle, "nvmlInit");
		pfn_nvmlShutdown = (PFNnvmlShutdown)GetProcAddress(hDLLhandle, "nvmlShutdown");
		pfn_nvmlErrorString = (PFNnvmlErrorString)GetProcAddress(hDLLhandle, "nvmlErrorString");
		pfn_nvmlDeviceGetCount = (PFNnvmlDeviceGetCount)GetProcAddress(hDLLhandle, "nvmlDeviceGetCount");
		pfn_nvmlDeviceGetHandleByIndex = (PFNnvmlDeviceGetHandleByIndex)GetProcAddress(hDLLhandle, "nvmlDeviceGetHandleByIndex");
		pfn_nvmlDeviceGetName = (PFNnvmlDeviceGetName)GetProcAddress(hDLLhandle, "nvmlDeviceGetName");
		pfn_nvmlDeviceGetUtilizationRates = (PFNnvmlDeviceGetUtilizationRates)GetProcAddress(hDLLhandle, "nvmlDeviceGetUtilizationRates");
		pfn_nvmlDeviceGetEncoderUtilization = (PFNnvmlDeviceGetEncoderUtilization)GetProcAddress(hDLLhandle, "nvmlDeviceGetEncoderUtilization");
		pfn_nvmlDeviceGetDecoderUtilization = (PFNnvmlDeviceGetDecoderUtilization)GetProcAddress(hDLLhandle, "nvmlDeviceGetDecoderUtilization");
		pfn_nvmlDeviceGetMemoryInfo = (PFNnvmlDeviceGetMemoryInfo)GetProcAddress(hDLLhandle, "nvmlDeviceGetMemoryInfo");
		pfn_nvmlDeviceGetPowerUsage = (PFNnvmlDeviceGetPowerUsage)GetProcAddress(hDLLhandle, "nvmlDeviceGetPowerUsage");
		pfn_nvmlDeviceGetTemperature = (PFNnvmlDeviceGetTemperature)GetProcAddress(hDLLhandle, "nvmlDeviceGetTemperature");

		// Before any of the NVML functions can be used nvmlInit() must be called
		nvRetValue = pfn_nvmlInit();
		if (NVML_SUCCESS != nvRetValue)
		{
			// Can not call the NVML specific error string handler if the initialization failed
			FreeLibrary(hDLLhandle);
			hDLLhandle = NULL;
		}
		ret = true;
		unsigned int count;
		pfn_nvmlDeviceGetCount(&count);
		devices = new nvmlDevice_t[count];
		for (unsigned int i = 0; i < count; i++)
		{
			nvmlDevice_t device;
			pfn_nvmlDeviceGetHandleByIndex(i, &device);
			devices[i] = device;
		}
	}
	return ret;
}

bool nv_shutdown() {
	bool ret = false;
	nvmlReturn_t nvRetValue = NVML_ERROR_UNINITIALIZED;

	if (hDLLhandle == NULL) {
		return ret;
	}
	nvRetValue = pfn_nvmlShutdown();
	if (NVML_SUCCESS == nvRetValue) {
		ret = true;
	}
	FreeLibrary(hDLLhandle);
	hDLLhandle = NULL;
	return ret;
}

bool nv_getCount(unsigned int* count) {
	return pfn_nvmlDeviceGetCount(count) == NVML_SUCCESS;
}

bool nv_getNameByIndex(int index, char* name, int* size) {
	if (!name) {
		*size = NVML_DEVICE_NAME_BUFFER_SIZE;
		return false;
	}
	return NVML_SUCCESS == pfn_nvmlDeviceGetName(devices[index], name, NVML_DEVICE_NAME_BUFFER_SIZE);;
}

bool nv_getPowerUsageByIndex(int index, unsigned int* power) {
	return NVML_SUCCESS == pfn_nvmlDeviceGetPowerUsage(devices[index], power);;
}

bool nv_getTemperatureByIndex(int index, unsigned int* temp) {
	return pfn_nvmlDeviceGetTemperature(devices[index], nvmlTemperatureSensors_enum::NVML_TEMPERATURE_GPU, temp) == NVML_SUCCESS;
}

bool nv_getUtilizationRatesByIndex(int index, unsigned int* u_memory, unsigned int* u_gpu) {
	nvmlReturn_t nvRetValue = NVML_ERROR_UNINITIALIZED;
	nvmlUtilization_t utilization;
	nvRetValue = pfn_nvmlDeviceGetUtilizationRates(devices[index], &utilization);
	*u_gpu = utilization.gpu;
	*u_memory = utilization.memory;
	return nvRetValue == NVML_SUCCESS;
}

bool nv_getEncoderUtilizationByIndex(int index, unsigned int* uiVidEncoderUtil, unsigned int* uiVideEncoderLastSamplePeriodUs) {
	return pfn_nvmlDeviceGetEncoderUtilization(devices[index], uiVidEncoderUtil, uiVideEncoderLastSamplePeriodUs) == NVML_SUCCESS;
}

bool nv_getDecoderUtilizationByIndex(int index, unsigned int* uiVidDecoderUtil, unsigned int* uiVideDecoderLastSamplePeriodUs) {
	return pfn_nvmlDeviceGetDecoderUtilization(devices[index], uiVidDecoderUtil, uiVideDecoderLastSamplePeriodUs) == NVML_SUCCESS;
}

bool nv_getMemoryInfoByIndex(int index, unsigned int* ulFrameBufferTotalMBytes, unsigned int* ulFrameBufferFreeMBytes) {
	nvmlReturn_t nvRetValue = NVML_ERROR_UNINITIALIZED;
	nvmlMemory_t GPUmemoryInfo;
	ZeroMemory(&GPUmemoryInfo, sizeof(GPUmemoryInfo));

	nvRetValue = pfn_nvmlDeviceGetMemoryInfo(devices[index], &GPUmemoryInfo);

	// convert the frame buffer value to MBytes
	*ulFrameBufferTotalMBytes = GPUmemoryInfo.total / (1024 * 1024);
	*ulFrameBufferFreeMBytes = GPUmemoryInfo.free / (1024 * 1024);
	return nvRetValue == NVML_SUCCESS;
}
#ifndef __SCIDK_API_LV_C_H
#define __SCIDK_API_LV_C_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef SCIDK_API_LV_EXPORTS
#define SCIDK_API_LV extern "C" __declspec(dllexport) 
	//__declspec(dllexport)
#else
#define SCIDK_API_LV __declspec(dllimport)
#endif

	SCISDK_LV_DLL_API int SCISDK_GetParameterString(char Path[], char value[], unsigned long long handle);

#ifdef __cplusplus
}
#endif

#endif // __SCIDK_API_LV_C_H
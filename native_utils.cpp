#include "extension.h"

int AddressParamOffset(IPluginContext *pContext) {
	return pContext->GetRuntime()->FindPubvarByName("__Int64_Address__", nullptr) == SP_ERROR_NONE ? 1 : 0;
}

cell_t ReturnAddress(IPluginContext *pContext, const cell_t *params, uintptr_t address) {
	if (AddressParamOffset(pContext)) {
		cell_t* result;
		if (int err = pContext->LocalToPhysAddr(params[1], &result); err != SP_ERROR_NONE) {
			return pContext->ThrowNativeErrorEx(err, "Could not write the return value");
		}
		*reinterpret_cast<int64_t*>(result) = address;
	}
	return address;
}

cell_t sm_CellRefToAddress(IPluginContext *pContext, const cell_t *params) {
#ifndef PLATFORM_64BITS
	cell_t* value;
	if (pContext->LocalToPhysAddr(params[1 + AddressParamOffset(pContext)], &value) != SP_ERROR_NONE) {
		return pContext->ThrowNativeError("Failed to convert cell reference to address.");
	}
	return ReturnAddress(pContext, params, reinterpret_cast<uintptr_t>(value));
#else
	return pContext->ThrowNativeError("GetAddressOfCell is not implemented for 64-bit platforms");
#endif

}

cell_t sm_CharArrayToAddress(IPluginContext *pContext, const cell_t *params) {
#ifndef PLATFORM_64BITS
	char* buffer;
	if (pContext->LocalToString(params[1 + AddressParamOffset(pContext)], &buffer) != SP_ERROR_NONE) {
		return pContext->ThrowNativeError("Failed to convert string reference to address.");
	}
	return ReturnAddress(pContext, params, reinterpret_cast<uintptr_t>(buffer));
#else
	return pContext->ThrowNativeError("GetAddressOfString is not implemented for 64-bit platforms");
#endif
}

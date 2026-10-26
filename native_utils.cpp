#include "extension.h"

int AddressParamOffset(IPluginContext *pContext) {
	return pContext->GetRuntime()->FindPubvarByName("__Int64_Address__", nullptr) == SP_ERROR_NONE ? 1 : 0;
}

cell_t ReturnAddress(IPluginContext *pContext, const cell_t *params, int offset, uintptr_t address) {
	if (offset) {
		cell_t* result;
		if (int err = pContext->LocalToPhysAddr(params[1], &result); err != SP_ERROR_NONE) {
			return pContext->ThrowNativeErrorEx(err, "Could not write the return value");
		}
		*reinterpret_cast<int64_t*>(result) = address;
	} else if (static_cast<uint64_t>(address) > UINT32_MAX) {
		return pContext->ThrowNativeError("Address %p does not fit in a cell; recompile the plugin with SourceMod 1.13 or later",
				reinterpret_cast<void*>(address));
	}
	return static_cast<cell_t>(address);
}

cell_t sm_CellRefToAddress(IPluginContext *pContext, const cell_t *params) {
	int offset = AddressParamOffset(pContext);
	cell_t* value;
	if (pContext->LocalToPhysAddr(params[1 + offset], &value) != SP_ERROR_NONE) {
		return pContext->ThrowNativeError("Failed to convert cell reference to address.");
	}
	return ReturnAddress(pContext, params, offset, reinterpret_cast<uintptr_t>(value));
}

cell_t sm_CharArrayToAddress(IPluginContext *pContext, const cell_t *params) {
	int offset = AddressParamOffset(pContext);
	char* buffer;
	if (pContext->LocalToString(params[1 + offset], &buffer) != SP_ERROR_NONE) {
		return pContext->ThrowNativeError("Failed to convert string reference to address.");
	}
	return ReturnAddress(pContext, params, offset, reinterpret_cast<uintptr_t>(buffer));
}

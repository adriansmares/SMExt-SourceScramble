#include "extension.h"

cell_t sm_CellRefToAddress(IPluginContext *pContext, const cell_t *params) {
	cell_t* value;
	if (pContext->LocalToPhysAddr(params[1 + AddressReturnOffset(pContext)], &value) != SP_ERROR_NONE) {
		return pContext->ThrowNativeError("Failed to convert cell reference to address.");
	}
	return ReturnAddress(pContext, params, value);
}

cell_t sm_CharArrayToAddress(IPluginContext *pContext, const cell_t *params) {
	char* buffer;
	if (pContext->LocalToString(params[1 + AddressReturnOffset(pContext)], &buffer) != SP_ERROR_NONE) {
		return pContext->ThrowNativeError("Failed to convert string reference to address.");
	}
	return ReturnAddress(pContext, params, buffer);
}

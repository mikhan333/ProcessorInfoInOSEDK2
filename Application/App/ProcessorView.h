#include <Library/UefiLib.h>
#include "LibSmbiosView.h"

EFI_STATUS
DisplayProcessorInformation(
    SMBIOS_STRUCTURE_POINTER *Struct);

EFI_STATUS
GetSmbiosStruct(
    IN UINT8 StructType,
    OUT SMBIOS_STRUCTURE_POINTER *Struct);

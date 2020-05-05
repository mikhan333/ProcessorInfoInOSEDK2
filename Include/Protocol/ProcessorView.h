#ifndef __PROCESSOR_VIEW_H__
#define __PROCESSOR_VIEW_H__

#include <Uefi.h>
#include <IndustryStandard/SmBios.h>

#define EFI_PROCESSOR_VIEW_PROTOCOL_GUID \
  {0x2b98da01, 0x4e0f, 0x463b, {0xad, 0xa2, 0xe0, 0xed, 0x12, 0xfe, 0xbf, 0x77}}

typedef struct _EFI_PROCESSOR_VIEW_PROTOCOL EFI_PROCESSOR_VIEW_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_DISPLAY_PROCESSOR_INFORMATION)(
    IN EFI_PROCESSOR_VIEW_PROTOCOL      *This,
    IN SMBIOS_STRUCTURE_POINTER         *Struct
);

typedef
EFI_STATUS EFIAPI
(EFIAPI *EFI_SAVE_PROCESSOR_INFORMATION)(
    IN EFI_PROCESSOR_VIEW_PROTOCOL *This,
    IN SMBIOS_STRUCTURE_POINTER *Struct,
    IN CHAR16 *VariableName
);

typedef
EFI_STATUS
(EFIAPI *EFI_GET_SMBIOS_STRUCT)(
    IN  EFI_PROCESSOR_VIEW_PROTOCOL     *This,
    IN  UINT8                           StructType,
    OUT SMBIOS_STRUCTURE_POINTER        *Struct
);

typedef struct _EFI_PROCESSOR_VIEW_PROTOCOL
{
    EFI_DISPLAY_PROCESSOR_INFORMATION               DisplayProcessorInformation;
    EFI_SAVE_PROCESSOR_INFORMATION                  SaveProcessorInformation;
    EFI_GET_SMBIOS_STRUCT                           GetSmbiosStruct;
} EFI_PROCESSOR_VIEW_PROTOCOL;

extern EFI_GUID gEfiProcessorViewProtocolGuid;

#endif //__PROCESSOR_VIEW_H__
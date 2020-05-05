#include <Protocol/ProcessorView.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "LibSmbiosView.h"

EFI_HANDLE ProcessorViewHandle = NULL;

#define PRINT_STRUCT_VALUE(pStruct, type, element) \
  do                                               \
  {                                                \
    Print(L"%a", #element);                        \
    Print(L": %d\n", (pStruct->type->element));    \
  } while (0);

#define BIT(value, bit) ((value) & ((UINT64)1) << (bit))

VOID DisplayProcessorVoltage(
    IN UINT8 Voltage)
{
  Print(L"Processor voltage information:\n");
  if (BIT(Voltage, 7) != 0)
  {
    Print(L"Current voltage = (%d/10)V (hex - 0x%x)\n", (Voltage - 0x80), Voltage);
  }
  else
  {
    if (BIT(Voltage, 0) != 0)
      Print(L"5V supported\n");
    if (BIT(Voltage, 1) != 0)
      Print(L"3.3V supported\n");
    if (BIT(Voltage, 2) != 0)
      Print(L"2.9V supported\n");

    //
    // check the reserved zero bits:
    //
    if (BIT(Voltage, 3) != 0 || BIT(Voltage, 4) != 0 || BIT(Voltage, 5) != 0 || BIT(Voltage, 6) != 0)
    {
      Print(L"Incorrect bits\n");
    }
  }
}

EFI_STATUS EFIAPI
DisplayProcessorInformation(
    IN EFI_PROCESSOR_VIEW_PROTOCOL *This,
    IN SMBIOS_STRUCTURE_POINTER *Struct)
{
  DisplayProcessorVoltage(*(UINT8 *)&(Struct->Type4->Voltage));
  PRINT_STRUCT_VALUE(Struct, Type4, ExternalClock);
  PRINT_STRUCT_VALUE(Struct, Type4, MaxSpeed);
  PRINT_STRUCT_VALUE(Struct, Type4, CurrentSpeed);
  return EFI_SUCCESS;
}

EFI_STATUS EFIAPI
SaveProcessorInformation(
    IN EFI_PROCESSOR_VIEW_PROTOCOL *This,
    IN SMBIOS_STRUCTURE_POINTER *Struct,
    IN CHAR16 *VariableName)
{
  EFI_STATUS Status;
  SMBIOS_TABLE_TYPE4 *Data = Struct->Type4;
  UINTN DataSize = sizeof(SMBIOS_TABLE_TYPE4);
  UINT32 Attributes = EFI_VARIABLE_NON_VOLATILE |
                      EFI_VARIABLE_BOOTSERVICE_ACCESS |
                      EFI_VARIABLE_RUNTIME_ACCESS;

  Status = gRT->SetVariable(
      VariableName,
      &gEfiProcessorViewProtocolGuid,
      Attributes,
      DataSize,
      Data);
  if (EFI_ERROR(Status))
    return Status;
  return EFI_SUCCESS;
}

EFI_STATUS EFIAPI
GetSmbiosStruct(
    IN EFI_PROCESSOR_VIEW_PROTOCOL *This,
    IN UINT8 StructType,
    OUT SMBIOS_STRUCTURE_POINTER *Struct)
{
  UINT16 StructHandle;
  UINTN Index;
  UINT8 *Buffer;
  UINT16 Length;
  EFI_STATUS Status;
  SMBIOS_STRUCTURE_POINTER m_SmbiosStruct;
  SMBIOS_TABLE_ENTRY_POINT *SMBiosTable;

  //
  // Init Lib: get SMBIOS table from System Configure table
  //
  Status = LibSmbiosInit();
  if (EFI_ERROR(Status))
    return Status;

  //
  // Initialize the StructHandle to be the first handle
  //
  StructHandle = INVALID_HANDLE;
  LibGetSmbiosStructure(&StructHandle, NULL, NULL);

  //
  // Get SMBIOS table, which was initialized
  //
  SMBiosTable = NULL;
  LibSmbiosGetEPS(&SMBiosTable);

  //
  // Searching structure info with Hdr->Type == StructType
  //
  for (Index = 0; Index < SMBiosTable->NumberOfSmbiosStructures; Index++)
  {
    //
    // If reach the end of table, break
    //
    if (StructHandle == INVALID_HANDLE)
      break;

    //
    // Handle point to the next
    //
    if (LibGetSmbiosStructure(&StructHandle, &Buffer, &Length) != DMI_SUCCESS)
      break;
    m_SmbiosStruct.Raw = Buffer;

    //
    // Return the correct structure
    //
    if (m_SmbiosStruct.Hdr->Type == StructType)
    {
      *Struct = m_SmbiosStruct;
      return EFI_SUCCESS;
    }
  }
  return EFI_BAD_BUFFER_SIZE;
}

EFI_PROCESSOR_VIEW_PROTOCOL gProcessorView = {
    DisplayProcessorInformation,
    SaveProcessorInformation,
    GetSmbiosStruct,
};

EFI_STATUS EFIAPI UefiDriverMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
  Print(L"##### Driver loading start #####\n");

  EFI_STATUS Status = EFI_SUCCESS;
  Status = gBS->InstallMultipleProtocolInterfaces(
      &ProcessorViewHandle,
      &gEfiProcessorViewProtocolGuid,
      &gProcessorView,
      NULL);
  ASSERT_EFI_ERROR(Status);

  Print(L"##### Driver loading end #####\n");
  return Status;
}

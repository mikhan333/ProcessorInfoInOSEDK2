#include <stdio.h>
#include <stdlib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/ProcessorView.h>
#include <Library/DebugLib.h>

// #include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
// #include <Library/IpmiLib.h>

EFI_PROCESSOR_VIEW_PROTOCOL *PROCESSOR_VIEW_PROTOCOL = NULL;

EFI_STATUS operate()
{
  EFI_STATUS Status;
  SMBIOS_STRUCTURE_POINTER SmbiosStruct;

  //
  // Get SMBIOS structure with processor information (Type 4)
  //
  printf("##### Get smbios structure with processor data #####\n");
  Status = PROCESSOR_VIEW_PROTOCOL->GetSmbiosStruct(
      PROCESSOR_VIEW_PROTOCOL,
      4,
      &SmbiosStruct);
  if (EFI_ERROR(Status))
  {
    printf("ERROR: GetSmbiosStruct() : %d\n", Status);
    return Status;
  }

  //
  // Display processor information
  //
  printf("##### Display processor information #####\n");
  Status = PROCESSOR_VIEW_PROTOCOL->DisplayProcessorInformation(
      PROCESSOR_VIEW_PROTOCOL,
      &SmbiosStruct);
  if (EFI_ERROR(Status))
  {
    printf("ERROR: DisplayProcessorInformation() : %d\n", Status);
    return Status;
  }

  //
  // DEBUG: Get the voltage of processor without functions
  //
  printf("##### Display processor voltage #####\n");
  UINT8 Voltage = *(UINT8 *)&(SmbiosStruct.Type4->Voltage);
  printf("Voltage in hex: 0x%x\n", Voltage);

  //
  // Save processor information into variable
  //
  CHAR16 *VariableName = L"ProcessorInformation";
  printf("##### Save processor information into variable #####\n");
  Status = PROCESSOR_VIEW_PROTOCOL->SaveProcessorInformation(
      PROCESSOR_VIEW_PROTOCOL,
      &SmbiosStruct,
      VariableName);
  if (EFI_ERROR(Status))
  {
    printf("ERROR: SaveProcessorInformation() : %d\n", Status);
    return Status;
  }

  //
  // DEBUG: get variable from RuntimeServices
  //
  SMBIOS_TABLE_TYPE4 *Data;
  Data = GetVariable(
      VariableName,
      &gEfiProcessorViewProtocolGuid);
  if (Data == NULL) {
    printf("ERROR: GetVariable() : EFI_NOT_FOUND\n");
    return EFI_NOT_FOUND;
  }
  Voltage = *(UINT8 *)&(Data->Voltage);
  printf("Voltage after GetVariable in hex: 0x%x\n", Voltage);

  //
  // Exit and return EFI_SUCCESS
  //
  return EFI_SUCCESS;
}

int main(
    IN int argc,
    IN char **argv)
{
  EFI_STATUS Status;
  UINTN NumberProcessorViewHandles;
  EFI_HANDLE *ProcessorViewHandles;

  Status = gBS->LocateHandleBuffer(
      ByProtocol,
      &gEfiProcessorViewProtocolGuid,
      NULL,
      &NumberProcessorViewHandles,
      &ProcessorViewHandles);
  ASSERT_EFI_ERROR(Status);

  EFI_HANDLE fHandel = ProcessorViewHandles[0];

  Status = gBS->OpenProtocol(
      fHandel,
      &gEfiProcessorViewProtocolGuid,
      (VOID **)&PROCESSOR_VIEW_PROTOCOL,
      gImageHandle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
  ASSERT_EFI_ERROR(Status);

  Status = operate();
  ASSERT_EFI_ERROR(Status);

  Status = gBS->CloseProtocol(
      fHandel,
      &gEfiProcessorViewProtocolGuid,
      gImageHandle,
      NULL);
  ASSERT_EFI_ERROR(Status);

  //
  // Attempt to get sensors data directly from sensorc using IPMI
  //

  // UINT8 Data = 0x03;
  // UINT32 DataSize = sizeof(UINT8);

  // UINT8     *ResponseData = NULL;
  // UINT32    *ResponseDataSize = NULL;
  // Status = IpmiSubmitCommand(
  //   0x04,
  //   0x2d,
  //   &Data,
  //   DataSize,
  //   ResponseData,
  //   ResponseDataSize
  // );
  // if (EFI_ERROR(Status))
  // {
  //   printf("ERROR: IpmiSubmitCommand() : %d\n", Status);
  //   return Status;
  // }

  exit(EXIT_SUCCESS);
}

#include <stdio.h>
#include <stdlib.h>
// #include <ctype.h>

#include "ProcessorView.h"

int main(
    IN int argc,
    IN char **argv)
{
  SMBIOS_STRUCTURE_POINTER SmbiosStruct;
  EFI_STATUS Status;

  //
  // Get SMBIOS structure with processor information (Type 4)
  //
  Status = GetSmbiosStruct(4, &SmbiosStruct);
  if (EFI_ERROR(Status))
  {
    printf("ERROR: %d\n", Status);
    exit(EXIT_FAILURE);
  }

  //
  // Display processor information
  //
  DisplayProcessorInformation(&SmbiosStruct);

  exit(EXIT_SUCCESS);
}

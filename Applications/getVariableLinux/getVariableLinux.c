#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/efi.h>

#define EFI_VARIABLE_NAME_SIZE 100

#define EFI_PROCESSOR_VIEW_PROTOCOL_GUID                                               \
    {                                                                                  \
        0x2b98da01, 0x4e0f, 0x463b, { 0xad, 0xa2, 0xe0, 0xed, 0x12, 0xfe, 0xbf, 0x77 } \
    }

#define EFI_PROCESSOR_VIEW_VARIABLE_NAME L"ProcessorInformation"

int main(int argc, char *argv[])
{
    efi_char16_t name[EFI_VARIABLE_NAME_SIZE];
    efi_guid_t guid;
    if (argc == 3)
    {
        strcpy(name, (efi_char16_t *) argv[1]);
        guid = (efi_guid_t) argv[2];
    }
    else
    {
        name = EFI_PROCESSOR_VIEW_VARIABLE_NAME;
        strcpy(name, (efi_char16_t *) EFI_PROCESSOR_VIEW_VARIABLE_NAME);
        guid = (efi_guid_t) EFI_PROCESSOR_VIEW_PROTOCOL_GUID;
    }

    u32 attr;
    u32 data_size = 0;
    u8  *data = NULL;
    efi_status_t status;

    //
    // Get real size of UEFI variable
    //
    status = efi.get_variable(name, &guid, &attr, &data_size, data);
    if (status == EFI_BUFFER_TOO_SMALL)
    {
        //
        // Allocate data buffer of data_size bytes
        //
        data = (u8 *)vmalloc(data_size);
        if (!data)
        {
            printf("ERROR: vmalloc; name of variable - %s", name);
            exit(1);
        }

        //
        // Get variable contents into buffer
        //
        status = efi.get_variable(name, &guid, &attr, &data_size, data);
        if (status != EFI_SUCCESS)
        {
            printf("ERROR: get_variable2; name of variable - %s", name);
            exit(1);
        }

        // TODO: define SMBIOS_TABLE_TYPE4
        SMBIOS_TABLE_TYPE4 *processor_data = (SMBIOS_TABLE_TYPE4 *) data;
        u8 voltage = *(UINT8 *)&(processor_data->Voltage);
        printf("Voltage after GetVariable in hex in Linux: 0x%x\n", voltage);
    }
    else if (status == EFI_NOT_FOUND)
    {
        printf("ERROR: variable didn't found; name of variable - %s", name);
        exit(1);
    }
    else
    {
        printf("ERROR: unrecognizable status; name of variable - %s", name);
        exit(1);
    }
    return 0;
}
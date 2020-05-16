# ProcessorInfoInOSEDK2

This project allows you to get the current processor voltage using the SMBIOS_TABLE structure and transfer it to a variable using SetVariable in runtime environment. In the future, this variable can be used in the operating system.

## Dependencies

EDK II

## Quick start

- Build:

  - Without using make

  ```bash
  build -p ProcessorInfoInOSEDK2/TestPkg.dsc
  ```

  - Use `make build`

- Run (uefi shell):

  - Copy your .efi files to VM or USB with EFI shell and start binaries with commands:

  ```bash
  load ProcessorView.efi
  App.efi
  ```

  - If you have qemu, you can use `make start`

## Examples

```bash
$ load ProcessirView.efi
Image 'FS0:\EFI\ProcessorView.efi' loaded at 778A2000 - Success
$ App.efi
##### Get smbios structure with processor data #####
##### Display processor information #####
Processor voltage information:
Current voltage = (9/10)V (hex - 0x89)
ExternalClock: 100
MaxSpeed: 2300
CurrentSpeed: 2300
##### Display processor voltage #####
Voltage in hex: 0x89
##### Save processor information into variable #####
Voltage after GetVariable in hex: 0x89
```

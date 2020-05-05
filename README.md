# ProcessorInfoInOSEDK2

This project allows you to get the current processor voltage using the SMBIOS_TABLE structure and transfer it to a variable using SetVariable in runtime environment. In the future, this variable can be used in the operating system.

## Dependencies

EDK II

## Quick start

- Build:

  ```bash
  build -p ProcessorInfoInOSEDK2/TestPkg.dsc
  ```

  Or use `make build`

- Run (uefi shell):

  - Copy your .efi files to VM or USB with EFI shell and start binaries with commands:

  ```bash
  load ProcessorView.efi
  App.efi
  ```

  - If you have qemu, you can use `make start`

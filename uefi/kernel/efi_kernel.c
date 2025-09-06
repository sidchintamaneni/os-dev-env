#include <efi.h>
#include <efilib.h>

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    /* Initialize the EFI library */
    InitializeLib(ImageHandle, SystemTable);
 
    /* Print a welcome message */
    Print(L"Hello, World! This is a minimal EFI kernel.\n");
    /* Get memory map (important for OS development) */
    UINTN MemoryMapSize = 0;
    EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
    /* First get the size of the memory map */
    EFI_STATUS Status = uefi_call_wrapper(SystemTable->BootServices->GetMemoryMap, 5,
							&MemoryMapSize, MemoryMap, &MapKey,
							&DescriptorSize, &DescriptorVersion);
    /* Add some buffer space for potential changes */
    MemoryMapSize += 2 * DescriptorSize;
    /* Allocate memory for the map */
    Status = uefi_call_wrapper(SystemTable->BootServices->AllocatePool, 3,
                              EfiLoaderData, MemoryMapSize, (void**)&MemoryMap);
    if (EFI_ERROR(Status)) {
        Print(L"Error allocating memory for memory map\n");
        return Status;
    }
    /* Get the actual memory map */
    Status = uefi_call_wrapper(SystemTable->BootServices->GetMemoryMap, 5,
				&MemoryMapSize, MemoryMap, &MapKey,
				&DescriptorSize, &DescriptorVersion);
    if (EFI_ERROR(Status)) {
        Print(L"Error getting memory map: %r\n", Status);
        return Status;
    }
    Print(L"Memory map acquired successfully\n");
    /* Display basic system information */
    Print(L"Firmware Vendor: %s\n", SystemTable->FirmwareVendor);
    Print(L"Firmware Revision: %d\n", SystemTable->FirmwareRevision);
    /* Wait for a key press before exiting */
    Print(L"Press any key to exit...\n");
    EFI_INPUT_KEY Key;
    while (uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key) == EFI_NOT_READY)
        uefi_call_wrapper(SystemTable->BootServices->Stall, 1, 100000);  /* 0.1 second */
    /* Free the memory map */
    uefi_call_wrapper(SystemTable->BootServices->FreePool, 1, MemoryMap);

    return EFI_SUCCESS;
}

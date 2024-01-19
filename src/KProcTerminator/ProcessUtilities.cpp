#include "ProcessUtilities.hpp"

NTSTATUS FindProcessId(const WCHAR* processName, PHANDLE ProcessId) {
    NTSTATUS status;
    ULONG neededSize;
    PSYSTEM_PROCESS_INFORMATION processInfo;

    status = ZwQuerySystemInformation(SystemProcessInformation, NULL, 0, &neededSize);
    if (!NT_SUCCESS(status) && status != STATUS_INFO_LENGTH_MISMATCH) {
        return status;
    }

    processInfo = (PSYSTEM_PROCESS_INFORMATION)ExAllocatePool2(NonPagedPool, neededSize, 'Proc');
    if (!processInfo) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    status = ZwQuerySystemInformation(SystemProcessInformation, processInfo, neededSize, &neededSize);
    if (!NT_SUCCESS(status)) {
        ExFreePoolWithTag(processInfo, 'Proc');
        return status;
    }

    PSYSTEM_PROCESS_INFORMATION currentEntry = processInfo;
    while (TRUE) {
        if (currentEntry->ImageName.Buffer != NULL) {
            if (wcscmp(processName, currentEntry->ImageName.Buffer) == 0) {
                *ProcessId = currentEntry->UniqueProcessId;
                break;
            }
        }

        if (currentEntry->NextEntryOffset == 0) {
            status = STATUS_NOT_FOUND;
            break;
        }

        currentEntry = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)currentEntry + currentEntry->NextEntryOffset);
    }

    ExFreePoolWithTag(processInfo, 'Proc');
    return status;
}

NTSTATUS TerminateProcess(HANDLE ProcessId) {
    NTSTATUS status;
    PEPROCESS Process;

    // ---
    status = PsLookupProcessByProcessId(ProcessId, &Process);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    HANDLE ProcessHandle;

    // Open handle
    status = ObOpenObjectByPointer(Process, OBJ_KERNEL_HANDLE, NULL, PROCESS_TERMINATE, *PsProcessType, KernelMode, &ProcessHandle);
    if (!NT_SUCCESS(status)) {
        ObDereferenceObject(Process);
        return status;
    }

    // Terminate
    status = ZwTerminateProcess(ProcessHandle, 0);
    ZwClose(ProcessHandle);
    ObDereferenceObject(Process);

    return status;
}

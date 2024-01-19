#ifndef PROCESS_UTILITIES_HPP
#define PROCESS_UTILITIES_HPP

#include "SystemProcessInformation.hpp"

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

    NTSTATUS ZwQuerySystemInformation(
        ULONG InfoClass,
        PVOID Buffer,
        ULONG Length,
        PULONG ReturnLength
    );
}

NTSTATUS FindProcessId(const WCHAR* processName, PHANDLE ProcessId);
NTSTATUS TerminateProcess(HANDLE ProcessId);

#endif // PROCESS_UTILITIES_HPP

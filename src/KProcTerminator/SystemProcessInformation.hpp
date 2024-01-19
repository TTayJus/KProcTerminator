#ifndef SYSTEM_PROCESS_INFORMATION_HPP
#define SYSTEM_PROCESS_INFORMATION_HPP

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

#endif // SYSTEM_PROCESS_INFORMATION_HPP

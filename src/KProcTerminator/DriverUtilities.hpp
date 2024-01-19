#ifndef DRIVER_UTILITIES_HPP
#define DRIVER_UTILITIES_HPP

#include "ProcessUtilities.hpp"

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath);

#endif // DRIVER_UTILITIES_HPP

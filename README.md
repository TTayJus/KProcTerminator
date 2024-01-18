# KProcTerminator

## Overview

This repository, 'KProcTerminator', contains a Windows kernel driver designed for terminating specific processes on a system. It is meant to be loaded using `kdmapper`, a known tool for loading unsigned drivers into the Windows kernel space without a signed driver.


![KProcTerminator Diagram](https://raw.githubusercontent.com/TTayJus/KProcTerminator/main/src/ErwYHijSYPUSeyMHd3B3Ye.png)


## Warning

⚠️ **This driver can terminate system processes, potentially leading to system instability or crashes.**

## Features

- Terminates specified processes by name.
- Continuously monitors and terminates target processes.
- Leverages native Windows Kernel APIs for process management.

## Prerequisites

- 64-bit Windows environment.
- Administrative privileges on the system.
- Knowledge of Windows kernel mode operations.
- `kdmapper` or similar tools for loading unsigned kernel drivers.

## Compilation

Compile the driver using Microsoft Visual Studio with Windows Driver Kit (WDK). Create a MVS project (WKMD) and paste the code in.

## Usage

1. **Driver Loading**: Load the driver into the kernel using `kdmapper` or a similar tool, with administrative privileges.
2. **Operation**: Post-loading, it will monitor and terminate the following processes:
    - `EPProtectedService.exe`
    - `EPIntegrationService.exe`
    - `EPSecurityService.exe`
3. **Monitoring**: The driver operates silently. Check system logs for its activities.
4. **Unloading**: The driver does not support unloading; reboot the system to remove it from operation.

## Customization

Modify the driver to target different processes by altering the `DriverEntry` function. Re-compile post-modifications.


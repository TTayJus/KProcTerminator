/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_KProcTerminator,
    0x7ec30ad0,0xc870,0x4110,0x98,0xef,0xba,0x99,0xc7,0x6d,0x09,0xc4);
// {7ec30ad0-c870-4110-98ef-ba99c76d09c4}

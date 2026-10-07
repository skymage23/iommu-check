#include <windows.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <initguid.h>
#include <devpkey.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "cfgmgr32.lib")

void CheckDeviceDmaIsolation() {
    // 1. Get device information set for the target class
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(NULL, NULL, NULL, (DIGCF_PRESENT | DIGCF_ALLCLASSES));
    if (deviceInfoSet == INVALID_HANDLE_VALUE) return;

    SP_DEVINFO_DATA devInfoData;
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &devInfoData); i++) {
        DEVPROPTYPE propType;
        ULONG bufferSize = 0;

        // Query the ACPI/Location path to see structural physical routing
        // This helps identify devices sharing a common upstream hub/switch
        CM_Get_DevNode_PropertyW(devInfoData.DevInst, &DEVPKEY_Device_LocationPaths, 
                                &propType, NULL, &bufferSize, 0);

        if (bufferSize > 0) {
            std::wstring buffer(bufferSize / sizeof(wchar_t), L'\0');
            if (CM_Get_DevNode_PropertyW(devInfoData.DevInst, &DEVPKEY_Device_LocationPaths, 
                &propType, reinterpret_cast<PBYTE>(&buffer[0]), &bufferSize, 0) == CR_SUCCESS) {
                std::wcout << L"Device Path (Topology): " << buffer << std::endl;
            }
        }
    }
    SetupDiDestroyDeviceInfoList(deviceInfoSet);
}

int main (int argc, char** argv){
    //First, we need to list all PCIe devices.

    CheckDeviceDmaIsolation();
    return 0;
}
# enumEDR

Windows EDR/AV enumeration tool written in C. Lists security processes and kernel drivers running on the system.

## Usage

```
enumEDR.exe <Option>

Options:
    -h       Show help
    -p       List EDR/AV processes
    -d       List EDR/AV loaded drivers
    -a       List both processes and drivers
```

## Example

```
C:\> enumEDR.exe -a

[+] Security Processes:

    MsSense.exe                                   PID: 1234      [Microsoft Defender for Endpoint]
    smartscreen.exe                               PID: 5678      [Windows SmartScreen]

    [i] Total: 2 security process(es) found

[+] Security Drivers:

    WdFilter.sys                        Base: 0xFFFFF80200000000  [Microsoft Defender]

    [i] Total: 1 security driver(s) found
```

## Supported EDR/AV Vendors

CrowdStrike, SentinelOne, Microsoft Defender for Endpoint, Sophos, WithSecure, ESET, Bitdefender, Kaspersky, Cortex XDR, ArfangLab, Carbon Black, Cylance, Symantec, McAfee/Trellix, Trend Micro, Elastic, Cybereason, Malwarebytes, Fortinet FortiEDR, Avast/AVG, Sysmon, Wazuh, Rapid7, Cisco AMP, Check Point, Qualys, Tanium.

## Build

Requires MSVC (Visual Studio):

```
cl /O2 /W4 /Fe:enumEDR.exe enumEDR.c psapi.lib
```

Or with MinGW:

```
x86_64-w64-mingw32-gcc -Wall -Wextra -O2 -o enumEDR.exe enumEDR.c -lpsapi
```

## Disclaimer

**This tool is provided for authorized security testing, educational purposes, and defensive security assessments only.**

Usage of this tool for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state, and federal laws. The developer assumes no liability and is not responsible for any misuse or damage caused by this tool.

Only use this tool on systems you own or have explicit written authorization to test.

## License

MIT License - see [LICENSE](LICENSE)

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char *name;
    const char *vendor;
} Signature;

static const Signature edr_processes[] = {
    // CrowdStrike
    {"csfalconservice.exe", "CrowdStrike"},
    {"csfalconcontainer.exe", "CrowdStrike"},
    {"csagent.exe", "CrowdStrike"},
    {"csdevicecontrol.exe", "CrowdStrike"},
    {"csinstallerservice.exe", "CrowdStrike"},
    // SentinelOne
    {"sentinelagent.exe", "SentinelOne"},
    {"sentinelctl.exe", "SentinelOne"},
    {"sentinelservicehost.exe", "SentinelOne"},
    {"sentinelstaticengine.exe", "SentinelOne"},
    {"sentinelstaticenginescanner.exe", "SentinelOne"},
    {"sentinelhelperservice.exe", "SentinelOne"},
    {"sentinelupdaterservice.exe", "SentinelOne"},
    {"logprocessorservice.exe", "SentinelOne"},
    // Carbon Black
    {"cb.exe", "Carbon Black"},
    {"cbcomms.exe", "Carbon Black"},
    {"cbstream.exe", "Carbon Black"},
    {"cbsystemanalysis.exe", "Carbon Black"},
    {"reptile.exe", "Carbon Black"},
    {"repux.exe", "Carbon Black"},
    {"repwsc.exe", "Carbon Black"},
    {"repmgr.exe", "Carbon Black"},
    {"cbdefense.exe", "Carbon Black"},
    {"cbdaemon.exe", "Carbon Black"},
    // Cylance
    {"cylancesvc.exe", "Cylance"},
    {"cylanceui.exe", "Cylance"},
    {"cylanceprotectsetup.exe", "Cylance"},
    // Symantec / Broadcom
    {"ccsvchst.exe", "Symantec"},
    {"rtvscan.exe", "Symantec"},
    {"smcgui.exe", "Symantec"},
    {"smc.exe", "Symantec"},
    {"snac.exe", "Symantec"},
    {"sepwscsvc64.exe", "Symantec"},
    {"sesmclient.exe", "Symantec"},
    {"symcorpui.exe", "Symantec"},
    // McAfee / Trellix
    {"mfemactl.exe", "McAfee"},
    {"mfetp.exe", "McAfee"},
    {"mcshield.exe", "McAfee"},
    {"masvc.exe", "McAfee"},
    {"mfefire.exe", "McAfee"},
    {"mfevtps.exe", "McAfee"},
    {"mfecanary.exe", "McAfee"},
    {"mfeesp.exe", "McAfee"},
    {"xagt.exe", "Trellix/FireEye"},
    {"firetray.exe", "Trellix/FireEye"},
    // Sophos
    {"sophosui.exe", "Sophos"},
    {"sophoshealth.exe", "Sophos"},
    {"sophosinterceptx.exe", "Sophos"},
    {"sophosfilescanner.exe", "Sophos"},
    {"sophoscleanm.exe", "Sophos"},
    {"sophosfs.exe", "Sophos"},
    {"savservice.exe", "Sophos"},
    {"swi_service.exe", "Sophos"},
    {"swc_service.exe", "Sophos"},
    {"hmpalert.exe", "Sophos"},
    {"sophossps.exe", "Sophos"},
    {"sophosntpservice.exe", "Sophos"},
    {"sophoscommsservice.exe", "Sophos"},
    // Kaspersky
    {"avp.exe", "Kaspersky"},
    {"avpui.exe", "Kaspersky"},
    {"kavtray.exe", "Kaspersky"},
    {"kavfs.exe", "Kaspersky"},
    {"klnagent.exe", "Kaspersky"},
    // Trend Micro
    {"pccntmon.exe", "Trend Micro"},
    {"tmbmsrv.exe", "Trend Micro"},
    {"ntrtscan.exe", "Trend Micro"},
    {"tmccsf.exe", "Trend Micro"},
    {"tmlisten.exe", "Trend Micro"},
    {"tmssclient.exe", "Trend Micro"},
    {"coreserviceshell.exe", "Trend Micro"},
    {"ds_agent.exe", "Trend Micro"},
    {"endpointbasecamp.exe", "Trend Micro"},
    // ESET
    {"egui.exe", "ESET"},
    {"ekrn.exe", "ESET"},
    {"eraagent.exe", "ESET"},
    {"eguiproxy.exe", "ESET"},
    {"epcfw.exe", "ESET"},
    {"epfw.exe", "ESET"},
    {"epintcli.exe", "ESET"},
    {"eset_rtp.exe", "ESET"},
    {"esetonlinescanner.exe", "ESET"},
    {"esetservice.exe", "ESET"},
    {"ehttpd.exe", "ESET"},
    {"ecmd.exe", "ESET"},
    {"ecls.exe", "ESET"},
    {"escanw64.exe", "ESET"},
    // Cortex XDR / Palo Alto
    {"traps.exe", "Cortex XDR"},
    {"trapsd.exe", "Cortex XDR"},
    {"cyserver.exe", "Cortex XDR"},
    {"cyveraservice.exe", "Cortex XDR"},
    {"cytray.exe", "Cortex XDR"},
    {"cortex xdr.exe", "Cortex XDR"},
    {"cabortsvc.exe", "Cortex XDR"},
    {"cyupdate.exe", "Cortex XDR"},
    {"cyvrmtgn.exe", "Cortex XDR"},
    {"trapsagent.exe", "Cortex XDR"},
    // Microsoft Defender for Endpoint (EDR)
    {"mssense.exe", "Microsoft Defender for Endpoint"},
    {"sensecncproxy.exe", "Microsoft Defender for Endpoint"},
    {"monitoringhost.exe", "Microsoft Defender for Endpoint"},
    {"mdecopacker.exe", "Microsoft Defender for Endpoint"},
    // Microsoft Defender Antivirus
    {"msmpeng.exe", "Microsoft Defender AV"},
    {"mpcmdrun.exe", "Microsoft Defender AV"},
    {"nissrv.exe", "Microsoft Defender AV"},
    // Windows Security
    {"securityhealthservice.exe", "Windows Security"},
    {"securityhealthsystray.exe", "Windows Security"},
    {"smartscreen.exe", "Windows SmartScreen"},
    // Elastic
    {"elastic-agent.exe", "Elastic"},
    {"elastic-endpoint.exe", "Elastic"},
    {"winlogbeat.exe", "Elastic"},
    {"filebeat.exe", "Elastic"},
    // Qualys
    {"qualysagent.exe", "Qualys"},
    // Tanium
    {"taniumclient.exe", "Tanium"},
    {"taniumdetectengine.exe", "Tanium"},
    // Cybereason
    {"activeconsole.exe", "Cybereason"},
    {"cyabortsvc.exe", "Cybereason"},
    {"crssvc.exe", "Cybereason"},
    {"cyoptics.exe", "Cybereason"},
    // Malwarebytes
    {"mbamservice.exe", "Malwarebytes"},
    {"mbamtray.exe", "Malwarebytes"},
    {"mbam.exe", "Malwarebytes"},
    // Bitdefender
    {"bdagent.exe", "Bitdefender"},
    {"bdservicehost.exe", "Bitdefender"},
    {"bdredline.exe", "Bitdefender"},
    {"epag.exe", "Bitdefender"},
    {"epintegrationservice.exe", "Bitdefender"},
    {"epsecurityservice.exe", "Bitdefender"},
    // WithSecure (ex F-Secure)
    {"fsgk32.exe", "WithSecure"},
    {"fsma32.exe", "WithSecure"},
    {"fssm32.exe", "WithSecure"},
    {"fnrb32.exe", "WithSecure"},
    {"fsaua.exe", "WithSecure"},
    {"fsorsp.exe", "WithSecure"},
    {"fsav32.exe", "WithSecure"},
    {"withsecure.exe", "WithSecure"},
    {"fshoster.exe", "WithSecure"},
    {"fsdevcon64.exe", "WithSecure"},
    {"fsgk32st.exe", "WithSecure"},
    {"fsmach64.exe", "WithSecure"},
    {"daas2.exe", "WithSecure"},
    {"epson.exe", "WithSecure"},
    {"wscepmainservice.exe", "WithSecure Elements"},
    {"wsceeprconnector.exe", "WithSecure Elements"},
    // ArfangLab
    {"arfangagent.exe", "ArfangLab"},
    {"arfangsvc.exe", "ArfangLab"},
    {"arfanglab.exe", "ArfangLab"},
    {"arfangcollector.exe", "ArfangLab"},
    {"arfangendpoint.exe", "ArfangLab"},
    // Fortinet FortiEDR
    {"fortiedr.exe", "Fortinet"},
    {"forticollector.exe", "Fortinet"},
    // Avast / AVG
    {"avastsvc.exe", "Avast"},
    {"avastui.exe", "Avast"},
    {"avgnt.exe", "AVG"},
    {"avgsvc.exe", "AVG"},
    // Windows built-in
    {"configsecuritypolicy.exe", "Windows Security"},
    // Sysmon
    {"sysmon.exe", "Sysmon"},
    {"sysmon64.exe", "Sysmon"},
    // Wazuh
    {"ossec-agent.exe", "Wazuh"},
    {"wazuh-agent.exe", "Wazuh"},
    // Rapid7
    {"ir_agent.exe", "Rapid7"},
    // Cisco
    {"amp.exe", "Cisco AMP"},
    {"sfc.exe", "Cisco AMP"},
    {"orbital.exe", "Cisco Orbital"},
    // Check Point
    {"cpda.exe", "Check Point"},
    {"cptray.exe", "Check Point"},
    {"tracsvr.exe", "Check Point"},
    {NULL, NULL}
};

static const Signature edr_drivers[] = {
    // CrowdStrike
    {"csagent.sys", "CrowdStrike"},
    {"csboot.sys", "CrowdStrike"},
    {"csdevicecontrol.sys", "CrowdStrike"},
    {"cspcm.sys", "CrowdStrike"},
    // SentinelOne
    {"sentinelmonitor.sys", "SentinelOne"},
    {"sentinelelam.sys", "SentinelOne"},
    // Carbon Black
    {"cbk7.sys", "Carbon Black"},
    {"cbstream.sys", "Carbon Black"},
    {"ctifile.sys", "Carbon Black"},
    // Cylance
    {"cyoptics.sys", "Cylance"},
    {"cyprotectdrv32.sys", "Cylance"},
    {"cyprotectdrv64.sys", "Cylance"},
    // Symantec
    {"symefasi.sys", "Symantec"},
    {"symevent.sys", "Symantec"},
    {"symevnt.sys", "Symantec"},
    {"srtsp.sys", "Symantec"},
    {"srtspx.sys", "Symantec"},
    {"bhdrvx64.sys", "Symantec"},
    {"bhdrvx86.sys", "Symantec"},
    {"sisipsfilefilter.sys", "Symantec"},
    // McAfee / Trellix
    {"mfefirek.sys", "McAfee"},
    {"mfencfilter.sys", "McAfee"},
    {"mfehidk.sys", "McAfee"},
    {"mfeavfk.sys", "McAfee"},
    {"mfencoak.sys", "McAfee"},
    {"mfewfpk.sys", "McAfee"},
    {"mfetdik.sys", "McAfee"},
    // Sophos
    {"savonaccess.sys", "Sophos"},
    {"sophosed.sys", "Sophos"},
    {"hmpalert.sys", "Sophos"},
    {"sophos_detoured.sys", "Sophos"},
    {"sophos_detoured_x64.sys", "Sophos"},
    // Kaspersky
    {"klif.sys", "Kaspersky"},
    {"klflt.sys", "Kaspersky"},
    {"klam.sys", "Kaspersky"},
    {"klbg.sys", "Kaspersky"},
    {"klhk.sys", "Kaspersky"},
    {"klkbd.sys", "Kaspersky"},
    {"kltdi.sys", "Kaspersky"},
    {"kneps.sys", "Kaspersky"},
    {"klwtp.sys", "Kaspersky"},
    // Trend Micro
    {"tmcomm.sys", "Trend Micro"},
    {"tmactmon.sys", "Trend Micro"},
    {"tmevtmgr.sys", "Trend Micro"},
    {"tmeext.sys", "Trend Micro"},
    {"tmel.sys", "Trend Micro"},
    {"tmtdi.sys", "Trend Micro"},
    {"tmnciesc.sys", "Trend Micro"},
    {"tmums.sys", "Trend Micro"},
    {"tmusa.sys", "Trend Micro"},
    // ESET
    {"eamonm.sys", "ESET"},
    {"ehdrv.sys", "ESET"},
    {"ekbdflt.sys", "ESET"},
    {"epfwwfp.sys", "ESET"},
    {"epfw.sys", "ESET"},
    {"edevmon.sys", "ESET"},
    {"eelam.sys", "ESET"},
    {"eapacket.sys", "ESET"},
    {"epp.sys", "ESET"},
    // Cortex XDR / Palo Alto
    {"tdevflt.sys", "Cortex XDR"},
    {"cyvrfsfd.sys", "Cortex XDR"},
    {"cytena.sys", "Cortex XDR"},
    {"cyverak.sys", "Cortex XDR"},
    {"trapsdrv.sys", "Cortex XDR"},
    {"cyelfam.sys", "Cortex XDR"},
    // Microsoft Defender for Endpoint (EDR)
    {"wdfilter.sys", "Microsoft Defender"},
    {"wdnisdrv.sys", "Microsoft Defender"},
    {"wdboot.sys", "Microsoft Defender"},
    {"mssecflt.sys", "Microsoft Defender for Endpoint"},
    {"sense.sys", "Microsoft Defender for Endpoint"},
    // Elastic
    {"elasticendpoint.sys", "Elastic"},
    // Malwarebytes
    {"mbamswissarmy.sys", "Malwarebytes"},
    {"mwac.sys", "Malwarebytes"},
    {"farflt.sys", "Malwarebytes"},
    {"mbamchameleon.sys", "Malwarebytes"},
    // Bitdefender
    {"bdselfpr.sys", "Bitdefender"},
    {"bdfndisf6.sys", "Bitdefender"},
    {"atc.sys", "Bitdefender"},
    {"avc3.sys", "Bitdefender"},
    {"avckf.sys", "Bitdefender"},
    {"edrsensor.sys", "Bitdefender"},
    {"trufos.sys", "Bitdefender"},
    {"gemma.sys", "Bitdefender"},
    // WithSecure (ex F-Secure)
    {"fsdfw.sys", "WithSecure"},
    {"fshs.sys", "WithSecure"},
    {"fsvista.sys", "WithSecure"},
    {"fses.sys", "WithSecure"},
    {"fsulgk.sys", "WithSecure"},
    {"fsatp.sys", "WithSecure"},
    {"fsni.sys", "WithSecure"},
    {"fsnk.sys", "WithSecure"},
    {"wscepedrdrv.sys", "WithSecure Elements"},
    // ArfangLab
    {"arfangflt.sys", "ArfangLab"},
    {"arfangdrv.sys", "ArfangLab"},
    // Fortinet
    {"fortishield.sys", "Fortinet"},
    // Avast / AVG
    {"aswsp.sys", "Avast"},
    {"aswbuniv.sys", "Avast"},
    {"aswarvrt.sys", "Avast"},
    {"avgntflt.sys", "AVG"},
    {"avgtpx64.sys", "AVG"},
    {"avgtpx86.sys", "AVG"},
    // Sysmon
    {"sysmondrv.sys", "Sysmon"},
    // Cybereason
    {"psychoctopusminifilter.sys", "Cybereason"},
    {"psychoctopuselam.sys", "Cybereason"},
    {"crexecprev.sys", "Cybereason"},
    // Cisco
    {"immunetprotect.sys", "Cisco AMP"},
    {"immunetselfprotect.sys", "Cisco AMP"},
    // Check Point
    {"epklib.sys", "Check Point"},
    {"cpminifilter.sys", "Check Point"},
    {NULL, NULL}
};

static void print_banner(void) {
    printf("\n");
    printf("  _____ _   _ _   _ __  __ _____ ____  ____  \n");
    printf(" | ____| \\ | | | | |  \\/  | ____|  _ \\|  _ \\ \n");
    printf(" |  _| |  \\| | | | | |\\/| |  _| | | | | |_) |\n");
    printf(" | |___| |\\  | |_| | |  | | |___| |_| |  _ < \n");
    printf(" |_____|_| \\_|\\___/|_|  |_|_____|____/|_| \\_\\\n");
    printf("                                    v1.0\n\n");
}

static void print_usage(const char *prog) {
    printf("[*] Usage:\n");
    printf("        %s <Option>\n\n", prog);
    printf("    [*] Options\n\n");
    printf("        -h       Show this help\n");
    printf("        -p       List EDR/AV processes\n");
    printf("        -d       List EDR/AV loaded drivers\n");
    printf("        -a       List both processes and drivers\n\n");
}

static void to_lower(char *dst, const char *src, size_t len) {
    size_t i;
    for (i = 0; i < len - 1 && src[i]; i++)
        dst[i] = (src[i] >= 'A' && src[i] <= 'Z') ? src[i] + 32 : src[i];
    dst[i] = '\0';
}

static int enum_processes(void) {
    HANDLE snap;
    PROCESSENTRY32 pe;
    int count = 0;
    char lower_name[MAX_PATH];

    snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        printf("[-] Failed to snapshot processes (error %lu)\n", GetLastError());
        return -1;
    }

    pe.dwSize = sizeof(pe);
    if (!Process32First(snap, &pe)) {
        CloseHandle(snap);
        printf("[-] Failed to enumerate processes\n");
        return -1;
    }

    printf("[+] Security Processes:\n\n");

    do {
        to_lower(lower_name, pe.szExeFile, MAX_PATH);
        for (const Signature *sig = edr_processes; sig->name; sig++) {
            if (strcmp(lower_name, sig->name) == 0) {
                printf("    %-45s PID: %-8lu  [%s]\n",
                       pe.szExeFile, pe.th32ProcessID, sig->vendor);
                count++;
                break;
            }
        }
    } while (Process32Next(snap, &pe));

    CloseHandle(snap);

    if (count == 0)
        printf("    (none detected)\n");

    printf("\n    [i] Total: %d security process(es) found\n\n", count);
    return count;
}

static int enum_drivers(void) {
    LPVOID drivers[1024];
    DWORD needed;
    int count = 0;
    char basename[MAX_PATH];
    char lower_name[MAX_PATH];

    if (!EnumDeviceDrivers(drivers, sizeof(drivers), &needed)) {
        printf("[-] Failed to enumerate drivers (error %lu)\n", GetLastError());
        return -1;
    }

    DWORD num_drivers = needed / sizeof(LPVOID);

    printf("[+] Security Drivers:\n\n");

    for (DWORD i = 0; i < num_drivers; i++) {
        if (GetDeviceDriverBaseNameA(drivers[i], basename, sizeof(basename))) {
            to_lower(lower_name, basename, MAX_PATH);
            for (const Signature *sig = edr_drivers; sig->name; sig++) {
                if (strcmp(lower_name, sig->name) == 0) {
                    printf("    %-35s Base: 0x%p  [%s]\n",
                           basename, drivers[i], sig->vendor);
                    count++;
                    break;
                }
            }
        }
    }

    if (count == 0)
        printf("    (none detected)\n");

    printf("\n    [i] Total: %d security driver(s) found\n\n", count);
    return count;
}

int main(int argc, char *argv[]) {
    int do_procs = 0, do_drivers = 0;

    print_banner();

    if (argc < 2) {
        print_usage(argv[0]);
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-p") == 0) {
            do_procs = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            do_drivers = 1;
        } else if (strcmp(argv[i], "-a") == 0) {
            do_procs = 1;
            do_drivers = 1;
        } else {
            printf("[-] Unknown option: %s\n\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (do_procs)
        enum_processes();
    if (do_drivers)
        enum_drivers();

    return 0;
}

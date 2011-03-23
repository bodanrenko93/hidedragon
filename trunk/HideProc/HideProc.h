//驱动内部名称和符号名称
#define DEVICE_NAME L"\\Device\\devHideProc"
#define LINK_NAME L"\\??\\slHideProc"



#define DWORD unsigned long
#define WORD unsigned short
#define BOOL unsigned long
typedef PVOID           POBJECT;

//定义服务描述表
typedef struct ServiceDescriptorEntry {
    unsigned int *ServiceTableBase;
    unsigned int *ServiceCounterTableBase; 
    unsigned int NumberOfServices;
    unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry, *PServiceDescriptorTableEntry;

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
    IN ULONG SystemInformationClass,
    IN OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength);


//驱动入口
NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT		pDrvObj,
	IN	PUNICODE_STRING		pRegistryPath
);

//处理Win32的Create,Close
NTSTATUS
DispatchCreateClose(IN	PDEVICE_OBJECT pDevObj,
					IN	PIRP pIrp);

//驱动卸载
VOID 
DriverUnload(
	IN PDRIVER_OBJECT		pDrvObj
);

//??IOCTL
NTSTATUS
DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);
//
//ZwQuerySystemInformation的枚举参数
//
typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemBasicInformation,
    SystemProcessorInformation,
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemNotImplemented1,
    SystemProcessesAndThreadsInformation,
    SystemCallCounts,
    SystemConfigurationInformation,
    SystemProcessorTimes,
    SystemGlobalFlag,
    SystemNotImplemented2,
    SystemModuleInformation,
    SystemLockInformation,
    SystemNotImplemented3,
    SystemNotImplemented4,
    SystemNotImplemented5,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPagefileInformation,
    SystemInstructionEmulationCounts,
    SystemInvalidInfoClass1,
    SystemCacheInformation,
    SystemPoolTagInformation,
    SystemProcessorStatistics,
    SystemDpcInformation,
    SystemNotImplemented6,
    SystemLoadImage,
    SystemUnloadImage,
    SystemTimeAdjustment,
    SystemNotImplemented7,
    SystemNotImplemented8,
    SystemNotImplemented9,
    SystemCrashDumpInformation,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemLoadAndCallImage,
    SystemPrioritySeparation,
    SystemNotImplemented10,
    SystemNotImplemented11,
    SystemInvalidInfoClass2,
    SystemInvalidInfoClass3,
    SystemTimeZoneInformation,
    SystemLookasideInformation,
    SystemSetTimeSlipEvent,
    SystemCreateSession,
    SystemDeleteSession,
    SystemInvalidInfoClass4,
    SystemRangeStartInformation,
    SystemVerifierInformation,
    SystemAddVerifier,
    SystemSessionProcessesInformation
} SYSTEM_INFORMATION_CLASS;

typedef NTSTATUS (*ZWQUERYSYSTEMINFORMATION)(ULONG, PVOID, ULONG, PULONG);
ZWQUERYSYSTEMINFORMATION OldZwQuerySystemInformation = NULL;

NTSTATUS
HookZwQuerySystemInformation(
							  IN SYSTEM_INFORMATION_CLASS sysInfoClass,
							  IN OUT PVOID sysInfo,
							  IN ULONG sysInfoLen,
							  OUT PULONG RetLen OPTIONAL);

#pragma warning(disable:4200)
typedef struct _SYSTEM_THREADS {
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER CreateTime;
    ULONG WaitTime;
    PVOID StartAddress;
    CLIENT_ID ClientId;
    KPRIORITY Priority;
    KPRIORITY BasePriority;
    ULONG ContextSwitchCount;
    ULONG State;
    KWAIT_REASON WaitReason;
} SYSTEM_THREADS, *PSYSTEM_THREADS;

typedef struct _SYSTEM_PROCESSES { // Information Class 5
    ULONG NextEntryDelta;
    ULONG ThreadCount;
    ULONG Reserved1[6];
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ProcessName;
    KPRIORITY BasePriority;
    ULONG ProcessId;
    ULONG InheritedFromProcessId;
    ULONG HandleCount;
    ULONG Reserved2[2];
    VM_COUNTERS VmCounters;
    ULONG IoCounters;  // Windows 2000 only
    SYSTEM_THREADS Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;
#pragma warning( default:4200 )

void HookOn();

void HookOff();

BOOLEAN isHidePid(ULONG uPid);


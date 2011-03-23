//////////////////////////////////////////////////////////////////////////
//ioctl.h --该文件只存放IOCTL代码，可复用于win32工程
#ifndef IOCTL_H_
#define IOCTL_H_



#define IOCTL_HIDEPID	\
CTL_CODE(FILE_DEVICE_UNKNOWN, 0x830, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SHOWPID	\
CTL_CODE(FILE_DEVICE_UNKNOWN, 0x831, METHOD_BUFFERED, FILE_ANY_ACCESS)


#endif


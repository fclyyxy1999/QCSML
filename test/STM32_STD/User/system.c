/**
 * 最小化实现STM32F4嵌入式系统调用函数
 * 1. I/O及文件系统访问
 * 2. 内存管理
 * 3. 任务管理
 */

#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/types.h>
#include "system.h"


/**
 * 文件系统访问
 */

int _close(int file) {
    // No-op implementation
    return -1;
}

int _fstat(int file, struct stat *st) {
    // No-op implementation
    st->st_mode = S_IFCHR;
    return 0;
}

int _lseek(int file, int ptr, int dir) {
    // No-op implementation
    return 0;
}

int _isatty(int file) {
    // No-op implementation
    return 1;
}

/**
 * 任务管理
 */

void _exit(int status) {
    // No-op implementation
    while (1);
}

int _kill(int pid, int sig) {
    // No-op implementation
    errno = EINVAL;
    return -1;
}

int _getpid(void) {
    // No-op implementation
    return 1;
}

/**
 * 内存管理
 */

extern char _end; /* Defined by the linker */
extern char _estack; /* Defined by the linker */
extern int _Min_Stack_Size; /* Defined by the linker */

void *_sbrk(int incr) {
    static char *heap_end = 0;
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_end;
    }
    prev_heap_end = heap_end;
    if (heap_end + incr > (char *)&_estack - _Min_Stack_Size) {
        errno = ENOMEM;
        return (void *)-1;
    }
    heap_end += incr;
    return (void *)prev_heap_end;
}

/**
 * I/O
 */


int _write(int fd, char *pBuffer, int len)
{
    for (int i = 0; i < len; i++)
    {
        while ((USART2->SR & USART_SR_TXE) == 0); // Wait for transmit buffer empty
        USART2->DR = (uint8_t) *(pBuffer + i); // Write data to DR
        while ((USART2->SR & USART_SR_TC) == 0); // Wait for transmit complete
    }
    return len;
}

int _read(int file, char *ptr, int len) {
    // No-op implementation

    return len;
}
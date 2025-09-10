#include "my_debug.h"

void RegularPrint(char* str, int n,double running_time) {
    if(strcmp(str,"sm4_enc") == 0 || strcmp(str, "sm4_dec") == 0) {
        printf("%-16s%-12d%-12.3f%.3f Mbps\r\n", str, n, running_time, (double) (n << 7) / 1000000 / running_time);
    }
    else if(strcmp(str,"sm3") == 0) {
        printf("%-16s%-12d%-12.3f%.3f Mbps\r\n", "sm3", n, running_time, (double) (n << 9) / (running_time * 1000000));
    }
    else
        printf("%-16s%-12d%-12.3f%.3f ops/s\r\n", str, n, running_time, n / running_time);
}

void JsonPrint(char* str, int n,double running_time) {
    if(strcmp(str,"sm4_enc") == 0 || strcmp(str, "sm4_dec") == 0) {
        printf("{ \"name\": \"%s\", \"rate\": %.3f, \"total_tests\": %d, \"duration\": %.3f } \r\n", str, (double) (n << 7) / 1000000 / running_time, n, running_time);
    }
    else if(strcmp(str,"sm3") == 0) {
        printf("{ \"name\": \"%s\", \"rate\": %.3f, \"total_tests\": %d, \"duration\": %.3f } \r\n", str, (double) (n << 9) / (running_time * 1000000), n, running_time);
    }
    else 
        printf("{ \"name\": \"%s\", \"rate\": %.3f, \"total_tests\": %d, \"duration\": %.3f } \r\n", str, n / running_time, n, running_time);
}
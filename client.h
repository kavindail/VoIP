#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#ifdef __cplusplus

extern "C" {
#endif

void sendDataToSocket(const char *inputBuffer, size_t dataSize);

#ifdef __cplusplus
}
#endif

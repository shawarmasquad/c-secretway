#ifndef SW_GLOBAL__
#define SW_GLOBAL__

typedef struct {
	char    ip[INET_ADDRSTRLEN];
	int32_t port;

	char    shortDesc[100];
} Server;

#endif // SW_GLOBAL__

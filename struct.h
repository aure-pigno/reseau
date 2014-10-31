#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>  
#include <sys/stat.h>  
#include <zlib.h>

#define TAILLE 512


struct packet {
uint8_t type : 3;
uint8_t window : 5;
uint8_t seqnum;
uint16_t length;
uint8_t data[512];
uint32_t CRC;

} __attribute__((packed));

struct packet {
uint8_t type : 3;
uint8_t window : 5;
uint8_t seqnum;
uint16_t lenght;
uint8_t data[512];
uint32_t CRC;

} __attribute__((packed));

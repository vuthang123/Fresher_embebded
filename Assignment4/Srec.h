#ifndef SREC_H_
#define SREC_H_

typedef enum srec_type {
    S0_RECORD = 0,
    S1_RECORD,
    S2_RECORD,
    S3_RECORD,
    S4_RECORD,
    S5_RECORD,
    S6_RECORD,
    S7_RECORD,
    S8_RECORD,
    S9_RECORD,
} SREC_type;

typedef struct srec_info {
    SREC_type type; 			// SREC line type
    unsigned char byte_count; 	// number of bytes in data field
    unsigned int address; 		// address field
    unsigned char data[256]; 	// data field (up to 256 bytes)
    unsigned char checksum; 	// checksum field
} SREC_info;

unsigned char char_to_hex(char c);

unsigned char str_to_hex(const char* str);

void Read_srec_Muilt_line(SREC_info info);

int read_srec_line(FILE* input, SREC_info* info);

#endif /*SREC_H_*/
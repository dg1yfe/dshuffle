#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


static void printUsage(){
	printf( "Data Shuffle for 16 bit datatypes\n"
			"\n"
			"Usage: dshuffle [options]\n"
			"\n"
			"Options:\n"
			"  -s    apply offset (change signedness)\n"
			"  -e    swap endianess\n"
			"  -h    show usage info (this text)\n"
			"\n"
			);
}



int main(int argc, char **argv) {
	int arg;
	int changeSignedness = 0;
	int swapEndianess = 0;
	ssize_t len;
	uint8_t buf[4096] __attribute__((aligned(4)));
	uint16_t * p;
	uint8_t byteBuf;
	unsigned int offset = 0;

	for(arg = 0; arg < argc; arg++){
		if(arg == 0)
			continue;
		else if (strncmp(argv[arg],"-s",2)==0){
			changeSignedness = 1;
		}
		else if (strncmp(argv[arg],"-e",2)==0){
			swapEndianess = 1;
		}
		else if (strncmp(argv[arg],"-h",2)==0){
			printUsage();
			return EXIT_SUCCESS;
		}
		else{
			printUsage();
			return EXIT_FAILURE;
		}
	}

	while((len = read(STDIN_FILENO, &buf[offset], sizeof(buf)-offset)) > 0){
		int j;
		if((len + offset) & 1){
			byteBuf = buf[len-1];
			offset = 1;
		}
		for(j=0, p = (uint16_t *) buf;j<(len >> 1);j++, p++){
			uint16_t w;
			if(swapEndianess){
				w = (*p >> 8) | (uint16_t) (*p << 8);
				*p = w;
			}
			if (changeSignedness){
				*p = (uint16_t) (((int32_t) *p) + 32768);
			}
		}
		write(STDOUT_FILENO, buf, (len >> 1) << 1);
		if(offset){
			buf[0] = byteBuf;
		}
	}
	return 0;
}

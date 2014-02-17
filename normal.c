#include <stdio.h>

void usage(){
	printf("norm <input-file> [<output-file>]");
}

int main(int argc, char** argv){
	FILE* in = NULL;
	FILE* out = NULL;

	if(argc < 2){
		usage();
		return -1;
	}

	in = fopen(argv[1], 'r');
	if(argc == 3){
		out = fopen(argv[2], 'w');
	}else{
		out = fopen("out.pgm", 'w');
	}


	return 0;
}
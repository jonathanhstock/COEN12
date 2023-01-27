#include <stdio.h>
#include <stdlib.h>
#define MAX_WORD_LENGTH 30

//main function
int main(int argc, char*argv[]){
	//declaration of file pointer, variable, and buffer
	FILE *fp;
	int count = 0;
	char n[MAX_WORD_LENGTH + 1];
	
	//opens file
	fp = fopen(argv[1], "r");
	
	//if the file is empty prints it can't open file
	if(fp==NULL){
		printf("Could not open file\n");
		return 0;
	}

	//while the function is reading words it increases the counter
	while(fscanf(fp, "%s", n) == 1){
		count++;
	}

	//closes the file
	fclose(fp);
	
	//prints how many words are in the file
	printf("%d total words\n", count);
}

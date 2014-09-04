#include <stdio.h>

int slimak[5][5];

int main(){
	int numer = 0;
	//wypełnij tablice
	
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
			slimak[i][j] = numer++;
	
	//wyświetl tablice
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++)
			printf("%d ", slimak[i][j]);
	
		printf("\n");
	}
}


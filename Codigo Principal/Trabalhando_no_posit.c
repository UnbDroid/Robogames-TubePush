int[2][2] matrix = { 0 };
int i_atual;
int j_atual;
#define NORTE 0
#define LESTE 1
#define SUL 2
#define OESTE 3

/*
0 -> NAO FOI
1 -> ESTA
2 -> TERA QUE PASSAR DENOVO POR ELE
3 -> TERA QUE IR, MAS NAO PASSOU
4 -> NAO PODE RETORNAR!*/
byte orientacao = OESTE;
void Path(){
	if(CAMINHO==VALIDO){

	}
	else if(CAMINHO!=VALIDO){

	}
}
bool GoToMatrix(int i, int j){
	if(frontUS<Parameter){
		return false;
	}
	else {
		if(j < j_atual){
			ChangeDirection(LESTE);
			MoveForward();
		}
		else if(j > j_atual){
			ChangeDirection(OESTE);
			MoveForward();
		}
		if(i < i_atual){
			ChangeDirection(NORTE);
			MoveForward();
		}
		else if(i > i_atual){
			ChangeDirection(LESTE);
			MoveForward();
		}
	}
}

void Next(){
	for(int i = 0; i<2; i++){
		if(i!=1){
			for(int j = 0; j<2; j++){
				if(matrix[i][j]==0){
					if(GoToMatrix(i, j)==true){
						
					}
					else {
						matrix[i][j] = 3;
						i = 0;
						j = 0;
					}
				}
				else if(matrix[i][j]==3){
					if(abs(i + j - i_atual - j_atual)<=1){
						GoToMatrix(i, j);
						matrix[i][j] = 2;
					}
				}
			}
		}
		else {
			for(int j = 2; j > 0; j--){
				if(matrix[i][j]==0){
					if(GoToMatrix()==true){

					}
					else {
						matrix[i][j] = 3;
						i = 0;
						j = 0;
					}
				}
				else if(matrix[i][j]==3){
					if(abs(i + j - i_atual - j_atual)<=1){
						GoToMatrix(i, j);
						matrix[i][j] = 2;
					}
				}
			}
		}
	}
}
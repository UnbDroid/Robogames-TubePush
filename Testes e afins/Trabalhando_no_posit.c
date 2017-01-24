int[2][2] matrix = { 0 };
byte direction = oeste;
int i_atual;
int j_atual;
/*
0 = não passou
1 = está
2 = já passou
3 = precisa passar, não consegue

ALGORITMO:
CORRER A MATRIZ PROCURANDO ONDE TEM QUE IR
CADA VEZ QUE ANDA. MARCA 1 QUANDO ESTIVER E 2 QUANDO PASSAR
SE O LOCAL FOR INACESSIVEL, MARCAR COMO 3 E RECOMEÇAR A BUSCA
SE O PROXIMO LOCAL FOR INACESSIVEL, MARCAR COMO 3 DENOVO E RECOMEÇAR A BUSCA
SE O PROXIMO LOCAL FOR ACESSIVEL, IR PRA ELE E RECOMEÇAR A BUSCA
RECOMEÇAR A BUSCA, E DAR PREFERENCIA PARA IR PARA O 3(ACESSIVEL PROVAVELMENTE)
PREFERENCIA PARA IR AO 3(ACESSIVEL PROVAVELMENTE). AGORA CONTINUAMOS O PERCURSO.*/



void Move(){

}
void GotoMatrix(int i, int j, bool MelhorCaminho){
	if(MelhorCaminho==true){
		if(j_atual<j){
			ChangeDirection(OESTE);
			WalkFoward(UNIDADE);
		}
		else if(j_atual>j){
			ChangeDirection(LESTE);
			WalkFoward(UNIDADE);
		}
		j_atual = j;
		if(i_atual<i){
			ChangeDirection(SUL);
			WalkFoward(UNIDADE);
		}
		else if(i_atual>i){
			ChangeDirection(NORTE);
			WalkFoward(UNIDADE);
		}
		i_atual = i;
	}
	else {
		if(EMPECILHO == true){
			if(abs(i + j - j_atual - i_atual) > 1){
				matrix[i][j] = 3;
				GoToNext();
			}
		}
	}
}

void GoToNext(){
	for(int i = 0; i<2; i++){
		if(i!=1){
			for(int j = 0; j<2 ; j++){
				if(matrix[i][j]==0){
					GoTo(i, j);
				}
			}
		}
		else {
			for(int j = 2; j>0 ; j--){
				if(matrix[i][j]==0){
					GoTo(i, j);
				}
			}
		}
	}
}

void Percurso(){
	GoToNext();
}

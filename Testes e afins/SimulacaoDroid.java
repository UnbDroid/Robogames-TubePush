public class SimulacaoDroid {
	static int[][] matrix = new int[3][3];
	static void printMatrix(int[][] mat){
		for(int i = 0; i<3; i++){
			for(int j = 2; j>=0 ; j--){
			System.out.print(mat[i][j] + " ");
			}
			System.out.println();

		}
		System.out.println(i_atual + "" + j_atual);
	}
	static int i_atual = 0;
	static int j_atual = 0;
	static boolean Valid(int i, int j){
		if(abs(i - i_atual)==1&&abs(j - j_atual)==0){
			return true;
		}
		else if(abs(j - j_atual)==1&&abs(i - i_atual)==0){
			return true;
		}
		else return false;
	}
	static boolean GoToMatrix(int i, int j){
		if((i==2&&j==1)&&i_atual==2&&j_atual==0){
			System.out.println("FALSE!");
			return false;
		}
		else if(i==2&&j==0&&i_atual==2&&j_atual==1){
			System.out.println("FALSE!");
			return false;
		}
		else if(i==2&&j==2&&i_atual==1&&j_atual==2){
			System.out.println("FALSE!");
			return false;
		}
		else if(i==1&&j==2&&i_atual==2&&j_atual==2){
			System.out.println("FALSE!");
			return false;
		}
		else {
			System.out.println(i + "" + j);
			return true;
		}
	}
	static int abs(int a){
		if(a<0){
			return -a;
		}
		else return a;
	}
	static boolean CheckMatrix(int[][] mat){
		for(int i = 0; i<3; i++){
			for(int j = 0; j<3 ; j++){
				if(mat[i][j]!=4&&mat[i][j]!=0){
					return false;
				}
			}
		}
		return true;
	}

/*	static void TEMP(){
		for(int i = 0; i<=2; i++){	
			if(i<0){
				i = 0;
			}
			if(i!=1){
				for(int j = 0; j<=2; j++){
					if(i < 0){
						i = 0;
					}
					if(j < 0){
						j = 0;
					}
					if(matrix[i][j]==0&&Valid(i, j)){
						if(GoToMatrix(i, j)==true){
							matrix[i][j] = 4;
							i_atual = i;
							j_atual = j;
						}
						else {
							matrix[i][j] = 3;
							i = -1;
							j = -1;
						}
					}
					else if(matrix[i][j]==0&&!Valid(i, j)){
						matrix[i][j] = 3;
						i = -1;
						j = -1;
					}
					else if(matrix[i][j]==3&&Valid(i, j)){
						if(GoToMatrix(i, j)==true){
							matrix[i][j] = 4;
							i_atual = i;
							j_atual = j;
							i = -1;
							j = -1;
						}
					}
					else if(matrix[i][j]==4&&flag==true&&Valid(i,j)==true&&retornos<2){
						if(GoToMatrix(i, j)==true){
							i_atual = i;
							j_atual = j;
							flag = false;
							retornos++;
						}
					}
					if(i==2&&j==2&&!(i_atual==2&&j_atual==2)){
						i = -1;
						j = -1;
						flag = true;
						System.out.println("FLAG!");
					}
					if(i_atual ==2&&j_atual==2){
						System.out.println("ACABOU");
						i = 3;
						break;
					}
				}
				printMatrix(matrix);
			}
			else {
				for(int j = 2; j >= 0; j--){
					if(i < 0){
						i = 0;
					}
					if(j < 0){
						j = 0;
					}
					if(matrix[i][j]==0&&Valid(i, j)){
						if(GoToMatrix(i, j)==true){
							matrix[i][j] = 4;
							i_atual = i;
							j_atual = j;
							i = -1;
							j = -1;
						}
						else {
							matrix[i][j] = 3;
							i = -1;
							j = -1;
						}
					}
					else if(matrix[i][j]==0&&!Valid(i, j)){
						matrix[i][j] = 3;
						i = -1;
						j = -1;
					} 
					else if(matrix[i][j]==3&&Valid(i, j)){
						if(GoToMatrix(i, j)==true){
							matrix[i][j] = 4;
							i_atual = i;
							j_atual = j;
							i = -1;
							j = -1;
						}
					}
					else if(matrix[i][j]==4&&flag==true&&Valid(i, j)&&retornos<2){
						if(GoToMatrix(i, j)==true){
							i_atual = i;
							j_atual = j;
							retornos++;
							flag = false;
						}
					}
					if(i_atual!=2&&j_atual!=2&&i==2&&j==2){
						i = -1;
						j = -1;
						flag = true;
						System.out.println("FLAG!");
					}
					if(i_atual ==2&&j_atual==2){
						System.out.println("ACABOU");
						i = 3;
						break;
					}
				}
				printMatrix(matrix);
			}
		}
	}*/
	public static void main(String[] args){
		int retornos = 0;
		matrix[0][0] = 2;
		boolean flag = false;
		printMatrix(matrix);
		while(!(i_atual==2&&j_atual==2)){
			for(int i = 0; i<3; i++){
				if(i<1){
					i = 0;
				}
				if(i!=1){
					for(int j = 0; j < 3; j++){
						if(i < 1){
							i = 0;
						}
						if(j < 1){
							j = 0;
						}
						if(Valid(i, j)==true){
							if(matrix[i][j]==0){
								if(GoToMatrix(i, j) == true){
									i_atual = i;
									j_atual = j;
									matrix[i][j] = 4;
									i = -1;
									j = -1;
								}
								else {
									matrix[i][j] = 3;
									matrix[i_atual][j_atual] = 2;
									i = -1;
									j = -1;
									//	reset?
								}
							}
							else if(matrix[i][j]==3){
								if(GoToMatrix(i, j) == true){
									i_atual = i;
									j_atual = j;
									matrix[i][j] = 4;
									//reset?
								}
							}
							else if(matrix[i][j] == 4&&flag==true){
								if(GoToMatrix(i, j) == true){
									i_atual = i;
									j_atual = j;
									matrix[i][j] = 2;
									retornos++;
									if(retornos>=2){
										flag = false;
									}
								}
							}
						}
						printMatrix(matrix);
					}
				}
				else {
					for(int j = 2; j>=0; j--){
						if(i < 1){
							i = 0;
						}
						if(Valid(i, j)==true){
							if(matrix[i][j]==0){
								if(GoToMatrix(i, j) == true){
									i_atual = i;
									j_atual = j;
									matrix[i][j] = 4;
									i = -1;
									j = -1;
								}
								else {
									matrix[i][j] = 3;
									matrix[i_atual][j_atual] = 2;
									i = -1;
									j = -2;
									//	reset?
								}
							}
							else if(matrix[i][j]==3){
								if(GoToMatrix(i, j) == true){
									i_atual = i;
									j_atual = j;
									matrix[i][j] = 4;
									//reset?
								}
							}
							else if(matrix[i][j] == 4&&flag==true){
								if(GoToMatrix(i, j) == true){
									i_atual = i;
									j_atual = j;
									matrix[i][j] = 2;
									retornos++;
									if(retornos>=2){
										flag = false;
									}
								}
							}
						}
						printMatrix(matrix);
					}
				}
			}
			if(retornos<=2){
				flag = true;
			}
		}
	}
}
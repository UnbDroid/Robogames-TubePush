/* Código alpha para robô do tube push! */
#define LEFT_WHEEL OUT_A
#define RIGHT_WHEEL OUT_C
#define WHEELS OUT_AC
#define CLAW OUT_B

#define FRONT_US IN_1
#define LEFT_US IN_2
#define COLOR IN_3
#define GYRO IN_4

#define US_PARAM 20
#define WHEEL_POWER 50
#define GYRO_POWER 50
#define CLAW_SPEED 55
#define CLAW_HOLD 15
#define OFFSET_SAMPLES 2000
#define CLAW_GRAB 35
#define LIFT_TIME 600
#define WHEEL_RADIUS 1.6 		//EM POLEGADAS
#define SLOWSPEED 40
#define SAFETYFACTOR 0.02

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define RIGHT 1
#define LEFT 0
#define BACK 2


byte leftUS = 255, rightUS = 255; // guardam a leitura atualizada dos ulBACKsons
int i_atual = 0;
int j_atual = 0;
int matrix[][] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
bool alligned = true;
byte botOrientation = WEST;

int InchesToAngle(int inches) {
	return inches*DEGREES_PER_RADIAN/WHEEL_RADIUS;
}

float GetGyroOffset() { // FUNCIONANDO (chamar com os motores desligados)
	float gyroSum = 0, i;

	for(i = 0; i < OFFSET_SAMPLES; ++i)
	{
		gyroSum += SensorHTGyro(GYRO);
	}

	return gyroSum/OFFSET_SAMPLES;
}

bool Valid(int i, int j){
	if(abs(i - i_atual)==1&&abs(j - j_atual)==0){
		return true;
	}
	else if(abs(j - j_atual)==1&&abs(i - i_atual)==0){
		return true;
	}
	else return false;
}

void LiftTubes() {
     int aux;
     
	 OnRev(CLAW, CLAW_GRAB);
     aux = MotorRotationCount(CLAW);
     Wait(100);
     
	 while((aux - MotorRotationCount(CLAW)) > 0) {
          aux = MotorRotationCount(CLAW);
          Wait(100);
          TextOut(20, LCD_LINE5, "passei");

     }
     Off(CLAW);
     OnRev(CLAW, CLAW_SPEED);
     Wait(LIFT_TIME);
     OnRev(CLAW, CLAW_HOLD);
}

sub ChangeDirection(byte side) {
	float degrees;
	float angle = 0, gyro = 0;
	unsigned long time = CurrentTick(), prevTime;
	
	OnRev(CLAW, CLAW_SPEED);
	Wait(400);
	Off(CLAW);
	Off(WHEELS);

	SetSensorHTGyro(GYRO);
	
	float offset = GetGyroOffset();
	
	if(side == RIGHT) {
		degrees = 90;
		OnFwd(LEFT_WHEEL, GYRO_POWER);
		OnRev(RIGHT_WHEEL, GYRO_POWER);
		while(angle < degrees) {
			prevTime = time;
			time = CurrentTick();
			gyro = SensorHTGyro(GYRO);
			angle += (gyro - offset) * (time - prevTime)/1000.0;
			ClearLine(LCD_LINE1);
			TextOut(0, LCD_LINE1, "ANGLE:");
			NumOut(48, LCD_LINE1, offset);
			NumOut(48, LCD_LINE2, angle);
			NumOut(48, LCD_LINE3, gyro);
		}
		switch(botOrientation)
		{
			case NORTH:
				botOrientation = EAST;
				break;
			case SOUTH:
				botOrientation = WEST;
				break;
			case EAST:
				botOrientation = SOUTH;
				break;
			case WEST:
				botOrientation = NORTH;
				break;
		}
	} else if(side == LEFT) {
		degrees = -90;
		OnFwd(RIGHT_WHEEL, GYRO_POWER);
		OnRev(LEFT_WHEEL, GYRO_POWER);
		while(angle > degrees) {
			prevTime = time;
			time = CurrentTick();
			gyro = SensorHTGyro(GYRO);
			angle += (gyro - offset) * (time - prevTime)/1000.0;
			//ClearLine(LCD_LINE1);
			//TextOut(0, LCD_LINE1, "ANGLE:");
			//NumOut(48, LCD_LINE1, angle);
		}
		switch(botOrientation)
		{
			case NORTH:
				botOrientation = WEST;
				break;
			case SOUTH:
				botOrientation = EAST;
				break;
			case EAST:
				botOrientation = NORTH;
				break;
			case WEST:
				botOrientation = SOUTH;
				break;
		}
	} else if(side = BACK) {
		degrees = 180;
		OnFwd(LEFT_WHEEL, GYRO_POWER);
		OnRev(RIGHT_WHEEL, GYRO_POWER);
		while(angle < degrees) {
			prevTime = time;
			time = CurrentTick();
			gyro = SensorHTGyro(GYRO);
			angle += (gyro - offset) * (time - prevTime)/1000.0;
			//ClearLine(LCD_LINE1);
			//TextOut(0, LCD_LINE1, "ANGLE:");
			//NumOut(48, LCD_LINE1, angle);
		}
		switch(botOrientation)
		{
			case NORTH:
				botOrientation = SOUTH;
				break;
			case SOUTH:
				botOrientation = NORTH;
				break;
			case EAST:
				botOrientation = WEST;
				break;
			case WEST:
				botOrientation = EAST;
				break;
		}
	}
	Off(WHEELS);
	OnFwd(CLAW, CLAW_SPEED);
	Wait(700);
	Off(CLAW);
}

sub Allign(){
	long countBeginEsq = MotorRotationCount(RIGHT_WHEEL);
	long countBeginDir = MotorRotationCount(LEFT_WHEEL);
	long tc1;
	long tc2;
	OnRev(WHEELS, SLOWSPEED);
	Wait(1000);
/*	do{
		tc1 = MotorRotationCount(RIGHT_WHEEL) - countBeginDir;
		tc2 = MotorRotationCount(LEFT_WHEEL) - countBeginEsq;
	}while(abs(tc1 - tc2) > SAFETYFACTOR);
*/
	Off(WHEELS);
}

bool NextToWall(){
	ClearScreen();
	Wait(200);
	TextOut(0, LCD_LINE1, "botOriet");
	NumOut(48, LCD_LINE1, botOrientation);
	TextOut(0, LCD_LINE2, "i atual");
	NumOut(48, LCD_LINE2, i_atual);
	TextOut(0, LCD_LINE3, "j_atual");
	NumOut(48, LCD_LINE3, j_atual);
	Wait(1000);

	switch(botOrientation)
	{
		case NORTH:
		{
			if(i_atual==2){
				alligned = true;
				return true;
			}
			else {
				alligned = false;
				return false;
			}
		}
		case SOUTH:
		{
			if(i_atual==0){
				alligned = true;
				return true;
			}
			else {
				alligned = false;
				return false;
			}
		}
		case WEST:
		{
			if(j_atual==0){
				alligned = true;
				return true;
			}
			else {
				alligned = false;
				return false;
			}
		}
		case EAST:
		{
			if(j_atual==2){
				alligned = true;
				return true;
			}
			else {
				alligned = false;
				return false;
			}
		}
	}
	alligned = false;
	return false;
}

sub ChangeOrientation(byte orientation){
	switch(botOrientation){
		case NORTH:
			if(orientation == WEST) {
				ChangeDirection(LEFT);
			} else if(orientation == EAST) {
				ChangeDirection(RIGHT);
			} else if(orientation == SOUTH) {
				ChangeDirection(BACK);
			}
			break;
		case SOUTH:
			if(orientation == EAST) {
				ChangeDirection(LEFT);
			} else if(orientation == WEST) {
				ChangeDirection(RIGHT);
			} else if(orientation == NORTH) {
				ChangeDirection(BACK);
			}
			break;
		case EAST:
			if(orientation == NORTH) {
				ChangeDirection(LEFT);
			} else if(orientation == SOUTH) {
				ChangeDirection(RIGHT);
			} else if(orientation == WEST) {
				ChangeDirection(BACK);
			}
			break;
		case WEST:
			if(orientation == SOUTH) {
				ChangeDirection(LEFT);
			} else if(orientation == NORTH) {
				ChangeDirection(RIGHT);
			} else if(orientation == EAST){
				ChangeDirection(BACK);
			}
			break;
	}
	if(NextToWall()==true){
		Allign();
	}
}

void UpdateUS() { // FUNCIONANDO
	int tmp = SensorUS(FRONT_US), i = 0;

	while((tmp == 255 || tmp == 0 || abs(tmp - rightUS) > 30) && i < 3)
	{
		tmp = (tmp + SensorUS(FRONT_US))/2;
		++i;
	}
	
	rightUS = 6*rightUS/10 + 4*tmp/10;
	//ClearLine(LCD_LINE7);
	//TextOut(0, LCD_LINE7, "FRONT_US");
	//NumOut(64, LCD_LINE7, rightUS);
	tmp = SensorUS(LEFT_US);				//qual porta?
	i = 0;
	while((tmp == 255 || tmp == 0 || abs(tmp - leftUS) > 30) && i < 3)
	{
		tmp = (tmp + SensorUS(FRONT_US))/2;
		++i;
	}
	
	leftUS = 6*leftUS/10 + 4*tmp/10;
	//ClearLine(LCD_LINE8);
	//TextOut(0, LCD_LINE8, "LEFT_US");
	//NumOut(64, LCD_LINE8, leftUS);
}

void WalkForward(int inches){
	ResetTachoCount(WHEELS);
	Off(WHEELS);
	
	int angle = InchesToAngle(inches);
	long leftTachoBegin, leftTachoSpeed;
	unsigned long begin = CurrentTick(), lastTime;
	
	OnFwdReg(WHEELS, WHEEL_POWER, OUT_REGMODE_SYNC);
	
	//while(MotorTachoCount(RIGHT_WHEEL) < angle && (leftUS < US_PARAM || rightUS < US_PARAM)) {	
	while(MotorTachoCount(RIGHT_WHEEL) < angle){
	}
	Off(WHEELS);
}
void WalkBackwards(int inches){
	ResetTachoCount(WHEELS);
	Off(WHEELS);
	
	int angle = InchesToAngle(inches);
	long leftTachoBegin, leftTachoSpeed;
	unsigned long begin = CurrentTick(), lastTime;
	
	OnRevReg(WHEELS, WHEEL_POWER, OUT_REGMODE_SYNC);
	
	while(MotorTachoCount(RIGHT_WHEEL) > angle) {	

	}
	Off(WHEELS);
}
void PullClaw(){
	OnRev(CLAW, CLAW_GRAB);
	Wait(2000);
	OnRev(CLAW, CLAW_HOLD);
}
bool GoToMatrix(int i, int j){
		if(j > j_atual){
			ChangeOrientation(WEST);
		}
		else if(j < j_atual){
			ChangeOrientation(EAST);
		}
		if(i < i_atual){
			ChangeOrientation(NORTH);
		}
		else if(i > i_atual){
			ChangeOrientation(SOUTH);
		}
		Wait(1000);
		if(i_atual==2&&j_atual==1&&i==2&&j==2){
			PullClaw();
			WalkForward(18);
		}
		if(SensorUS(FRONT_US) > US_PARAM){
			if(alligned==true){
				WalkForward(20);
				
			}
			else {
				WalkForward(16);
			}
			return true;
		}
		else {
			return false;
		}
	}

task main(){
	SetSensorHTGyro(GYRO);
	SetSensorLowspeed(FRONT_US);
	SetSensorLowspeed(LEFT_US);
	int retornos = 0;
	matrix[0][0] = 2;
	bool flag = false;
	Wait(2000);
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
				}
			}
		}
		if(retornos<=2){
			flag = true;
		}
	}
	
}
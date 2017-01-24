/* Código alpha para robô do tube push! */
#define LEFT_WHEEL OUT_A
#define RIGHT_WHEEL OUT_C
#define WHEELS OUT_AC
#define CLAW OUT_B

#define RIGHT_US IN_1
#define LEFT_US IN_2
#define COLOR IN_3
#define GYRO IN_4

#define US_PARAM 20
#define WHEEL_POWER 60
#define GYRO_POWER 50
#define CLAW_SPEED 55
#define CLAW_HOLD 30
#define OFFSET_SAMPLES 1000
#define CLAW_GRAB 28
#define LIFT_TIME 600
#define WHEEL_RADIUS 1 		//EM POLEGADAS

#define RIGHT 1
#define LEFT 0
#define BACK 2


byte leftUS = 255, rightUS = 255; // guardam a leitura atualizada dos ultrassons

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

void LiftTubes() {
     int aux;
     
	 OnRev(CLAW, CLAW_GRAB);
     aux = MotorRotationCount(CLAW);
     Wait(100);
     
	 while((aux - MotorRotationCount(CLAW)) > 0) {
          aux = MotorRotationCount(CLAW);
          Wait(100);
          TextOut(20, LCD_LINE2, "passei");

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
	}
	Off(WHEELS);
	OnFwd(CLAW, CLAW_SPEED);
	Wait(700);
	Off(CLAW);
}

void UpdateUS() { // FUNCIONANDO
	int tmp = SensorUS(RIGHT_US), i = 0;

	while((tmp == 255 || tmp == 0 || abs(tmp - rightUS) > 30) && i < 3)
	{
		tmp = (tmp + SensorUS(RIGHT_US))/2;
		++i;
	}
	
	rightUS = 6*rightUS/10 + 4*tmp/10;
	//ClearLine(LCD_LINE7);
	//TextOut(0, LCD_LINE7, "RIGHT_US");
	//NumOut(64, LCD_LINE7, rightUS);
	tmp = SensorUS(LEFT_US);				//qual porta?
	i = 0;
	while((tmp == 255 || tmp == 0 || abs(tmp - leftUS) > 30) && i < 3)
	{
		tmp = (tmp + SensorUS(RIGHT_US))/2;
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
		UpdateUS();
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
	OnRev(CLAW, CLAW_SPEED);
	Wait(2000);
	OnRev(CLAW, CLAW_HOLD);
}
task main(){
	SetSensorHTGyro(GYRO);
	WalkForward(16);
	ChangeDirection(RIGHT);
	WalkForward(16);
	ChangeDirection(LEFT);
	WalkForward(8);
//	LiftTubes();
	PullClaw();
	
}

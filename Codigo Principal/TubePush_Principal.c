/* Código alpha para robô do tube push! */
#define RODA_ESQ OUT_A
#define RODA_DIR OUT_C
#define RODAS OUT_AC
#define CLAW OUT_B

#define USDIR IN_1
#define USESQ IN_2
#define COLOR IN_3
#define GYRO IN_4

#define UsParam 20
#define RODA_POWER 60
#define GYRO_POWER 50
#define CLAWSPEED 55
#define CLAWHOLD 30
#define OFFSET_SAMPLES 1000
#define CLAWGRAB 28
#define TEMPOERGUE 600
#define RAIO_RODAS 1 		//EM POLEGADAS

#define DIREITA 1
#define ESQUERDA 0
#define TRAS 2


byte usesq = 255, usdir = 255; // guardam a leitura atualizada dos ultrassons
int InchesToAngle(int inches){
	return inches*DEGREES_PER_RADIAN/RAIO_RODAS;
}

float GetGyroOffset() { // FUNCIONANDO (chamar com os motores desligados)
	float gyro_sum = 0, i;

	for(i = 0; i < OFFSET_SAMPLES; ++i)
	{
		gyro_sum += SensorHTGyro(GYRO);
	}

	return gyro_sum/OFFSET_SAMPLES;
}

void levanta() {
     int aux;
     OnRev(CLAW, CLAWGRAB);
     aux=MotorRotationCount(CLAW);
     Wait(100);
     while((aux-MotorRotationCount(CLAW))>0){
          aux=MotorRotationCount(CLAW);
          Wait(100);
          TextOut(20, LCD_LINE2, "passei");

     }
     Off(CLAW);
     OnRev(CLAW, CLAWSPEED);
     Wait(TEMPOERGUE);
     OnRev(CLAW, CLAWHOLD);
}

sub ChangeDirection(byte side) {
	float degrees;
	float angle = 0, gyro = 0;
	unsigned long time = CurrentTick(), prev_time;
	OnRev(CLAW, CLAWSPEED);
	Wait(400);
	Off(CLAW);
	Off(RODAS);

	float offset = GetGyroOffset();

	SetSensorHTGyro(GYRO);
	
	if(side == DIREITA) {
		degrees = 90;
		OnFwd(RODA_ESQ, GYRO_POWER);
		OnRev(RODA_DIR, GYRO_POWER);
		while(angle < degrees) {
			prev_time = time;
			time = CurrentTick();
			gyro = SensorHTGyro(GYRO);
			angle += (gyro - offset) * (time - prev_time)/1000.0;
			ClearLine(LCD_LINE1);
			TextOut(0, LCD_LINE1, "ANGLE:");
			NumOut(48, LCD_LINE1, offset);
			NumOut(48, LCD_LINE2, angle);
			NumOut(48, LCD_LINE3, gyro);
		}
	} else if(side == ESQUERDA) {
		degrees = -90;
		OnFwd(RODA_DIR, GYRO_POWER);
		OnRev(RODA_ESQ, GYRO_POWER);
		while(angle > degrees) {
			prev_time = time;
			time = CurrentTick();
			gyro = SensorHTGyro(GYRO);
			angle += (gyro - offset) * (time - prev_time)/1000.0;
			//ClearLine(LCD_LINE1);
			//TextOut(0, LCD_LINE1, "ANGLE:");
			//NumOut(48, LCD_LINE1, angle);
		}
	} else if(side = TRAS) {
		degrees = 180;
		OnFwd(RODA_ESQ, GYRO_POWER);
		OnRev(RODA_DIR, GYRO_POWER);
		while(angle < degrees) {
			prev_time = time;
			time = CurrentTick();
			gyro = SensorHTGyro(GYRO);
			angle += (gyro - offset) * (time - prev_time)/1000.0;
			//ClearLine(LCD_LINE1);
			//TextOut(0, LCD_LINE1, "ANGLE:");
			//NumOut(48, LCD_LINE1, angle);
		}
	}
	Off(RODAS);
	OnFwd(CLAW, CLAWSPEED);
	Wait(700);
	Off(CLAW);
}

void UpdateUS() { // FUNCIONANDO
	int tmp = SensorUS(USDIR), i = 0;

	while((tmp == 255 || tmp == 0 || abs(tmp - usdir) > 30) && i < 3)
	{
		tmp = (tmp + SensorUS(USDIR))/2;
		++i;
	}
	
	usdir = 6*usdir/10 + 4*tmp/10;
	//ClearLine(LCD_LINE7);
	//TextOut(0, LCD_LINE7, "USDIR");
	//NumOut(64, LCD_LINE7, usdir);
	tmp = SensorUS(USESQ);				//qual porta?
	i = 0;
	while((tmp == 255 || tmp == 0 || abs(tmp - usesq) > 30) && i < 3)
	{
		tmp = (tmp + SensorUS(USDIR))/2;
		++i;
	}
	
	usesq = 6*usesq/10 + 4*tmp/10;
	//ClearLine(LCD_LINE8);
	//TextOut(0, LCD_LINE8, "USESQ");
	//NumOut(64, LCD_LINE8, usesq);
}

void WalkFoward(int inches){
	ResetTachoCount(RODAS);
	Off(RODAS);
	int angle = InchesToAngle(inches);
	long tachoesq_begin, tachoesq_speed;
	unsigned long begin = CurrentTick(), lasttime;
	
	OnFwdReg(RODAS, RODA_POWER, OUT_REGMODE_SYNC);
	
	//while(MotorTachoCount(RODA_DIR) < angle && (usesq < UsParam || usdir < UsParam)) {	
	while(MotorTachoCount(RODA_DIR) < angle){
	UpdateUS();
	}
	Off(RODAS);
}
void WalkBackwards(int inches){
	ResetTachoCount(RODAS);
	Off(RODAS);
	int angle = InchesToAngle(inches);
	long tachoesq_begin, tachoesq_speed;
	unsigned long begin = CurrentTick(), lasttime;
	
	OnRevReg(RODAS, RODA_POWER, OUT_REGMODE_SYNC);
	
	while(MotorTachoCount(RODA_DIR) > angle) {	

	}
	Off(RODAS);
}

void PullClaw(){
	OnRev(CLAW, CLAWSPEED);
	Wait(2000);
	OnRev(CLAW, CLAWHOLD);
}
task main(){
	SetSensorHTGyro(GYRO);
	WalkFoward(16);
	ChangeDirection(DIREITA);
	WalkFoward(16);
	ChangeDirection(ESQUERDA);
	WalkFoward(8);
//	levanta();
	PullClaw();
	
}
#include "MPU6050.h"
#include "HardwareSerial.h"

/*
* R represents the measurement covariance noise. In this case,
* it is a 1x1 matrix that says that we expect 0.3 rad jitter
* from the accelerometer.
*/
static const float R_angle = 0.3; 		//.3 default

/*
* Q is a 2x2 matrix that represents the process covariance noise.
* In this case, it indicates how much we trust the acceleromter
* relative to the gyros
*/
static const float Q_angle = 0.01;	//0.01 (Kalman)
static const float Q_gyro = 0.04;	//0.04 (Kalman)

MPU6050::MPU6050() {
	for(int i=0; i<5; i++) {
		xInit[i]=0; yInit[i]=0; zInit[i]=0;
	}

	initIndex=0;
	initSize=5;
	xCal=0;
	yCal=0;
	zCal=0;

	lowX=-150;
	highX=150;
	lowY=-150;
	highY=150;
	lowZ=-150;
	highZ=150;

}

MPU6050::~MPU6050() {
}


/**************************************************
 * Sensor read/write
 **************************************************/
int MPU6050::MPU6050_read(int start, uint8_t *buffer, int size)
{
	int i, n, error;
	
	Wire.beginTransmission(MPU6050_I2C_ADDRESS);
	
	n = Wire.write(start);
	if (n != 1)
		return (-10);
	
	n = Wire.endTransmission(false); // hold the I2C-bus
	if (n != 0)
		return (n);
	
	// Third parameter is true: relase I2C-bus after data is read.
	Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
	i = 0;
	while(Wire.available() && i<size)
	{
		buffer[i++]=Wire.read();
	}
	if ( i != size)
		return (-11);
	return (0); // return : no error
}

int MPU6050::MPU6050_write(int start, const uint8_t *pData, int size)
{
	int n, error;
	
	Wire.beginTransmission(MPU6050_I2C_ADDRESS);
	
	n = Wire.write(start); // write the start address
	if (n != 1)
		return (-20);
		
	n = Wire.write(pData, size); // write data bytes
	if (n != size)
		return (-21);
		
	error = Wire.endTransmission(true); // release the I2C-bus
	if (error != 0)
		return (error);
	return (0); // return : no error
}

int MPU6050::MPU6050_write_reg(int reg, uint8_t data)
{
	int error;
	error = MPU6050_write(reg, &data, 1);
	return (error);
}

/**************************************************
 * Raw data processing
 **************************************************/
float MPU6050::angleInDegrees(int lo, int hi, int measured) {
	float x = (hi - lo)/180.0;
	return (float)measured/x;
}

void MPU6050::initGyroKalman(struct GyroKalman *kalman, const float Q_angle, const float Q_gyro, const float R_angle) {
	kalman->Q_angle = Q_angle;
	kalman->Q_gyro = Q_gyro;
	kalman->R_angle = R_angle;
	
	kalman->P_00 = 0;
	kalman->P_01 = 0;
	kalman->P_10 = 0;
	kalman->P_11 = 0;
}

/*
* The kalman predict method.
* kalman 		the kalman data structure
* dotAngle 		Derivitive Of The (D O T) Angle. This is the change in the angle from the gyro.
* 					This is the value from the Wii MotionPlus, scaled to fast/slow.
* dt 				the change in time, in seconds; in other words the amount of time it took to sweep dotAngle
*/
void MPU6050::predict(struct GyroKalman *kalman, float dotAngle, float dt) {
	kalman->x_angle += dt * (dotAngle - kalman->x_bias);
	kalman->P_00 += -1 * dt * (kalman->P_10 + kalman->P_01) + dt*dt * kalman->P_11 + kalman->Q_angle;
	kalman->P_01 += -1 * dt * kalman->P_11;
	kalman->P_10 += -1 * dt * kalman->P_11;
	kalman->P_11 += kalman->Q_gyro;
}

/*
* The kalman update method
* kalman 	the kalman data structure
* angle_m 	the angle observed from the Wii Nunchuk accelerometer, in radians
*/
float MPU6050::update(struct GyroKalman *kalman, float angle_m) {
	const float y = angle_m - kalman->x_angle;
	const float S = kalman->P_00 + kalman->R_angle;
	const float K_0 = kalman->P_00 / S;
	const float K_1 = kalman->P_10 / S;
	kalman->x_angle += K_0 * y;
	kalman->x_bias += K_1 * y;
	kalman->P_00 -= K_0 * kalman->P_00;
	kalman->P_01 -= K_0 * kalman->P_01;
	kalman->P_10 -= K_1 * kalman->P_00;
	kalman->P_11 -= K_1 * kalman->P_01;
	return kalman->x_angle;
}

void MPU6050::begin() 
{
	int error;
	uint8_t c;

	initGyroKalman(&angX, Q_angle, Q_gyro, R_angle);
	initGyroKalman(&angY, Q_angle, Q_gyro, R_angle);
	initGyroKalman(&angZ, Q_angle, Q_gyro, R_angle);

	Wire.begin();
	
	// default at power-up:
	// Gyro at 250 degrees second
	// Acceleration at 2g
	// Clock source at internal 8MHz
	// The device is in sleep mode.
	//
	error = MPU6050_read (MPU6050_WHO_AM_I, &c, 1);
	Serial.print(F("WHO_AM_I : "));
	Serial.print(c,HEX);
	Serial.print(F(", error = "));
	Serial.println(error,DEC);

	// According to the datasheet, the 'sleep' bit
	// should read a '1'. But I read a '0'.
	// That bit has to be cleared, since the sensor
	// is in sleep mode at power-up. Even if the
	// bit reads '0'.
	error = MPU6050_read (MPU6050_PWR_MGMT_2, &c, 1);
	Serial.print(F("PWR_MGMT_2 : "));
	Serial.print(c,HEX);
	Serial.print(F(", error = "));
	Serial.println(error,DEC);

	// Clear the 'sleep' bit to start the sensor.
	MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);

	prevMillis = 0;
}

void MPU6050::ReadData(int *pX, int *pY, int *pZ, unsigned long m) 
{
	int gx1=0, gy1=0, gz1 = 0;
	float gx2=0, gy2=0, gz2 = 0;

	int error;
	double dT;
	accel_t_gyro_union accel_t_gyro;

	unsigned long loopTime;

//	Serial.println(F(""));
//	Serial.println(F("MPU-6050"));
	
	// Read the raw values.
	// Read 14 bytes at once,
	// containing acceleration, temperature and gyro.
	// With the default settings of the MPU-6050,
	// there is no filter enabled, and the values
	// are not very stable.
	error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));
	if(error != 0) {
		Serial.print(F("Read accel, temp and gyro, error = "));
		Serial.println(error,DEC);
	}
	// Swap all high and low bytes.
	// After this, the registers values are swapped,
	// so the structure name like x_accel_l does no
	// longer contain the lower byte.
	uint8_t swap;
	#define SWAP(x,y) swap = x; x = y; y = swap
	SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
	SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
	SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
	SWAP (accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
	SWAP (accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
	SWAP (accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
	SWAP (accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);


	loopTime = m - prevMillis;

	gx2 = angleInDegrees(lowX, highX, accel_t_gyro.value.x_gyro);
	gy2 = angleInDegrees(lowY, highY, accel_t_gyro.value.y_gyro);
	gz2 = angleInDegrees(lowZ, highZ, accel_t_gyro.value.z_gyro);


	predict(&angX, gx2, loopTime);
	predict(&angY, gy2, loopTime);
	predict(&angZ, gz2, loopTime);
	

	gx1 = update(&angX, accel_t_gyro.value.x_accel) / 10;
	gy1 = update(&angY, accel_t_gyro.value.y_accel) / 10;
	gz1 = update(&angZ, accel_t_gyro.value.z_accel) / 10;

	if(initIndex < initSize) {
		xInit[initIndex] = gx1;
		yInit[initIndex] = gy1;
		zInit[initIndex] = gz1;
		if(initIndex == initSize - 1) {
			int sumX = 0; int sumY = 0; int sumZ = 0;
			for(int k=1; k <= initSize; k++) {
				sumX += xInit[k];
				sumY += yInit[k];
				sumZ += zInit[k];
			}

			xCal -= sumX/(initSize -1);
			yCal -= sumY/(initSize -1);
			zCal = (sumZ/(initSize -1) - zCal);
		}
		initIndex++;
	}
	
	else {
		gx1 += xCal;
		gy1 += yCal;
		gz1 += zCal;
	}

	if(gx1 > highX) highX = gx1;
	else if(gx1 < lowX) lowX = gx1;
	if(gy1 > highY) highY = gy1;
	else if(gy1 < lowY) lowY = gy1;
	if(gz1 > highZ) highZ = gz1;
	else if(gz1 < lowZ) lowZ = gz1;

	gx1 = angleInDegrees(lowX, highX, gx1);
	gy1 = angleInDegrees(lowY, highY, gy1);
	gz1 = angleInDegrees(lowZ, highZ, gz1);

	*pX=gx1; 
	*pY=gy1;
	*pZ=gz1;

	prevMillis = m;
}
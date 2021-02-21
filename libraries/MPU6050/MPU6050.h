
#include <Wire.h>


/* MPU-6050 sensor */
#define MPU6050_ACCEL_XOUT_H 0x3B // R
#define MPU6050_PWR_MGMT_1 0x6B // R/W
#define MPU6050_PWR_MGMT_2 0x6C // R/W
#define MPU6050_WHO_AM_I 0x75 // R
#define MPU6050_I2C_ADDRESS 0x68


/* Kalman filter */
class GyroKalman{
public:
	/* These variables represent our state matrix x */
	float x_angle, x_bias;

	/* Our error covariance matrix */
	float P_00, P_01, P_10, P_11;
	
	/*
	* Q is a 2x2 matrix of the covariance. Because we
	* assume the gyro and accelerometer noise to be independent
	* of each other, the covariances on the / diagonal are 0.
	* Covariance Q, the process noise, from the assumption
	* x = F x + B u + w
	* with w having a normal distribution with covariance Q.
	* (covariance = E[ (X - E[X])*(X - E[X])' ]
	* We assume is linear with dt
	*/
	float Q_angle, Q_gyro;

	/*
	* Covariance R, our observation noise (from the accelerometer)
	* Also assumed to be linear with dt
	*/
	float R_angle;
};


typedef union accel_t_gyro_union
{
	struct
	{
	uint8_t x_accel_h;
	uint8_t x_accel_l;
	uint8_t y_accel_h;
	uint8_t y_accel_l;
	uint8_t z_accel_h;
	uint8_t z_accel_l;
	uint8_t t_h;
	uint8_t t_l;
	uint8_t x_gyro_h;
	uint8_t x_gyro_l;
	uint8_t y_gyro_h;
	uint8_t y_gyro_l;
	uint8_t z_gyro_h;
	uint8_t z_gyro_l;
	} reg;

	struct
	{
		int x_accel;
		int y_accel;
		int z_accel;
		int temperature;
		int x_gyro;
		int y_gyro;
		int z_gyro;
	} value;
};


class MPU6050
{
public:
	MPU6050();
	~MPU6050();
	
	int xInit[5];
	int yInit[5];
	int zInit[5];
	int initIndex;
	int initSize;
	int xCal;
	int yCal;
	int zCal;
	
	//These are the limits of the values I got out of the Nunchuk accelerometers (yours may vary).
	int lowX;
	int highX;
	int lowY;
	int highY;
	int lowZ;
	int highZ;
	
	GyroKalman angX;
	GyroKalman angY;
	GyroKalman angZ;

	unsigned long prevMillis;


	void begin();
	void ReadData(int *pX, int *pY, int *pZ, unsigned long m);
	int MPU6050_read(int start, uint8_t *buffer, int size);
	int MPU6050_write(int start, const uint8_t *pData, int size);
	int MPU6050_write_reg(int reg, uint8_t data);
	float angleInDegrees(int lo, int hi, int measured);
	void initGyroKalman(struct GyroKalman *kalman, const float Q_angle, const float Q_gyro, const float R_angle);
	void predict(struct GyroKalman *kalman, float dotAngle, float dt);
	float update(struct GyroKalman *kalman, float angle_m);
};
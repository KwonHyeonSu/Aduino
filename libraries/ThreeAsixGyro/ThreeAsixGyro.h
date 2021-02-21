/* GBTec_RnD 2012.03.26 Ver.01 */
/* ThreeAsixGyro.h */

#ifndef __3ASIXGYRO_H__
#define __3ASIXGYRO_H__

#include <inttypes.h>

#define REG_WHO		0x00
#define	REG_SMPL	0x15
#define REG_DLPF	0x16
#define REG_INTC	0x17
#define REG_INTS	0x1A
#define	REG_TMPH	0x1B
#define	REG_TMPL	0x1C
#define	REG_GXH		0x1D
#define	REG_GXL		0x1E
#define	REG_GYH		0x1F
#define	REG_GYL		0x20
#define REG_GZH		0x21
#define REG_GZL		0x22
#define REG_PWRM	0x3E

class ThreeAsixGyro
{
public:
	//ThreeAsixGyro();

	void begin(uint8_t addr=105);
	int readXValue();
	int readYValue();
	int readZValue();

private:
	uint8_t gyroAddr;
	void gyroInit();
	int read(uint8_t regh, uint8_t regl);
	uint8_t read(uint8_t reg);
};

extern ThreeAsixGyro gThreeAsixGyro;
#endif
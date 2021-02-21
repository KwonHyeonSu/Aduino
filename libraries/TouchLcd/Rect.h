#ifndef __RECT_H__
#define __RECT_H__

#include "Arduino.h"

class Rect {

	public:
		int x;
		int y;
		int w;
		int h;

		Rect();
		Rect(int x, int y, int w, int h);

		void SetRect(int x, int y, int w, int h);
		boolean PtInRect(int sx, int sy);

	private:
};

#endif // __RECT_H__

#include "Rect.h"


Rect::Rect() {
	Rect(0,0,0,0);
}

Rect::Rect(int tx, int ty, int tw, int th) {
	SetRect(tx, ty, tw, th);
}

void Rect::SetRect(int tx, int ty, int tw, int th) {
	x = tx;
	y = ty;
	w = tw;
	h = th;
}

boolean Rect::PtInRect(int sx, int sy) {
	if(sx > x && sx < x+w) {
		if(sy > y && sy < y+h)
			return true;
	}

	return false;
}

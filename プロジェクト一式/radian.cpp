#include "radian.h"

#include "main.h"

#define	D3DX_PI2	(D3DX_PI * 2)

double NormalizeRadian(double rad);


double LerpRadian(double a, double b, float t)
{
	//t‚Ì•â³
	if (t < 0.0f)
		t = 0.0f;
	if (t > 1.0f)
		t = 1.0f;

	return NormalizeRadian(a + t * NormalizeRadian(b - a));
}

double NormalizeRadian(double rad)
{
	//2ƒÎ‚ğí‚é
	int num = (int)(rad / D3DX_PI2);
	double temp = rad - (D3DX_PI2 * num);

	//-ƒÎ`+ƒÎ‚Ì’l‚É‚È‚é—l‚É•â³‚·‚é
	if (temp < -D3DX_PI) {
		temp += 2 * D3DX_PI;
	}
	if (temp > D3DX_PI) {
		temp -= 2 * D3DX_PI;
	}

	return temp;
}


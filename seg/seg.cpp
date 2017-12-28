// seg.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main()
{
	float lrot;
	float srf;
	float posr[2];
	float posa[2];
	float ld;
	float velr[2];
	float dvelr[2];
	float t;
	float aa1m;
	float dp[2];
	float d;
	float ac[2];
	float vr[2];
	float p[2][2];
	float ar;
	float dv;
	float sy;
	float dot;
	float velrp[2];
	float dld;

#define DT	0.00001f

	printf("DT=%f s\r\n", DT);

	printf("enter length of rotation in meters: ");
	scanf("%f", &lrot);

	printf("enter straight resistance factor ratio: ");
	scanf("%f", &srf);

	posr[0] = 0;
	posr[1] = sy = lrot / (2.0f * 3.14159f);
	printf("starting position of rotator at t0 = %f,%f\r\n", posr[0], posr[1]);

	printf("enter position of attractor x: ");
	scanf("%f", &posa[0]);

	printf("enter position of attractor y: ");
	scanf("%f", &posa[1]);

	printf("enter acceleration of attractor at 1 m: ");
	scanf("%f", &aa1m);
	aa1m *= DT;

	printf("enter starting velocity forward: ");
	scanf("%f", &velr[0]);
	velr[0] *= -1.0f * DT;
	velr[1] = 0;

	ld = 0;

	t = 0;

	dvelr[0] = dvelr[1] = 0;
	dv = 0;

	while (ld < lrot * 10)
	{
		t += DT;

		dp[0] = posa[0] - posr[0];
		dp[1] = posa[1] - posr[1];
		d = sqrtf(dp[0] * dp[0] + dp[1] * dp[1]);

		ac[0] = dp[0] / (d*d*d);
		ac[1] = dp[1] / (d*d*d);

		d = sqrtf(velr[0] * velr[0] + velr[1] * velr[1]);

		p[0][0] = posr[0];
		p[0][1] = posr[1];
		ar = atan2(p[0][1], p[0][0]);
		ar += 2.0f * 3.14159f * (d / lrot);
		p[1][0] = cos(ar);
		p[1][1] = sin(ar);

		p[0][0] = p[1][0] - p[0][0];
		p[0][1] = p[1][1] - p[0][1];
		d = sqrtf(p[0][0] * p[0][0] + p[0][1] * p[0][1]);
		p[0][0] /= d;
		p[0][1] /= d;

		dot = p[0][0] * velr[0] + p[0][1] * velr[1];
		velrp[0] = velr[0] * fabs(dot);
		velrp[1] = velr[1] * fabs(dot);

		dot = ac[0] * p[0][0] + ac[1] * p[0][1];
		velrp[0] += fabs(dot) * ac[0];
		velrp[1] += fabs(dot) * ac[1];

		dot = (velrp[0]-velr[0]) * p[0][0] + (velrp[1]-velr[1]) * p[0][1];
		dv += dot;

		dot = p[0][0] * velrp[0] + p[0][1] * velrp[1];
		velrp[0] *= fabs(dot);
		velrp[1] *= fabs(dot);

		velrp[0] = velrp[0] - velrp[0] * srf;
		velrp[1] = velrp[1] - velrp[1] * srf;

		posr[0] += velrp[0];
		posr[1] += velrp[1];

		velr[0] = velrp[0];
		velr[1] = velrp[1];

		d = sqrtf(posr[0] * posr[0] + posr[1] * posr[1]);
		posr[0] *= sy / d;
		posr[1] *= sy / d;

		dld = sqrtf(velrp[0] * velrp[0] + velrp[1] * velrp[1]);

		if((int)((ld + dld)/lrot*4.0f) - (int)((ld) / lrot*4.0f) > 0)
		{
			ld += dld;

			printf("t=%f,posr=%f,%f,sumdv=%f,velx,y,l=%f,%f,%f,acxy,l=%f,%f,%f,sumL,r=%f,%f\r\n",
				t,
				posr[0], posr[1],
				dv,
				velr[0], velr[1], sqrtf(velr[0] * velr[0] + velr[1] * velr[1]),
				ac[0], ac[1], sqrtf(ac[0] * ac[0] + ac[1] * ac[1]),
				ld, ld / lrot * 2.0f * 3.14159f);
		}
		else
		{
			ld += dld;
		}
	}

	system("pause");

	return 0;
}


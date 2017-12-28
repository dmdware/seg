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

#define DT	0.001f

	printf("DT=%f s\r\n", DT);

	printf("enter length of rotation in meters eg 10: ");
	scanf("%f", &lrot);

	printf("enter straight resistance factor ratio eg 0.001: ");
	scanf("%f", &srf);

	posr[0] = 0;
	posr[1] = sy = lrot / (2.0f * 3.14159f);
	printf("starting position of rotator at t0 = %f,%f\r\n", posr[0], posr[1]);

	printf("enter position of attractor x eg 0: ");
	scanf("%f", &posa[0]);

	printf("enter position of attractor y eg -0.05: ");
	scanf("%f", &posa[1]);

	printf("enter acceleration of attractor at 1 m eg 0.1: ");
	scanf("%f", &aa1m);
	aa1m *= DT * DT;

	printf("enter starting velocity forward eg 1000: ");
	scanf("%f", &velr[0]);
	velr[0] *= -1.0f * DT;
	velr[1] = 0;

	ld = 0;

	t = 0;

	dvelr[0] = dvelr[1] = 0;
	dv = 0;

	printf("timestep adjusted velocity = %f,%f length %f\r\n", velr[0], velr[1], sqrtf(velr[0] * velr[0] + velr[1] * velr[1]));
	printf("timestep adjusted acceleration = %f\r\n", aa1m);

	while (ld < lrot * 100)
	{
		t += DT;

		dp[0] = posa[0] - posr[0];
		dp[1] = posa[1] - posr[1];
		d = sqrtf(dp[0] * dp[0] + dp[1] * dp[1]);
		printf("ac %f\r\n", d);

		ac[0] = dp[0] * aa1m / (d*d*d);
		ac[1] = dp[1] * aa1m / (d*d*d);
		printf("ac %f,%f\r\n", ac[0], ac[1]);

		d = sqrtf(velr[0] * velr[0] + velr[1] * velr[1]);
		printf("vel %f,%f=dist %f\r\n", velr[0], velr[1], d);

		p[0][0] = posr[0];
		p[0][1] = posr[1];
		ar = atan2(p[0][1], p[0][0]);
		printf("angle v = +%f rads (of %f vel around %f length) (at angle%f to %f)\r\n", 2.0f * 3.14159f * (d / lrot), d, lrot, ar, 2.0f * 3.14159f * (d / lrot) + ar);
		ar += 2.0f * 3.14159f * (d / lrot);
		p[1][0] = cos(ar) * fabs(sy);
		p[1][1] = sin(ar) * fabs(sy);
		printf("p%f,%f->DP by angle of length v dist to %f,%f\r\n", p[0][0], p[0][1], p[1][0], p[1][1]);

		p[0][0] = p[1][0] - p[0][0];
		p[0][1] = p[1][1] - p[0][1];
		d = sqrtf(p[0][0] * p[0][0] + p[0][1] * p[0][1]);
		p[0][0] /= d;
		p[0][1] /= d;
		printf("delta length= of angle vector%f, giving vector angle %f,%f for dot\r\n", d, p[0][0], p[0][1]);

		//dot = p[0][0] * velr[0] + p[0][1] * velr[1];
		//velrp[0] = velr[0] * fabs(dot);
		//velrp[1] = velr[1] * fabs(dot);

		//dot = ac[0] * p[0][0] + ac[1] * p[0][1];
		//velrp[0] += fabs(dot) * ac[0];
		//velrp[1] += fabs(dot) * ac[1];

//		velrp[0] = velr[0] - velr[0] * srf;
	//	velrp[1] = velr[1] - velr[1] * srf;
		//printf("vel rot prime %f,%f srf\r\n", velrp[0], velrp[1]);

		velrp[0] = velr[0] + ac[0];
		velrp[1] = velr[1] + ac[1];
		printf("vel rot prime %f,%f ac+\r\n", velrp[0], velrp[1]);

		velrp[0] = velrp[0] - velrp[0] * srf;
		velrp[1] = velrp[1] - velrp[1] * srf;
		printf("vel rot prime %f,%f srf\r\n", velrp[0], velrp[1]);

		dot = p[0][0] * velrp[0] + p[0][1] * velrp[1];
		//velrp[0] *= fabs(dot);
		//velrp[1] *= fabs(dot);
		d = sqrtf(velrp[0] * velrp[0] + velrp[1] * velrp[1]);
		velrp[0] = p[0][0] * d * dot;
		velrp[1] = p[0][1] * d * dot;
		printf("dot of vel p and rot ang circle %f, =velp %f,%f\r\n", dot, velrp[0], velrp[1]);
		d = sqrtf(velrp[0] * velrp[0] + velrp[1] * velrp[1]);

		//dot = (velrp[0] - velr[0]) * p[0][0] + (velrp[1] - velr[1]) * p[0][1];
		dv += (d - sqrtf(velr[0]*velr[0] + velr[1]*velr[1])) * fabs(dot)/dot ;
		printf("dv+ (%f-%f)*%f = %f, dv'=%f\r\n", d, sqrtf(velr[0] * velr[0] + velr[1] * velr[1]), fabs(dot) / dot,
			(d - sqrtf(velr[0] * velr[0] + velr[1] * velr[1])) * fabs(dot) / dot, dv);

		posr[0] += velrp[0];
		posr[1] += velrp[1];
		printf("p' = %f,%f vel+\r\n", posr[0], posr[1]);

		velr[0] = velrp[0];
		velr[1] = velrp[1];

		d = sqrtf(posr[0] * posr[0] + posr[1] * posr[1]);
		posr[0] *= fabs(sy) / d;
		posr[1] *= fabs(sy) / d;
		printf("p' = %f,%f    radius bounds %f->%f\r\n", posr[0], posr[1], d, fabs(sy));


		dld = sqrtf(velrp[0] * velrp[0] + velrp[1] * velrp[1]) * fabs(dot) / dot;
		printf("dist done + %f vel'+dir dot\r\n", dld);

		//if((int)((ld + dld)/lrot*40.0f) - (int)((ld) / lrot*40.0f) > 0)
		{
			ld += dld;

			printf("t=%f,posr=%f,%f,sumdv=%f,velx,y,l=%f,%f,%f,acxy,l=%f,%f,%f,sumL,r=%f,%f\r\n",
				t,
				posr[0], posr[1],
				dv,
				velr[0], velr[1], sqrtf(velr[0] * velr[0] + velr[1] * velr[1]),
				ac[0], ac[1], sqrtf(ac[0] * ac[0] + ac[1] * ac[1]),
				ld, ld / lrot * 2.0f * 3.14159f);

			system("pause");
		}
		//else
		{
			ld += dld;
		}
	}

	system("pause");

	return 0;
}


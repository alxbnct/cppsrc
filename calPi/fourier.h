#include"nr3.h"

void four1(Doub *data, const Int n, const Int isign) {
	//	Replaces data[0..2*n - 1] by its discrete Fourier transform, if isign is input as 1; or replaces
	//		data[0..2*n - 1] by n times its inverse discrete Fourier transform, if isign is input as 1. data
	//		is a complex array of length n stored as a real array of length 2 * n.n must be an integer power
	//		of 2.
	Int nn, mmax, m, j, istep, i;
	Doub wtemp, wr, wpr, wpi, wi, theta, tempr, tempi;
	if (n<2 || n&(n - 1)) throw("n must be power of 2 in four1");
	nn = n << 1;
	j = 1;
	for (i = 1; i<nn; i += 2) {
		//This is the bit - reversal section of the
		if (j > i) {
			//routine.
			SWAP(data[j - 1], data[i - 1]);// Exchange the two complex numbers.
			SWAP(data[j], data[i]);
		}
		m = n;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	//Here begins the Danielson - Lanczos section of the routine.
	mmax = 2;
	while (nn > mmax) {
		//Outer loop executed log2 n times.
		istep = mmax << 1;
		theta = isign*(6.28318530717959 / mmax); //Initialize the trigonometric recurrence.
		wtemp = sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 1; m<mmax; m += 2) {
			//Here are the two nested inner loops.
			for (i = m; i <= nn; i += istep) {
				j = i + mmax; //This is the Danielson - Lanczos for
				tempr =wr*data[j - 1] - wi*data[j]; //mula:
				tempi = wr*data[j] + wi*data[j - 1];
				data[j - 1] = data[i - 1] - tempr;
				data[j] = data[i] - tempi;
				data[i - 1] += tempr;
				data[i] += tempi;
			}
			wr = (wtemp = wr)*wpr - wi*wpi + wr; //Trigonometric recurrence.
			wi = wi*wpr + wtemp*wpi + wi;
		}
		mmax = istep;
	}
}




void four1(VecDoub_IO &data, const Int isign) {
	//fourier.h
	//	Overloaded interface to four1.Replaces the vector data, a complex vector of length N stored
	//	as a real vector of twice that length, by its discrete Fourier transform, with components in
	//	wraparound order, if isign is 1; or by N times the inverse Fourier transform, if isign is 1.
	four1(&data[0], data.size() / 2, isign);
}
void four1(VecComplex_IO &data, const Int isign) {
	//Overloaded interface to four1.Replaces the vector data, a complex vector of length N stored
	//	as such, by its discrete Fourier transform, with components in wraparound order, if isign is 1;
	//or by N times the inverse Fourier transform, if isign is 1.
	four1((Doub*)(&data[0]), data.size(), isign);
}




void realft(VecDoub_IO &data, const Int isign) {
//	fourier.h
/*		Calculates the Fourier transform of a set of n real - valued data points.Replaces these data
		(which are stored in array data[0..n - 1]) by the positive frequency half of their complex Fourier
		transform.The real - valued first and last components of the complex transform are returned
		as elements data[0] and data[1], respectively.n must be a power of 2. This routine also
		calculates the inverse transform of a complex data array if it is the transform of real data.
		(Result in this case must be multiplied by 2 / n.)
*/
		Int i, i1, i2, i3, i4, n = data.size();
	Doub c1 = 0.5, c2, h1r, h1i, h2r, h2i, wr, wi, wpr, wpi, wtemp;
	Doub theta = 3.141592653589793238 / Doub(n >> 1); //Initialize the recurrence.
	if (isign == 1) {
		c2 = -0.5;
		four1(data, 1); //The forward transform is here.
	}
	else {
		c2 = 0.5; //Otherwise set up for an inverse trans
		theta= -theta; //form.
	}
	wtemp = sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi = sin(theta);
	wr = 1.0 + wpr;
	wi = wpi; 
	for (i = 1; i<(n >> 2); i++) {
		//Case i = 0 done separately below.
			i2 = 1 + (i1 = i + i);
		i4 = 1 + (i3 = n - i1);
		h1r = c1*(data[i1] + data[i3]); //The two separate transforms are sep
		h1i =c1*(data[i2] - data[i4]); //arated out of data.
			h2r = -c2*(data[i2] + data[i4]);
		h2i = c2*(data[i1] - data[i3]);
		data[i1] = h1r + wr*h2r - wi*h2i; //Here they are recombined to form
			//the true transform of the original
			//real data.
			data[i2] = h1i + wr*h2i + wi*h2r;
		data[i3] = h1r - wr*h2r + wi*h2i;
		data[i4] = -h1i + wr*h2i + wi*h2r;
		wr = (wtemp = wr)*wpr - wi*wpi + wr; //The recurrence.
			wi = wi*wpr + wtemp*wpi + wi;
	}
	if (isign == 1) {
		data[0] = (h1r = data[0]) + data[1]; //Squeeze the first and last data together
		//	to get them all within the
		//	original array.
			data[1] = h1r - data[1];
	}
	else {
		data[0] = c1*((h1r = data[0]) + data[1]);
		data[1] = c1*(h1r - data[1]);
		four1(data, -1); //This is the inverse transform for the
	} //case isign = -1.
}



#include "TComplex.h"
#include <cmath>
#include <stdexcept>
#include "TComplex.h"


TComplex::TComplex(double real, double image) : re(real), im(image) {
}

TComplex::TComplex(const std::string& number) {
	try {
		size_t pos = number.find("+i*");
		int k = 1;
		if (pos == std::string::npos) {
			pos = number.find("-i*");
			k = -1;
		}
		if (pos == std::string::npos) {
			re = std::stod(number);
			im = 0;
		}
		else {
			re = std::stod(number.substr(0, pos));
			im = std::stod(number.substr(pos + 3)) * k;
		}
	}
	catch (...) {
		re = 0;
		im = 0;
	}
}

TComplex& TComplex::operator=(const TComplex& sec) = default;

TComplex TComplex::operator+(const TComplex & sec) {
	return TComplex(re + sec.re, im + sec.im);
}

TComplex TComplex::operator-(const TComplex & sec) {
	return TComplex(re - sec.re, im - sec.im);
}

TComplex TComplex::operator*(const TComplex & sec) {
	return TComplex(re * sec.re - im * sec.im, re * sec.im + im * sec.re);
}

TComplex TComplex::operator/(const TComplex & sec) {
	if (sec.re == 0 && sec.im == 0) {
		throw std::invalid_argument("Division by zero");
	}

	double denominator = sec.re * sec.re + sec.im * sec.im;

	return TComplex(
		(re * sec.re + im * sec.im) / denominator,
		(im * sec.re - re * sec.im) / denominator
	);
}

TComplex TComplex::operator-() {
	return TComplex(-re, -im);
}

bool TComplex::operator==(const TComplex & sec) {
	double eps = 0.00001;
	return fabs(re - sec.re) < eps && fabs(im - sec.im) < eps;
}

bool TComplex::operator!=(const TComplex & sec) {
	return !(*this == sec);
}

TComplex TComplex::sqr() {
	return *this * *this;
}

TComplex TComplex::inverse() {
	return TComplex(1) / *this;
}

double TComplex::module() {
	return sqrt(re * re + im * im);
}

double TComplex::radAngle() {
	switch (sign(re)) {
	case 1:
		return atan(im / re);
	case 0:
		switch (sign(im)) {
		case 1:
			return COMPL_PI / 2;
		case 0:
			throw std::invalid_argument("Number is zero!");
		case -1:
			return -COMPL_PI / 2;
		}
	case -1:
		return atan(im / re) + COMPL_PI;
	}
}

double TComplex::degAngle() {
	return radAngle() * 180 / COMPL_PI;
}

TComplex TComplex::pow(int exp) {
	double mod = std::pow(module(), exp);
	double arg = radAngle() * exp;

	return TComplex(mod * cos(arg), mod * sin(arg));
}

TComplex TComplex::root(int exp, int idx) {
	if (exp == 0) {
		throw std::invalid_argument("Division by zero");
	}
	if (idx >= exp) {
		throw std::invalid_argument("Index must be less than exponent");
	}

	double mod = std::pow(module(), 1.0 / exp);
	double arg = (radAngle() + 2 * COMPL_PI * idx) / exp;

	return TComplex(mod * cos(arg), mod * sin(arg));
}

double TComplex::getRealPart() {
	return re;
}

std::string TComplex::getRealPartAsString() {
	return std::to_string(re);
}

double TComplex::getImaginaryPart() {
	return im;
}

std::string TComplex::getImaginaryPartAsString() {
	return std::to_string(im);
}

std::string TComplex::toString() {
	std::string realPart = getRealPartAsString();
	std::string imaginaryPart;
	std::string signPart;

	switch (sign(im)) {
	case -1:
		signPart = "-i*";
		imaginaryPart = std::to_string(fabs(im));
		break;
	case 0:
		signPart = imaginaryPart = "";
		break;
	case 1:
		signPart = "+i*";
		imaginaryPart = getImaginaryPartAsString();
		break;
	}

	return realPart + signPart + imaginaryPart;
}

int TComplex::sign(double x) {
	if (x > 0) {
		return 1;
	}
	else {
		return x == 0 ? 0 : -1;
	}
}
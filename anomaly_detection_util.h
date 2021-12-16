#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_

/**
 * @brief Function to measure the average
 * of a given value
 * @param x
 * @param size
 * @return float
 */
float avg(float *x, int size);

/**
 * @brief returns the variance of X or Y
 * @param x
 * @param size
 * @return float
 */
float var(float *x, int size);

/**
 * @brief returns the covariance of X and Y
 * @param x
 * @param y
 * @param size
 * @return float
 */
float cov(float *x, float *y, int size);

/**
 * @brief returns the Pearson correlation coefficient of X and Y
 * @param x
 * @param y
 * @param size
 * @return float
 */
float pearson(float *x, float *y, int size);

class Line
{
public:
	const float a, b;
	Line(float a, float b) : a(a), b(b) {}
	float f(float x)
	{
		return a * x + b;
	}
};

class Point
{
public:
	const float x, y;
	Point(float x, float y) : x(x), y(y) {}
};

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size);

// returns the deviation between point p and the line
float dev(Point p, Line l);

#endif

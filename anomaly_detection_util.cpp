/**
 * Author: Ori Azualy
 * Implementaion for anomaly detection based on statisticall methods
 *
 **/

#include <cmath>
#include "anomaly_detection_util.h"

/**
 * @brief Function to measure the average
 * of a given value
 * @param x
 * @param size
 * @return float
 */
float avg(float *x, int size)
{
    float meanSum = 0.0;
    // for loop to sum
    for (int i = 0; i < size; i++)
    {
        meanSum += x[i];
    }
    return (meanSum / size);
}

/**
 * @brief returns the variance of X or Y
 * @param x
 * @param size
 * @return float
 */
float var(float *x, int size)
{
    float mean_pow = 0.0;
    for (int i = 0; i < size; i++)
    {
        mean_pow += pow(x[i], 2);
    }
    mean_pow = mean_pow / size;
    return mean_pow - pow(avg(x, size), 2);
}

/**
 * @brief returns the covariance of X and Y
 * @param x
 * @param y
 * @param size
 * @return float
 */
float cov(float *x, float *y, int size)
{
    float covSum = 0.0;
    // condition if the array's size are matching
    while ((sizeof(x) / sizeof(*x)) == (sizeof(y) / sizeof(*y)))
    {
        for (int i = 0; i < size; i++)
        {
            covSum += x[i] * y[i];
        }
        covSum = covSum / size;
        return covSum - (avg(x, size) * avg(y, size));
    }
    return 0;
}

/**
 * @brief returns the Pearson correlation coefficient of X and Y
 * @param x
 * @param y
 * @param size
 * @return float
 */
float pearson(float *x, float *y, int size)
{
    float cov_val = cov(x, y, size);
    return cov_val / (sqrt(var(x, size)) * sqrt(var(y, size)));
}

/**
 * @brief performs a linear regression and returns the line equation
 *
 * @param points
 * @param size
 * @return Line
 */
Line linear_reg(Point **points, int size)
{
    // create arrays of x values and y valuse of the points
    float x_from_p[size];
    float y_from_p[size];
    for (int i = 0; i < size; i++)
    {
        x_from_p[i] = points[i]->x;
        y_from_p[i] = points[i]->y;
    }
    float a = cov(x_from_p, y_from_p, size) / var(x_from_p, size);
    float b = avg(y_from_p, size) - (a * avg(x_from_p, size));
    return Line(a, b);
}

/**
 * @brief returns the deviation between point p and the line equation of the points
 *
 * @param p
 * @param points
 * @param size
 * @return float
 */
float dev(Point p, Point **points, int size)
{
    Line ln = linear_reg(points, size);
    return std::abs(ln.f(p.x) - p.y);
}

/**
 * @brief returns the deviation between point p and the line
 * @param p
 * @param l
 * @return float
 */
float dev(Point p, Line l)
{
    return std::abs(l.f(p.x) - p.y);
}

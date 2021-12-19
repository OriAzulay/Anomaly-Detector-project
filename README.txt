########## Anomaly Detector project ############

This project implements a simple Detector Anomaly based on simple statistical methods.

****** Anomaly_detection_utill *******
This is the first step for building anomaly detector.
On this step i implemented basic detector based on statistical methods:

- 'Var' method return the variance for a final sample of a discrete variable.
- 'Cov' method return the covariance for x & y
- 'Pearson' method is  a measure of linear correlation. The closer the value is to 1 the more the two variables behave
More similar (when one rises or falls the other rises or falls respectively).
The variables behave in the opposite direction (when one goes up, the other goes down). In both cases there is a very correlation
strong. In contrast, the closer the value is to 0 the greater the lack of connection in the bio variables.
- 'linear_reg' return the linear equation

******* TimeSeries ******************
Thie time series is a method that gets a CSV of flights data features as flight time, altitude, speed, and heading by degree
and keep the data with convenient data structure as a vector with column name as string key & column list as the value

******* Simple Anomaly Detection *********
This file is seperate into 2 :
The first part is by the method 'learnNormal' - a method that gets time series  - data of flight features
and she checks every 2 columns if there is a correlation anomaly between 2 features - a value greater than a given threshold.
if there is, it saves it into correlationFeature vector by the names of both features and columns, the correlation,
the threshold, center between both of the features points.
this part is a method for learning the threshold and values we want to get
The second part is to check a real anomaly detection - where we receive a real CSV file and we check if there is 
a correlation by the values we mentioned before.

******* minimum circle detection ***********

In the simple detector, we assumed that there are linear correlations between variables, and using linear regression we chose to predict
The expected value. When the observed value was different from the expected one we declared an exception.
However, not in every domain can we assume these assumptions. For example, as in the following diagrams.
In such cases, we would like to efficiently find the circle with the radius
The minimum that contains all the points (considered normal) (i.e. find the
The center of the circle and its radius.
Thus, given a new point we can measure its distance from the center of the circle, and if
It is larger than its radius so we will declare an exception

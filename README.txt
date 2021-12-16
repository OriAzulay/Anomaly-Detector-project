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
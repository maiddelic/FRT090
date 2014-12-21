/* Inverse transfer function of motor
*
* This class creates a filter for the inverse transfer function of a motor multiplied by a second order low pass filter,
* use the matlab code to get coefficients for the A and B polynomials, which is then used to filter the signal for the motor before
* sending it to the actual motors. Thus there will be a more rapid response by the motor than if only the signal, the step, would
* have been sent.
*
*	By Johan Malmberg, Maid Delic	 
*								2014 LTH, Department of Automatic Control, @ the Information- and Communication Engineering Program
*
*/
#pragma once

#define _TF_

ref class TF {
public:

	/**
	* Creates a transfer function for a motor. Instantiates the temporary variables that are used for updating the input value to the motor.
	* The update member function needs to be used to store the tranfer function parameters before it is used for running data thorugh it.
	*/
	TF();
	~TF();

	/**
	* Updates the variables for the transfer function of the system as the system runs, useful for tweaking the motors.
	* The transfer function is assumed to be of maximum second order, therefore high order polynomials are implementable!
	*
	*
	* @param a1		first parameter of denominator. Determines gain.
	* @param a2		second parameter of denominator
	* @param a3		third parameter of denominator
	* @param b1		first parameter of numerator
	* @param b2		second parameter of numerator
	* @param b3		third parameter of numerator
	* @return		void
	*/
	void updateVariables(double a1, double a2, double a3, double b1, double b2, double b3);

	/**
	* The output y1 is calculated by 
	*		a1*y1 + a2*y2 + a3*y3 = b1*u1 + b2*u2 + b3*u3
	*
	* @param signal		the signal, usually a step, that is to be filtered by the inverse of the
	*					transfer function of the motor before being sent to the motor.
	* @return			the filtered signal
	*/
	double filterSignal(double signal);

	/**
	* Flushes all temporary variables in the transfer function so the next step is not dependent on previous steps
	*
	* @return			void
	*/
	void flush();
	
private:
	double temp_u_1;
	double temp_u_2;
	double temp_u_3;
	double temp_y_1;
	double temp_y_2;
	double temp_y_3;

	double a1;
	double a2;
	double a3;
	double b1;
	double b2;
	double b3;

};
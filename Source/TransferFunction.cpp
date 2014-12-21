/** Inverse transfer function of motor
*
* This class creates a filter for the inverse transfer function of a motor multiplied by a second order low pass filter,
* use the matlab code to get coefficients for the A and B polynomials, which is then used to filter the signal for the motor before
* sending it to the actual motors. Thus there will be a more rapid response by the motor than if only the signal, the step, would
* have been sent.
*
*	By Johan Malmberg, Maid Delic, 2014 LTH, Department of Automatic Control, @ the Information- and Communication Engineering Program
*
*/

#include "stdafx.h"
#include "TransferFunction.h"

TF::TF(){
	temp_u_1 = 0;
	temp_u_2 = 0;
	temp_u_3 = 0;
	temp_y_1 = 0;
	temp_y_2 = 0;
	temp_y_3 = 0;
}


TF::~TF(){

}

void TF::updateVariables(double a1, double a2, double a3, double b1, double b2, double b3){
	this->a1 = a1;
	this->a2 = a2;
	this->a3 = a3;
	this->b1 = b1;
	this->b2 = b2;
	this->b3 = b3;
}

double TF::filterSignal(double signal){
	temp_u_1 = signal;
	temp_y_1 = ((-a2 * temp_y_2) - (a3 * temp_y_3) + (b1 * temp_u_1) + (b2 * temp_u_2) + (b3 * temp_u_3))/a1; 
	temp_u_3 = temp_u_2;
	temp_u_2 = temp_u_1;
	temp_y_3 = temp_y_2;
	temp_y_2 = temp_y_1;
	

	return temp_y_1;
}

void TF::flush(){
	temp_u_1 = 0;
	temp_u_2 = 0;
	temp_u_3 = 0;
	temp_y_1 = 0;
	temp_y_2 = 0;
	temp_y_3 = 0;
}

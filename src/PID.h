#ifndef HUMIDISTAT_PID_H
#define HUMIDISTAT_PID_H

#include <stdint.h>

/// PID controller in parallel form implemented in floating-point arithmetic. Features Derivative-on-Measurement and
/// anti-windup through conditional integration.
class PID {
private:
	const double &pv; //!< Process variable
	double &cv;       //!< Control variable
	const double &sp; //!< Setpoint

	const double Kp, Ki, Kd;   //!< Gains
	const uint16_t dt;         //!< Timestep
	const double cvMin, cvMax; //!< Lower/upper limits for cv

	bool inAuto = false;        //!< Mode
	unsigned long lastComputed; //!< Last time the loop was run
	double lastPv;              //!< Last value of pv
	double integral;            //!< Integral of pv

	/// Method to be called when the controller goes from manual to auto mode for proper bumpless transfer.
	void init();

	/// Clip value to [cvMin, cvMax].
	/// \param value Value to clip
	/// \return Clipped value
	double clip(double value) const;

public:
	double pTerm = 0, iTerm = 0, dTerm = 0; //!< PID terms

	/// Constructor.
	/// \param pv Pointer to process variable
	/// \param cv Pointer to control value
	/// \param sp Pointer to setpoint
	/// \param kp Proportional gain
	/// \param ki Integral gain (in 1/s)
	/// \param kd Derivative gain (in s)
	/// \param dt Timestep (in ms)
	/// \param cvMin Lower limit for control value
	/// \param cvMax Upper limit for control value
	PID(const double *pv, double *cv, const double *sp, double kp, double ki, double kd, uint16_t dt, double cvMin,
	    double cvMax);

	/// Run a cycle of the PID loop. Call this as often as you want in loop(), it handles timing internally.
	/// \return True if a PID step was run, and false if not.
	bool compute();

	/// Set the mode of the controller.
	/// \param inAuto Set to true for automatic, false for manual.
	void setAuto(bool inAuto);
};

#endif //HUMIDISTAT_PID_H

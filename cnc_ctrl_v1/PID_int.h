#ifndef PID_int_h
#define PID_int_h
#define LIBRARY_VERSION	1.2.1

class PID_int
{


  public:

  //Constants used in some of the functions below
  #define AUTOMATIC	1
  #define MANUAL	0
  #define DIRECT  0
  #define REVERSE  1
  #define P_ON_M 0
  #define P_ON_E 1
  bool pOnE = true, pOnM = false;
  unsigned long pOnEKp, pOnMKp;

  //commonly used functions **************************************************************************
    
  PID_int();
    
  void setup(volatile long*, volatile long*, volatile long*,        // * constructor.  links the PID to the Input, Output, and 
        const unsigned long&, const unsigned long&, const unsigned long&, const unsigned long&, const int&);//   Setpoint.  Initial tuning parameters are also set here.
                                          //   (overload for specifying proportional mode)

  PID_int(unsigned long*, unsigned long*, unsigned long*,        // * constructor.  links the PID to the Input, Output, and 
        unsigned long, unsigned long, unsigned long, int);     //   Setpoint.  Initial tuning parameters are also set here
	
  void SetMode(const int& Mode);               // * sets PID to either Manual (0) or Auto (non-0)

  bool Compute();                       // * performs the PID calculation.  it should be
                                          //   called every time loop() cycles. ON/OFF and
                                          //   calculation frequency can be set using SetMode
                                          //   SetSampleTime respectively

  void SetOutputLimits(const long&, const long&); //clamps the output to a specific range. 0-255 by default, but
										  //it's likely the user will want to change this depending on
										  //the application
	


  //available but not commonly used functions ********************************************************
  void SetTunings(const unsigned long&, const unsigned long&,       // * While most users will set the tunings once in the 
                    const unsigned long&);         	  //   constructor, this function gives the user the option
                                          //   of changing tunings during runtime for Adaptive control
  void SetTunings(const unsigned long&, const unsigned long&,       // * overload for specifying proportional mode
                    const unsigned long&, const unsigned long&);         	  

	void SetControllerDirection(const int&);	  // * Sets the Direction, or "Action" of the controller. DIRECT
										  //   means the output will increase when error is positive. REVERSE
										  //   means the opposite.  it's very unlikely that this will be needed
										  //   once it is set in the constructor.
  void SetSampleTime(const unsigned long&);              // * sets the frequency, in Milliseconds, with which 
                                          //   the PID calculation is performed.  default is 100
										  
                                          
  //Display functions ****************************************************************
	unsigned long GetKp();						  // These functions query the pid for interal values.
	unsigned long GetKi();						  //  they were created mainly for the pid front-end,
	unsigned long GetKd();						  // where it's important to know what is actually 
	int GetMode();						  //  inside the PID.
	int GetDirection();					  //
  long GetIterm();

  private:
	void Initialize();
	
	unsigned long dispKp;				// * we'll hold on to the tuning parameters in user-entered 
	unsigned long dispKi;				//   format for display purposes
	unsigned long dispKd;				//
    
	unsigned long kp;                  // * (P)roportional Tuning Parameter
  unsigned long ki;                  // * (I)ntegral Tuning Parameter
  unsigned long kd;                  // * (D)erivative Tuning Parameter

	int controllerDirection;

  volatile long *myInput;              // * Pointers to the Input, Output, and Setpoint variables
  volatile long *myOutput;             //   This creates a hard link between the variables and the 
  volatile long *mySetpoint;           //   PID, freeing the user from having to constantly tell us
                                  //   what these values are.  with pointers we'll just know.
			  
	unsigned long lastTime;
	long outputSum, lastInput;

	unsigned long SampleTime;
	long outMin, outMax;
	bool inAuto;
};
#endif


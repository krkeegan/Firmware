/*This file is part of the Maslow Control Software.

    The Maslow Control Software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Maslow Control Software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Maslow Control Software.  If not, see <http://www.gnu.org/licenses/>.
    
    Copyright 2014-2017 Bar Smith*/

/*
The Motor module imitates the behavior of the Arduino servo module. It allows a gear motor (or any electric motor)
to be a drop in replacement for a continuous rotation servo.

*/

#include "Arduino.h"
#include "MotorGearboxEncoder.h"

MotorGearboxEncoder::MotorGearboxEncoder(const int& pwmPin, const int& directionPin1, const int& directionPin2, const int& encoderPin1, const int& encoderPin2)
:
encoder(encoderPin1,encoderPin2)
{
    
    //initialize motor
    motor.setupMotor(pwmPin, directionPin1, directionPin2);
    motor.write(0);
    
    //initialize the PID
    _PIDController.setup(&_currentSpeed, &_pidOutput, &_targetSpeed, _Kp, _Ki, _Kd, P_ON_E, DIRECT);
    initializePID();
    
    
}

void  MotorGearboxEncoder::write(const float& speed){
    /*
    Command the motor to turn at the given speed. Should be RPM is PWM right now.
    */
    
    _targetSpeed = (speed * 1024);
    
}

void   MotorGearboxEncoder::initializePID(){
    //setup positive PID controller
    _PIDController.SetMode(AUTOMATIC);
    _PIDController.SetOutputLimits(-255000, 255000);
    _PIDController.SetSampleTime(10000);
}

void  MotorGearboxEncoder::computePID(){
    /*
    Recompute the speed control PID loop and command the motor to move.
    */
    _currentSpeed = computeSpeed();
    
    /*if (millis() < 8000){
        _targetSpeed = 0;
    }
    else if (millis() < 12000){
        _targetSpeed = 10;
    }
    else if (millis() < 18000){
         _targetSpeed = 0;
    }
    else if(millis() < 24000){
        _targetSpeed = float((millis() - 18000))/400.0;
    }
    else if (millis() < 32000){
        _targetSpeed = 0;
    }
    else if (millis() < 40000){
        _targetSpeed = 10;
    }
    else if (millis() < 48000){
        _targetSpeed = 0;
    }
    else if (millis() < 56000){
        _targetSpeed = -10;
    }
    else if (millis() < 64000){
        _targetSpeed = 0;
    }
    else if (millis() < 72000){
        _targetSpeed = 10;
    }
    else if (millis() < 80000){
        _targetSpeed = 0;
    }
    else{
        _targetSpeed = 0;
    }*/
    
    // Between these speeds the motor is incapable of turning and it only
    // causes the Iterm in the PID calculation to wind up
    if (abs(_targetSpeed) <= _minimumRPM) _targetSpeed = 0;

    _PIDController.Compute();
        
    /*if(_motorName[0] == 'R'){
        //Serial.print(_currentSpeed);
        //Serial.print(" ");
        Serial.println(_targetSpeed);
    }*/
    
    //motor.attach();
    motor.write(_pidOutput >> 10);
}

void  MotorGearboxEncoder::setPIDValues(float KpV, float KiV, float KdV){
    /*
    
    Set PID tuning values
    
    */
    
    _Kp = KpV * 1024;
    _Ki = KiV * 1024;
    _Kd = KdV * 1024;
    
    _PIDController.SetTunings(_Kp, _Ki, _Kd, P_ON_E);
}

String  MotorGearboxEncoder::getPIDString(){
    /*
    
    Get PID tuning values
    
    */
    String PIDString = "Kp=";
    return PIDString + (_Kp >> 10) + ",Ki=" + (_Ki >> 10) + ",Kd=" + (_Kd >> 10);
}

void MotorGearboxEncoder::setPIDAggressiveness(float aggressiveness){
    /*
    
    The setPIDAggressiveness() function sets the aggressiveness of the PID controller to
    compensate for a change in the load on the motor.
    
    */
    
    _PIDController.SetTunings(aggressiveness*_Kp, _Ki, _Kd, P_ON_E);
    
}

void MotorGearboxEncoder::setEncoderResolution(float resolution){
    /*
    
    Change the encoder resolution
    
    */
    
    _encoderStepsToRPMScaleFactor = 60000000.0/resolution; //6*10^7 us per minute divided by 8148 steps per revolution
    
}

long MotorGearboxEncoder::computeSpeed(){
    /*
    
    Returns the motors speed in RPM since the last time this function was called
    
    */
    unsigned long timeElapsed =  micros() - _lastTimeStamp;
    
    long distMoved   =  encoder.read() - _lastPosition;
    distMoved = distMoved << 10;
    
    //Compute the speed in RPM
    long RPM = (_encoderStepsToRPMScaleFactor*distMoved)/timeElapsed;
    
    //Store values for next time
    _lastTimeStamp = micros();
    _lastPosition  = encoder.read();
    
    return -1.0*RPM;
}

float MotorGearboxEncoder::_runningAverage(const int& newValue){
    /*
    
    Compute a running average from the number passed in.
    
    */
    
    int sum = newValue + _oldValue1 + _oldValue2 + _oldValue3 + _oldValue4 + _oldValue5 + _oldValue6 + _oldValue7 + _oldValue8 + _oldValue9 + _oldValue10;
    float runningAverage = (float(sum))/11.0;
    
    _oldValue10 = _oldValue9;
    _oldValue9 = _oldValue8;
    _oldValue8 = _oldValue7;
    _oldValue7 = _oldValue6;
    _oldValue6 = _oldValue5;
    _oldValue5 = _oldValue4;
    _oldValue4 = _oldValue3;
    _oldValue3 = _oldValue2;
    _oldValue2 = _oldValue1;
    _oldValue1 = newValue;
    
    return runningAverage;
}

void MotorGearboxEncoder::setName(const String& newName){
    /*
    Set the name for the object
    */
    _motorName = newName;
}

String MotorGearboxEncoder::name(){
    /*
    Get the name for the object
    */
    return _motorName;
}

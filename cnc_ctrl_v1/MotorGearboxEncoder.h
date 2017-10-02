    /*This file is part of the Makesmith Control Software.

    The Makesmith Control Software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Makesmith Control Software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Makesmith Control Software.  If not, see <http://www.gnu.org/licenses/>.
    
    Copyright 2014-2017 Bar Smith*/
    
    #ifndef MotorGearboxEncoder_h
    #define MotorGearboxEncoder_h

    #include "Arduino.h"
    #include "Encoder.h"
    #include "Motor.h"
    #include "PID_v1.h"
    #include "PID_int.h"
    
    class MotorGearboxEncoder{
        public:
            MotorGearboxEncoder(const int& pwmPin, const int& directionPin1, const int& directionPin2, const int& encoderPin1, const int& encoderPin2);
            Encoder    encoder;
            Motor      motor;
            long      computeSpeed();
            void       write(const float& speed);
            void       computePID();
            void       setName(const String& newName);
            String     name();
            void       initializePID();
            void       setPIDAggressiveness(float aggressiveness);
            void       setPIDValues(float KpV, float KiV, float KdV);
            void       setEncoderResolution(float resolution);
            String     getPIDString();
        private:
            long       _targetSpeed;
            long       _currentSpeed;
            unsigned long     _lastPosition;
            unsigned long     _lastTimeStamp;
            float      _runningAverage(const int& newValue);
            String     _motorName;
            long       _pidOutput;
            PID_int    _PIDController;
            unsigned long _Kp=0, _Ki=0, _Kd=0;
            float      _encoderStepsToRPMScaleFactor = 7364.0;   //6*10^7 us per minute divided by 8148 steps per revolution
            int        _oldValue1;
            int        _oldValue2;
            int        _oldValue3;
            int        _oldValue4;
            int        _oldValue5;
            int        _oldValue6;
            int        _oldValue7;
            int        _oldValue8;
            int        _oldValue9;
            int        _oldValue10;
            unsigned int _minimumRPM = 500;
    };

    #endif
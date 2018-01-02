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

// This file contains the functions for outgoing Serial responses

#include "Maslow.h"

void  reportStatusMessage(byte status_code){
    /*
    
    Sends confirmation protocol response for commands. For every incoming line,
    this method responds with an 'ok' for a successful command or an 'error:'
    to indicate some error event with the line or some critical system error during
    operation.
    
    Taken from Grbl http://github.com/grbl/grbl
    */
    if (status_code == 0) { // STATUS_OK
      Serial.println(F("ok"));
    } else {
      Serial.print(F("error: "));
      #ifdef REPORT_GUI_MODE
        Serial.println(status_code);
      #else
        switch(status_code) {
          // case STATUS_EXPECTED_COMMAND_LETTER=")); Serial.println(  // Serial.println(F("Expected command letter")); break;
          case STATUS_BAD_NUMBER_FORMAT:
          Serial.println(F("Bad number format")); break;
          case STATUS_INVALID_STATEMENT:
            Serial.println(F("Invalid statement")); break;
          case STATUS_OLD_SETTINGS:
            Serial.println(F("Please set $12, $13, $19, and $20 to load old position data.")); break;
          // case STATUS_NEGATIVE_VALUE:
          // Serial.println(F("Value < 0")); break;
          // case STATUS_SETTING_DISABLED:
          // Serial.println(F("Setting disabled")); break;
          // case STATUS_SETTING_STEP_PULSE_MIN:
          // Serial.println(F("Value < 3 usec")); break;
          case STATUS_SETTING_READ_FAIL:
            Serial.println(F("EEPROM read fail. Using default settings.")); break;
          case STATUS_IDLE_ERROR:
            Serial.println(F("Not idle")); break;
          case STATUS_ALARM_LOCK:
            Serial.println(F("Alarm lock, $X to unlock")); break;
          case STATUS_CRITICAL_LOCK:
            Serial.println(F("Critical lock, fix settings or recalibrate chains to unlock")); break;
          // case STATUS_SOFT_LIMIT_ERROR:
          // Serial.println(F("Homing not enabled")); break;
          // case STATUS_OVERFLOW:
          // Serial.println(F("Line overflow")); break;
          // #ifdef MAX_STEP_RATE_HZ
          //   case STATUS_MAX_STEP_RATE_EXCEEDED:
          //   Serial.println(F("Step rate > 30kHz")); break;
          // #endif
          // Common g-code parser errors.
          // case STATUS_GCODE_MODAL_GROUP_VIOLATION:
          // Serial.println(F("Modal group violation")); break;
          // case STATUS_GCODE_UNSUPPORTED_COMMAND:
          // Serial.println(F("Unsupported command")); break;
          // case STATUS_GCODE_UNDEFINED_FEED_RATE:
          // Serial.println(F("Undefined feed rate")); break;
          default:
            // Remaining g-code parser errors with error codes
            Serial.print(F("Invalid gcode ID:"));
            Serial.println(status_code); // Print error code for user reference
        }
      #endif
    }
}

void reportFeedbackMessage(byte message_code){
  Serial.print(F("Message: "));
  switch(message_code) {
    // case MESSAGE_CRITICAL_EVENT:
    //   Serial.print(F("Reset to continue")); break;
    // case MESSAGE_ALARM_LOCK:
    //   Serial.print(F("'$H'|'$X' to unlock")); break;
    case MESSAGE_ALARM_UNLOCK:
      Serial.print(F("Caution: Alarm Unlocked")); break;
    // case MESSAGE_ENABLED:
    //   Serial.print(F("Enabled")); break;
    // case MESSAGE_DISABLED:
    //   Serial.print(F("Disabled")); break;
    // case MESSAGE_SAFETY_DOOR_AJAR:
    //   Serial.print(F("Check Door")); break;
    // case MESSAGE_CHECK_LIMITS:
    //   Serial.print(F("Check Limits")); break;
    // case MESSAGE_PROGRAM_END:
    //   Serial.print(F("Pgm End")); break;
    case MESSAGE_RESTORE_DEFAULTS:
      Serial.print(F("Restoring default settings")); break;
    // case MESSAGE_SPINDLE_RESTORE:
    //   Serial.print(F("Restoring spindle")); break;
    // case MESSAGE_SLEEP_MODE:
    //   Serial.print(F("Sleeping")); break;
    case MESSAGE_CHAIN_LENGTH_ERROR:
      Serial.print(F("Unable to find valid machine position for chain lengths"));
      Serial.print(leftAxis.read());
      Serial.print(", ");
      Serial.print(rightAxis.read());
      Serial.println(F(" . Please calibrate chain lengths or check settings."));
      break;
  }
  Serial.println(F(" "));
}

// Prints alarm messages.
void  reportAlarmMessage(byte alarm_code) {
  Serial.print(F("ALARM: "));
  #ifdef REPORT_GUI_MODE
    Serial.println(alarm_code);
  #else
    switch (alarm_code) {
      case EXEC_ALARM_POSITION_LOST:
        Serial.println(F("Position Lost, check settings or recalibrate chain lengths")); break;
      case EXEC_ALARM_SOFT_LIMIT:
        Serial.println(F("Sled is outside the bounds of the limits, use jogging to return and $x to unlock.")); break;
    }
  #endif
}

// Maslow global settings print out.
// NOTE: The numbering scheme here must correlate to storing in settings.c
void reportMaslowSettings() {
  // Print Maslow settings.
  // Taken from Grbl. http://github.com/grbl/grbl
  #ifdef REPORT_GUI_MODE
    Serial.print(F("$0=")); Serial.println(sysSettings.machineWidth);
    Serial.print(F("$1=")); Serial.println(sysSettings.machineHeight);
    Serial.print(F("$2=")); Serial.println(sysSettings.distBetweenMotors);
    Serial.print(F("$3=")); Serial.println(sysSettings.motorOffsetY);
    Serial.print(F("$4=")); Serial.println(sysSettings.sledWidth);
    Serial.print(F("$5=")); Serial.println(sysSettings.sledHeight);
    Serial.print(F("$6=")); Serial.println(sysSettings.sledCG);
    Serial.print(F("$7=")); Serial.println(sysSettings.kinematicsType);
    Serial.print(F("$8=")); Serial.println(sysSettings.rotationDiskRadius);
    Serial.print(F("$9=")); Serial.println(sysSettings.axisDetachTime);
    Serial.print(F("$11=")); Serial.println(sysSettings.originalChainLength);
    Serial.print(F("$12=")); Serial.println(sysSettings.encoderSteps);
    Serial.print(F("$13=")); Serial.println(sysSettings.distPerRot);
    Serial.print(F("$15=")); Serial.println(sysSettings.maxFeed);
    Serial.print(F("$16=")); Serial.println(sysSettings.zAxisAttached);
    Serial.print(F("$17=")); Serial.println(sysSettings.zAxisAuto);
    Serial.print(F("$18=")); Serial.println(sysSettings.maxZRPM);
    Serial.print(F("$19=")); Serial.println(sysSettings.zDistPerRot);
    Serial.print(F("$20=")); Serial.println(sysSettings.zEncoderSteps);
    Serial.print(F("$21=")); Serial.println(sysSettings.KpPos);
    Serial.print(F("$22=")); Serial.println(sysSettings.KiPos);
    Serial.print(F("$23=")); Serial.println(sysSettings.KdPos);
    Serial.print(F("$24=")); Serial.println(sysSettings.propWeightPos);
    Serial.print(F("$25=")); Serial.println(sysSettings.KpV);
    Serial.print(F("$26=")); Serial.println(sysSettings.KiV);
    Serial.print(F("$27=")); Serial.println(sysSettings.KdV);
    Serial.print(F("$28=")); Serial.println(sysSettings.propWeightV);
    Serial.print(F("$29=")); Serial.println(sysSettings.zKpPos);
    Serial.print(F("$30=")); Serial.println(sysSettings.zKiPos);
    Serial.print(F("$31=")); Serial.println(sysSettings.zKdPos);
    Serial.print(F("$32=")); Serial.println(sysSettings.zPropWeightPos);
    Serial.print(F("$33=")); Serial.println(sysSettings.zKpV);
    Serial.print(F("$34=")); Serial.println(sysSettings.zKiV);
    Serial.print(F("$35=")); Serial.println(sysSettings.zKdV);
    Serial.print(F("$36=")); Serial.println(sysSettings.zPropWeightV);
  #else
    Serial.print(F("$0=")); Serial.print(sysSettings.machineWidth);
    Serial.print(F(" (machine width, mm, $K)\r\n$1=")); Serial.print(sysSettings.machineHeight);
    Serial.print(F(" (machine height, mm, $K)\r\n$2=")); Serial.print(sysSettings.distBetweenMotors);
    Serial.print(F(" (motor distance, mm, $K)\r\n$3=")); Serial.print(sysSettings.motorOffsetY);
    Serial.print(F(" (motor height, mm, $K)\r\n$4=")); Serial.print(sysSettings.sledWidth);
    Serial.print(F(" (sled width, mm, $K)\r\n$5=")); Serial.print(sysSettings.sledHeight);
    Serial.print(F(" (sled height, mm, $K)\r\n$6=")); Serial.print(sysSettings.sledCG);
    Serial.print(F(" (sled cg, mm, $K)\r\n$7=")); Serial.print(sysSettings.kinematicsType);
    Serial.print(F(" (Kinematics Type 1=Quadrilateral, 2=Triangular, $K)\r\n$8=")); Serial.print(sysSettings.rotationDiskRadius);
    Serial.print(F(" (rotation radius, mm, $K)\r\n$9=")); Serial.print(sysSettings.axisDetachTime);
    Serial.print(F(" (axis idle before detach, ms)\r\n$11=")); Serial.print(sysSettings.originalChainLength);
    Serial.print(F(" (original chain length, mm)\r\n$12=")); Serial.print(sysSettings.encoderSteps);
    Serial.print(F(" (main steps per revolution, $K)\r\n$13=")); Serial.print(sysSettings.distPerRot);
    Serial.print(F(" (distance / rotation, mm, $K)\r\n$15=")); Serial.print(sysSettings.maxFeed);
    Serial.print(F(" (max feed, mm/min, $K)\r\n$16=")); Serial.print(sysSettings.zAxisAttached);
    Serial.print(F(" (Auto Z Axis, 1 = Yes)\r\n$17=")); Serial.print(sysSettings.zAxisAuto);
    Serial.print(F(" (auto z axis)\r\n$18=")); Serial.print(sysSettings.maxZRPM);
    Serial.print(F(" (max z axis RPM)\r\n$19=")); Serial.print(sysSettings.zDistPerRot);
    Serial.print(F(" (z axis distance / rotation)\r\n$20=")); Serial.print(sysSettings.zEncoderSteps);
    Serial.print(F(" (z axis steps per revolution)\r\n$21=")); Serial.print(sysSettings.KpPos);
    Serial.print(F(" (main Kp Pos)\r\n$22=")); Serial.print(sysSettings.KiPos);
    Serial.print(F(" (main Ki Pos)\r\n$23=")); Serial.print(sysSettings.KdPos);
    Serial.print(F(" (main Kd Pos)\r\n$24=")); Serial.print(sysSettings.propWeightPos);
    Serial.print(F(" (main Pos proportional weight)\r\n$25=")); Serial.print(sysSettings.KpV);
    Serial.print(F(" (main Kp Velocity)\r\n$26=")); Serial.print(sysSettings.KiV);
    Serial.print(F(" (main Ki Velocity)\r\n$27=")); Serial.print(sysSettings.KdV);
    Serial.print(F(" (main Kd Velocity)\r\n$28=")); Serial.print(sysSettings.propWeightV);
    Serial.print(F(" (main Velocity proportional weight)\r\n$29=")); Serial.print(sysSettings.zKpPos);
    Serial.print(F(" (z axis Kp Pos)\r\n$30=")); Serial.print(sysSettings.zKiPos);
    Serial.print(F(" (z axis Ki Pos)\r\n$31=")); Serial.print(sysSettings.zKdPos);
    Serial.print(F(" (z axis Kd Pos)\r\n$32=")); Serial.print(sysSettings.zPropWeightPos);
    Serial.print(F(" (z axis Pos proportional weight)\r\n$33=")); Serial.print(sysSettings.zKpV);
    Serial.print(F(" (z axis Kp Velocity)\r\n$34=")); Serial.print(sysSettings.zKiV);
    Serial.print(F(" (z axis Ki Velocity)\r\n$35=")); Serial.print(sysSettings.zKdV);
    Serial.print(F(" (z axis Kd Velocity)\r\n$36=")); Serial.print(sysSettings.zPropWeightV);
    Serial.println(F(" (z axis Velocity proportional weight)"));
  #endif
}

void  returnError(){
    /*
    Prints the machine's positional error and the amount of space available in the 
    gcode buffer
    */
        Serial.print(F("[PE:"));
        Serial.print(leftAxis.error());
        Serial.print(',');
        Serial.print(rightAxis.error());
        Serial.print(',');
        Serial.print(incSerialBuffer.spaceAvailable());
        Serial.println(F("]"));
}

void  returnPoz(){
    /*
    Causes the machine's position (x,y) to be sent over the serial connection updated on the UI
    in Ground Control. Also causes the error report to be sent. Only executes 
    if hasn't been called in at least POSITIONTIMEOUT ms.
    */
    
    static unsigned long lastRan = millis();
    
    if (millis() - lastRan > POSITIONTIMEOUT){
        
        Serial.print(F("<"));
        switch (sys.state) {
          case STATE_IDLE: Serial.print(F("Idle")); break;
          case STATE_CYCLE: Serial.print(F("Run")); break;
          // case STATE_HOLD:
          //   if (!(sys.suspend & SUSPEND_JOG_CANCEL)) {
          //     Serial.print(F("Hold:"));
          //     if (sys.suspend & SUSPEND_HOLD_COMPLETE) { serial_write('0'); } // Ready to resume
          //     else { serial_write('1'); } // Actively holding
          //     break;
          //   } // Continues to print jog state during jog cancel.
          // case STATE_JOG: Serial.print(F("Jog")); break;
          // case STATE_HOMING: Serial.print(F("Home")); break;
          case STATE_ALARM: Serial.print(F("Alarm")); break;
          // case STATE_CHECK_MODE: Serial.print(F("Check")); break;
          case STATE_CRITICAL: Serial.print(F("Critical")); break;
          // case STATE_SLEEP: Serial.print(F("Sleep")); break;
        }
        Serial.print(F(",MPos:"));
        Serial.print(sys.xPosition/sys.inchesToMMConversion);
        Serial.print(F(","));
        Serial.print(sys.yPosition/sys.inchesToMMConversion);
        Serial.print(F(","));
        Serial.print(zAxis.read()/sys.inchesToMMConversion);
        Serial.println(F(",WPos:0.000,0.000,0.000>"));
        
        returnError();
        
        lastRan = millis();
    }
    
}

void  reportMaslowHelp(){
    /*
    This function outputs a brief summary of the $ system commands available.
    The list is somewhat aspirational based on what Grbl offers. Maslow
    does not currently support all of these features.

    This is taken heavily from grbl.  https://github.com/grbl/grbl
    */
    #ifndef REPORT_GUI_MODE
        Serial.println(F("$$ (view Maslow settings)"));
        // Serial.println(F("$# (view # parameters)"));
        // Serial.println(F("$G (view parser state)"));
        // Serial.println(F("$I (view build info)"));
        // Serial.println(F("$N (view startup blocks)"));
        Serial.println(F("$x=value (save Maslow setting)"));
        Serial.println(F("$K (recompute kinematics)"));
        // Serial.println(F("$Nx=line (save startup block)"));
        // Serial.println(F("$C (check gcode mode)"));
        // Serial.println(F("$X (kill alarm lock)"));
        // Serial.println(F("$H (run homing cycle)"));
        Serial.println(F("~ (cycle start)"));  // Maslow treats this as resume or un-pause currently
        Serial.println(F("! (feed hold)"));    // Maslow treats this as a cycle stop.
        // Serial.println(F("? (current status)"));
        // Serial.println(F("ctrl-x (reset Maslow)"));
    #endif
}
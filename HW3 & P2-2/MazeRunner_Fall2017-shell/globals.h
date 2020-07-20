/* Gatech ECE2035 2015 SPRING PAC MAN
 * Copyright (c) 2015 Gatech ECE2035
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef GLOBAL_H
#define GLOBAL_H

// This is a hack because the library for the screen is bad.
#ifndef ULCD_4DGL_H_
#define ULCD_4DGL_H_
#include "uLCD_4DGL.h"
#endif

// Includes for hardware support
#include "wave_player.h"
#include "SDFileSystem.h"
#include "MMA8452.h"

// Hardware declarations. Initialization is in main.cpp
extern uLCD_4DGL uLCD;      // Screen
extern MMA8452 acc;         // Accelerometer
extern DigitalIn left_pb;   // push button
extern DigitalIn right_pb;  // push button
extern DigitalIn up_pb;     // push button
extern DigitalIn down_pb;   // push button
extern Serial pc;           // Serial output to PC
extern AnalogOut DACout;    // speaker
extern wave_player waver;   // wav player
extern SDFileSystem sd;     // SD card and filesystem (mosi, miso, sck, cs)
extern PwmOut speaker;      // Speaker

// Global parameters. Initialized in main.cpp
extern const float mass;
extern const int radius;
extern const float bounce;

#endif //GLOBAL_H
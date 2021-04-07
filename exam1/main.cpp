#include "mbed.h"
#include "uLCD_4DGL.h" // for uLCD
#include "mbed_events.h"
#include <iostream>
#include <thread> // for thread
#include <ctime> // for clock

using namespace std::chrono; // to set nanosecond

Timer t;
Thread changeThread;
Thread thread; // the thread for sample

EventQueue queue(32 * EVENTS_EVENT_SIZE);

InterruptIn upbn(D13);
InterruptIn downbn(D11);
InterruptIn selectbn(D12);
InterruptIn samplebn(USER_BUTTON);

uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(PA_4);
AnalogIn ain(PC_0);


float freqlist[4] = {0.125, 0.250, 0.500, 1.000};
float freq = freqlist[0];
int pos = 0;


// setting
double T = 1/freq;
int range = (int)(800*freq);
int flat = 2400 - 2*range;
double amp = 3/3.3;
double rangef = (double)range*amp;

float ADCdata[500]; // record sample data

// ADC
void sample(void)
{
    for(int i = 0; i < 500; i++)
    {
        ADCdata[i] = aout;
        ThisThread::sleep_for(4ms);
    }
    for(int i = 0; i < 500; i++)
    {
        printf("%f\r\n", ADCdata[i]);
    }
}

// show up
void U()
{
    if(pos!=3) pos++;
    uLCD.cls();
    uLCD.color(GREEN);
    uLCD.locate(0,0);
    uLCD.printf("%-6.3f\n", freqlist[pos]);
    ThisThread::sleep_for(100ms);
}

void DN()
{
    if(pos != 0) pos--;
    uLCD.cls();
    uLCD.color(GREEN);
    uLCD.locate(0,0);
    uLCD.printf("%-6.3f\n", freqlist[pos]);
    ThisThread::sleep_for(100ms);
}

void ST()
{
    freq = freqlist[pos];
    uLCD.cls();
    uLCD.color(RED);
    uLCD.locate(0,0);
    uLCD.printf("%-6.3f\n", freqlist[pos]);
    ThisThread::sleep_for(100ms);
    range = (int)(800*freq);
    flat = 2400-2*range;
    rangef = (double)range*amp;
}


int main()
{    
    // uLCD first show
    uLCD.color(RED);
    uLCD.locate(0,0);
    uLCD.printf("%-6.3f\n", freqlist[pos]);
    ThisThread::sleep_for(100ms);

    // start queue thread
    changeThread.start(callback(&queue, &EventQueue::dispatch_forever));

    //Interrupt
    upbn.rise(queue.event(&U));
    downbn.rise(queue.event(&DN));
    selectbn.rise(queue.event(&ST));
    samplebn.rise(queue.event(&sample));
    
    // DAC
    // main loop
    while(1)
    {
        for(int i = 0; i < range; i++)
        {
            aout = i/rangef;
            wait_us(40);
        }
        for(int count = 0; count < flat; count++)
        {
            aout = range/rangef;
            wait_us(40);
        }
        for(int i = range; i > 0; i--)
        {
            aout = i/rangef;
            wait_us(40);
        }
    }
    
}
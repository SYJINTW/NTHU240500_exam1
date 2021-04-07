#include "mbed.h"
#include "uLCD_4DGL.h" // for uLCD
#include "mbed_events.h"
#include <iostream>
#include <thread> // for thread
#include <ctime> // for clock

using namespace std::chrono; // to set nanosecondsd

Timer t;
Thread changeThread;
Thread thread; // the thread for sample

EventQueue queue(32 * EVENTS_EVENT_SIZE);

InterruptIn upbn(D13);
InterruptIn downbn(D11);
InterruptIn selectbn(D12);

uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(PA_4);
AnalogIn ain(PC_0);


float freqlist[4] = {0.125, 0.250, 0.500, 1.000};
float freq = freqlist[0];
int pos = 0;


// setting
double T = 1/freq;
int range = (int)(8000*freq);
int flat = 24000 - 2*range;
double amp = 3/3.3;
double waitTime[4] = {1,2,3,4};
    

//float ADCdata[500]; // record sample data


/*
void sample(void)
{
    //t.start();
    for(int i = 0; i < 500; i++)
    {
        ADCdata[i] = ain;
        ThisThread::sleep_for(2000ms/500);
    }
    //t.stop();
    for(int i = 0; i < 500; i++)
    {
        printf("%f\r\n", ADCdata[i]);
    }
    //auto s = chrono::duration_cast<chrono::seconds>(t.elapsed_time()).count();
    //printf("time: %llus\n", s); // -> 3 sec & 500
}
*/

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
    range = (int)(8000*freq);
    flat = 24000-2*range;
}


int main()
{    
    // check if the button works
    /*
    while(1)
    {
        printf("up has value : %d \n\r", upbn.read());
        printf("down has value : %d \n\r", downbn.read());
        printf("select has value : %d \n\r", selectbn.read());
        ThisThread::sleep_for(250ms);
    }
    */

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
    
    // DAC
    
    // start thread for sample
    // thread.start(sample);

    // main loop
    while(1)
    {
        for(int i = 0; i < range; i++)
        {
            aout = i/(float)range*amp;
            wait_us(10);
        }
        for(int count = 0; count < flat; count++)
        {
            aout = range/(float)range*amp;
            wait_us(10);
        }
        for(int i = range; i > 0; i--)
        {
            aout = i/(float)range*amp;
            wait_us(10);
        }
        
        //auto s = chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
        //printf("time: %llus\n", s); // -> 3 sec & 500
        //ThisThread::sleep_for(5s);

    }
    
}
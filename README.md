<!-- ABOUT THE PROJECT -->
## About The Project

Exam 1

### Built With

* C++
* Python

### Equipment List
* PC or notebook
* B_L4S5I_IOT01A
* Bread board
* Buttons * 3
* Picoscope
* uLCD display
* Wires * 20
* Resistor and Capacitor

## Homework Description


<!-- GETTING STARTED -->
## Getting Started

### Running & compile

* Embedding in B_L4S5I_IOT01A
  
    ```sh
    sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM -f
    ```
* To check the output of ADC by screen
    
    ```sh
    sudo screen /dev/ttyACM0
    ```
* Run python FFT.py to analysis the wave
  
    ```sh
    sudo python3.8 FFT.py
    ```


<!-- ROADMAP -->
## Roadmap

1. Create GUI for choosing the which freqency to generate. And sleep 100ms to refreshing the output on uLCD.

    ```C++
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
        rangef = (float)range*amp;
    }

    // uLCD first show
    uLCD.color(RED);
    uLCD.locate(0,0);
    uLCD.printf("%-6.3f\n", freqlist[pos]);
    ThisThread::sleep_for(100ms);

    ```

    * GUI may look like the image below.
         


1. DAC

    ```C++
    // setting
    double T = 1/freq;
    int range = (int)(8000*freq);
    int flat = 24000 - 2*range;
    double amp = 3/3.3;
    double rangef = (float)range*amp;

    
    while(1)
    {
        for(int i = 0; i < range; i++)
        {
            aout = i/rangef;
            wait_us(2);
        }
        for(int count = 0; count < flat; count++)
        {
            aout = range/rangef;
            wait_us(2);
        }
        for(int i = range; i > 0; i--)
        {
            aout = i/rangef;
            wait_us(2);
        }
    }
    ```

1. Use Timer to calculate the time of the loop, and use `duration_cast<chrono::seconds>(t.elapsed_time()).count()` to get the time of the loop, the time will be `3 seconds`.
So that we know the freqency of sample is `500/3=16.66667Hz`
    ```C++
    // in main function
    ...
    thread.start(sample);
    ...

    // sample function define
    void sample(void)
    {
        t.start();
        for(int i = 0; i < 500; i++)
        {
            ADCdata[i] = aout;
            ThisThread::sleep_for(2ms); // let it finish in 1sec
        }
        //t.stop();
        for(int i = 0; i < 500; i++)
        {
            printf("%f\r\n", ADCdata[i]);
        }
        auto s = chrono::duration_cast<chrono::seconds>(t.elapsed_time()).count();
        printf("time: %llus\n", s); // -> 3 sec & 500
    }
    ```
    
1. In FFT.py, we must change the sampling rate to the loop frequency in sample function.
    
    ```python
    Fs = 1000.000
    ```

<!-- Screenshot -->
## Results

* Picoscope result at 1 Hz
  
    <img src="https://github.com/SYJINTW/NTHU240500_hw2/blob/master/hw2/src/wv_1Hz.png?raw=true">  

* Python Matplotlib at 1 Hz

    <img src="https://github.com/SYJINTW/NTHU240500_hw2/blob/master/hw2/src/py_1Hz.png?raw=true">

* Picoscope result at 82.6 Hz
    
    <img src="https://github.com/SYJINTW/NTHU240500_hw2/blob/master/hw2/src/wv_82.6Hz.png?raw=true">  

* Python Matplotlib at 82.6 Hz
    
    <img src="https://github.com/SYJINTW/NTHU240500_hw2/blob/master/hw2/src/py_82.6Hz.png?raw=true">

* Picoscope result at 500 Hz

    <img src="https://github.com/SYJINTW/NTHU240500_hw2/blob/master/hw2/src/wv_500Hz.png?raw=true">  

* Python Matplotlib at 500 Hz

    <img src="https://github.com/SYJINTW/NTHU240500_hw2/blob/master/hw2/src/py_500Hz.png?raw=true">

* Picoscope result at 826.3 Hz (cut-off frequency)
  
    <img src="https://github.com/SYJINTW/NTHU240500_hw2/blob/master/hw2/src/wv_826Hz.png?raw=true"> 

<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements

* [Electronics Tutorials](https://www.electronics-tutorials.ws/filter/filter_2.html)
* [Embedded System Lab](https://www.ee.nthu.edu.tw/ee240500/)


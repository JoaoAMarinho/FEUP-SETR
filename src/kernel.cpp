#include "kernel.h"

const int NT = 3;
const int TICK_RATE = 200000; // 0.2 seconds

// Scheduler tasks variables
SchedTask tasks[NT];
int currTask = NT;

// AlphaBot instance
AlphaBot *alphaBot;
Remote *remote;
Voice *voice;
Sensor *sensor;

// Program execution
bool *run;

// Commands buffers
Command motorBuffer = none;
Command voiceBuffer = none;
Command sensorBuffer = none;
Command remoteBuffer = none; 

SchedTask::SchedTask() : func(NULL), delay(0), period(0), exec(0) {} // Default constructor
SchedTask::SchedTask(void (*func)(), int delay, int period) : func(func), delay(delay), period(period), exec(0) {}

void schedSchedule()
{
    for (int x = 0; x < NT; x++)
    {
        if (!tasks[x].func)
            continue;
        if (tasks[x].delay > 0)
        {
            tasks[x].delay--;
        }
        else
        {
            tasks[x].exec = 1;
            tasks[x].delay = tasks[x].period - 1;
        }
    }
}

void schedDispatch()
{
    int prev_task = currTask;
    std::chrono::steady_clock::time_point begin, end;
    for (int x = 0; x < currTask; x++)
    {
        if (tasks[x].exec && tasks[x].func)
        {
            tasks[x].exec = 0;
            currTask = x;
            enableInterrupts();
            begin = std::chrono::steady_clock::now();
            tasks[x].func();
            end = std::chrono::steady_clock::now();
            std::cout << "Task " << x+1 << " execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
            disableInterrupts();
            currTask = prev_task;
            /*Delete if one-shot */
            if (!tasks[x].period)
                tasks[x].func = NULL;
        }
    }
}

void timerInterruptHandler(int signal)
{
    // std::cout << "Tick!\n";
    disableInterrupts();
    schedSchedule();
    schedDispatch();
    enableInterrupts();
}

int schedAddTask(void (*func)(), int delay, int period)
{
    if (func == nullptr)
        return -1;
    for (int x = 0; x < NT; x++)
    {
        if (!tasks[x].func)
        {
            SchedTask task(func, delay, period);
            tasks[x] = task;
            return x;
        }
    }
    return -1;
}

void setupTasks(AlphaBot *ab, Remote *rm, Voice *vc, Sensor *ss)
{
    alphaBot = ab;
    remote = rm;
    voice = vc;
    sensor = ss;

    // Add tasks
    if (schedAddTask(&motorTask, 1, 2) == -1)
    {
        std::cout << "Error adding motor task";
    };
    if (schedAddTask(&sensorControlTask, 2, 3) == -1)
    {
        std::cout << "Error adding sensor task";
    };
    if (schedAddTask(&voiceControlTask, 3, 3) == -1)
    {
        std::cout << "Error adding voice task";
    };
    // if (schedAddTask(&remoteControlTask, 0, 3) == -1)
    // {
    //     std::cout << "Error adding remote task";
    // };
}

void enableInterrupts()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timerInterruptHandler;
    sigaction(SIGALRM, &sa, NULL);
}

void disableInterrupts()
{
    signal(SIGALRM, SIG_IGN);
}

void closeHandler(int signal)
{
    *run = false;
}

void schedInit(bool *val)
{
    struct sigaction sa;
    struct itimerval timer;

    run = val;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timerInterruptHandler;
    sigaction(SIGALRM, &sa, NULL);

    // Ent program ctrl + c
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &closeHandler;
    sigaction(SIGINT, &sa, NULL);

    getitimer(ITIMER_REAL, &timer);

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = TICK_RATE;

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = TICK_RATE;

    setitimer(ITIMER_REAL, &timer, NULL);
}

void motorTask()
{
    if (voiceBuffer != none)
    {
        motorBuffer = voiceBuffer;
        voiceBuffer = none;
    }
    if (remoteBuffer != none)
    {
        motorBuffer = remoteBuffer;
        remoteBuffer = none;
    }
    if (sensorBuffer != none)
    {
        motorBuffer = sensorBuffer;
        sensorBuffer = none;
    }

    switch (motorBuffer)
    {
    case forward:
    {
        std::cout << "forward\n";
        (*alphaBot).forward();
        break;
    }
    case stop:
    {
        std::cout << "stop\n";
        (*alphaBot).stop();
        break;
    }
    case right:
    {
        std::cout << "right\n";
        (*alphaBot).right();
        break;
    }
    case left:
    {
        std::cout << "left\n";
        (*alphaBot).left();
        break;
    }
    case backward:
    {
        std::cout << "backward\n";
        (*alphaBot).backward();
        break;
    }
    case none:
    {
        // std::cout << "none\n";
        break;
    }
    }
    motorBuffer = none;
}

void sensorControlTask()
{
    std::string value = (*sensor).sensorCommand();
    std::cout << "Sensor" << std::endl;

    if (value == "stop")
    {
        sensorBuffer = stop;
    }
    else
    {
        sensorBuffer = none;
    }
}

void voiceControlTask()
{
    std::string value = (*voice).voiceCommand();
    std::cout << "Voice" << std::endl;

    if (value == "forward")
    {
        voiceBuffer = forward;
    }
    else if (value == "stop")
    {
        voiceBuffer = stop;
    }
    else if (value == "right")
    {
        voiceBuffer = right;
    }
    else if (value == "left")
    {
        voiceBuffer = left;
    }
    else if (value == "backward")
    {
        voiceBuffer = backward;
    }
    else
    {
        voiceBuffer = none;
    }
}

void remoteControlTask()
{
    std::string value = (*remote).remoteCommand();
    if (value == "forward")
    {
        remoteBuffer = forward;
    }
    else if (value == "stop")
    {
        remoteBuffer = stop;
    }
    else if (value == "right")
    {
        remoteBuffer = right;
    }
    else if (value == "left")
    {
        remoteBuffer = left;
    }
    else if (value == "backward")
    {
        remoteBuffer = backward;
    }
    else
    {
        remoteBuffer = none;
    }
}

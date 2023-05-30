#include "kernel.h"

const int NT = 3;
const int TICK_RATE = 200000; // 0.2 seconds

// Scheduler tasks variables
SchedTask tasks[NT];
int currTask = NT;

// AlphaBot instance
AlphaBot *alphaBot;
Remote *remote;

// Program execution
bool *run;

// Commands buffers
Command motorBuffer = none;
Command microBuffer = none;
Command sensorBuffer = none;
Command controllerBuffer = none;

SchedTask::SchedTask(void (*func)(), int delay, int period) : func(func), delay(delay), period(period), exec(0) {}
SchedTask::SchedTask() : func(NULL), delay(0), period(0), exec(0) {} // Default constructor

void schedSchedule()
{
    for (int x = 0; x < NT; x++)
    {
        if (!tasks[x].func)
            continue;
        if (tasks[x].delay != 0)
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
            std::cout << "Task " << x << " execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
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

void setupTasks(AlphaBot *ab, Remote *rm)
{
    alphaBot = ab;
    remote = rm;
    // Add tasks
    if (schedAddTask(&motorTask, 1, 10) == -1)
    {
        std::cout << "Erro adding motor task";
    };
    if (schedAddTask(&inputsControlTask, 0, 10) == -1)
    {
        std::cout << "Erro adding inputs task";
    };
    if (schedAddTask(&remoteControlTask, 0, 5) == -1)
    {
        std::cout << "Erro adding remote task";
    };
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

void inputsControlTask()
{
    if (microBuffer != none)
    {
        motorBuffer = microBuffer;
        microBuffer = none;
    };
    if (controllerBuffer != none)
    {
        motorBuffer = controllerBuffer;
        controllerBuffer = none;
    };
    if (sensorBuffer != none)
    {
        motorBuffer = sensorBuffer;
        sensorBuffer = none;
    };
}

void remoteControlTask()
{
    std::string value = (*remote).remoteCommand();
    if (value == "forward")
    {
        controllerBuffer = forward;
    }
    else if (value == "stop")
    {
        controllerBuffer = stop;
    }
    else if (value == "right")
    {
        controllerBuffer = right;
    }
    else if (value == "left")
    {
        controllerBuffer = left;
    }
    else if (value == "backward")
    {
        controllerBuffer = backward;
    }
    else
    {
        controllerBuffer = none;
    }
}

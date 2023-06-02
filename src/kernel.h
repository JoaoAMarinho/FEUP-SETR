#include "module.h"
#include <iostream>
#include <signal.h>
#include <chrono>

// Motor commands
enum Command
{
    forward,
    stop,
    right,
    left,
    backward,
    none
};

// Task struct representing a single task
struct SchedTask
{
    /* period in ticks */
    int period;
    /* ticks to activate */
    int delay;
    /* function pointer */
    void (*func)();
    /* activation counter */
    int exec;

    SchedTask();
    SchedTask(void (*func)(), int delay, int period);
};

void schedSchedule();

void schedDispatch();

void timerInterruptHandler(int signal);

int schedAddTask(void (*func)(), int delay, int period, int priority);

void setupTasks(AlphaBot *ab, Remote *rm, Voice *vc);

void enableInterrupts();

void disableInterrupts();

void closeHandler(int signal);

void schedInit(bool *val);

void motorTask();

void voiceControlTask();

void remoteControlTask();

#include <iostream>
#include <wiringPi.h>
#include <Python.h>

const int NT = 3;

// Task struct representing a single task
struct Sched_Task_t {
    /* period in ticks */
    int period;
    /* ticks to activate */
    int delay;
    /* function pointer */
    PyObject* callable;
    /* activation counter */
    int exec;

    Sched_Task_t(PyObject* func, int d, int p, int exec) : callable(func), delay(d), period(p), exec(exec) {}
    Sched_Task_t() : callable(nullptr), delay(0), period(0), exec(0) {}  // Default constructor
};

Sched_Task_t Tasks[NT];
int cur_task = -1;

void Sched_Schedule(){
    for(int x=0; x<NT; x++) {
        if (!Tasks[x].callable) continue;
        /* Schedule Task */
        Tasks[x].exec++;
        Tasks[x].delay = Tasks[x].period;
    }
}

void enable_interrupts();
void disable_interrupts();
void Sched_Dispatch(){
    int prev_task = cur_task;
    for(int x=0; x<cur_task; x++) {
        if (!Tasks[x].exec) continue;
        
        Tasks[x].exec=0;
        cur_task = x;
        enable_interrupts();
        // TODO
        // CALL PYTHON FUNC
        // Tasks[x].callable();
        // PyObject* pArgs = PyTuple_New(0);
        // PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

        // if (pResult != nullptr) {
        //     // Process the result (if needed)
        //     // ...

        //     Py_DECREF(pResult); // Release the reference to the result object
        // }
        std::cout << "Calling function from task " << x << std::endl;
        disable_interrupts();
        cur_task = prev_task;
        /*Delete if one-shot */
        if (!Tasks[x].period) Tasks[x].callable = 0;
    }
}

void tickInterruptHandler() {
    // Task scheduling logic
    // ...

    std::cout << "Tick interrupt occurred!\n";
    // Sched_Schedule();
    // Sched_Dispatch();
}

void enable_interrupts() {
    wiringPiISR(0, INT_EDGE_RISING, &tickInterruptHandler);
}

void disable_interrupts() {
    wiringPiISR(0, INT_EDGE_RISING, nullptr);
}

void Sched_Init() {
    wiringPiISR(17, INT_EDGE_RISING, &tickInterruptHandler);
}

int setupPythonModule(PyObject* (&pModule)) {
    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();

    pModule = PyImport_ImportModule("AlphaBot2_old");

    return pModule == nullptr ? -1 : 0;
}

int _Sched_AddT(PyObject* pFunc, int d, int p, int pri){
    if (pFunc == nullptr || Tasks[pri].callable) return -1;

    Sched_Task_t task(pFunc, d, p, 0);
    
    Tasks[pri] = task;
    std::cout << "Task added" << std::endl;
    return pri;
}

PyObject* get_python_func(PyObject* pModule, const char* func_name) {
    std::cout << "returning function" << std::endl;
    PyObject* pFunc = PyObject_GetAttrString(pModule, func_name);

    if (pFunc != nullptr && PyCallable_Check(pFunc)) {
        std::cout << "function returned" << std::endl;
        return pFunc;
    }
    std::cout << "noooooo function" << std::endl;

    return nullptr;
}

void Setup_Tasks(PyObject* pModule) {
    // Add tasks
    _Sched_AddT(get_python_func(pModule, "main"), 0, 1, 0);
}

int main() {
    // Setup
    wiringPiSetup();
    PyObject* pModule = nullptr;

    if (setupPythonModule(pModule) == -1) {
        std::cout << "here";
        Py_Finalize();
        return -1;
    }

    std::cout << "hereeeeee" << std::endl;
    Setup_Tasks(pModule);
    std::cout << Tasks;

    Sched_Init();
    std::cout << "Morreu";
    while (1) { /* do nothing! */ };

    for (int i = 0; i < NT; i++) {
        if (Tasks[i].callable) {
            Py_DECREF(Tasks[i].callable);
        }
    }

    Py_DECREF(pModule); // Release the reference to the module object
    Py_Finalize(); // Clean up and finalize the Python interpreter

    return 0;
}

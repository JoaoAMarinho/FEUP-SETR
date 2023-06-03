#include "kernel.h"

int main()
{
    // Initializes the Python interpreter
    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();

    AlphaBot ab;
    Remote rm;
    Voice vc;
    Sensor ss;
    bool run = true;

    setupTasks(&ab, &rm, &vc, &ss);
    schedInit(&run);

    while (run)
    { /* do nothing! */
    };

    ab.destroy();
    rm.destroy();
    vc.destroy();
    ss.destroy();
    Py_Finalize();

    return 0;
}

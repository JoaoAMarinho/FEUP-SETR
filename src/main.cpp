#include "kernel.h"

int main()
{
    // Initializes the Python interpreter
    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();

    AlphaBot ab;
    Remote rm;
    Voice vc;
    bool run = true;

    setupTasks(&ab, &rm, &vc);
    schedInit(&run);

    while (run)
    { /* do nothing! */
    };

    ab.destroy();
    rm.destroy();
    vc.destroy();
    Py_Finalize();

    return 0;
}

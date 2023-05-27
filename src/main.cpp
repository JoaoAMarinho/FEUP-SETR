#include "kernel.h"

int main()
{
    AlphaBot ab;
    Remote rm;
    bool run = true;

    setupTasks(&ab, &rm);
    schedInit(&run);

    while (run)
    { /* do nothing! */
    };

    ab.destroy();

    return 0;
}

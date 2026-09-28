#include <ZipsLib.h>

int main()
{
    init_libs();
    BEGIN_SETUP();

    BEGIN_LOOP();
    while (1)
    {
        tight_loop_contents();
    }

    return 0;
}
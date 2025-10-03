#include <ZipsLib.h>

int main()
{
    BEGIN_SETUP();

    BEGIN_LOOP();
    while (1)
    {
        tight_loop_contents();
    }

    return 0;
}
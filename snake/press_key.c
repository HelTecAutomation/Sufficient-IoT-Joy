#include "press_key.h"
#define TTY_PATH            "/dev/tty"
#define STTY_US             "stty raw -echo -F "
#define STTY_DEF            "stty -raw echo -F "

int press_key_init(void)
{
    system(STTY_US TTY_PATH);
}

int press_key_get_key_val(void)
{
    fd_set rfds;
    struct timeval tv;
    int ch = 0;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 200*1000; 

    if (select(1, &rfds, NULL, NULL, &tv) > 0)
    {
        ch = getchar();
    }
    return ch;
}
int press_key_deinit(void)
{
    system(STTY_DEF TTY_PATH);
}

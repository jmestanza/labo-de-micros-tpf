


#include "include\\global.h"
#include "include\\app_main.h"
#include "include\\hal.h"
#include "include\\app_audio.h"

int main(void)
{
    hal_init();
    printf("main\n");
    app_audio();

    while(1);
}





#include <zephyr/kernel.h>
#include "biz_logic.h"

int main(void) {
    biz_logic_init();

    while (1) {
        biz_logic_process();
        k_msleep(get_current_blink_interval());
    }
    return 0;
}
#include <time.h>

#include "cmdfx/util.h"

// Time

unsigned long currentTimeMillis() {
    return (int) (clock() / (CLOCKS_PER_SEC / 1000));
}
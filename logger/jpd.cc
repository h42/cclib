#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "logger.h"

int main() {
    logger log("test.log");
    log.log("hey now");
    return 0;
}

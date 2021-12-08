#include "debug_format.h"

ostream& spaced_left(ostream& outs)
{
    outs << setw(20) << left;
    return outs;
}

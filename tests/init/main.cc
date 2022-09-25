#include "nabu.h"

#include <print.h>
#include <range.h>

int main(int argc, char** argv)
{
    nabu::gate_t my_or(nabu::op_or);
    my_or.in(0) = nabu::on_state;
    my_or.in(1) = nabu::on_state;
    my_or.propagate();
    print(my_or.out());
    return 0;
}
#include "nabu.h"

#include <print.h>
#include <range.h>

int main(int argc, char** argv)
{
    nabu::gate_t my_and(nabu::op_and);
    nabu::gate_t my_or (nabu::op_or);
    nabu::edge_t e;
    e.attach(my_or.out());
    e.attach(my_and.in(0));
    my_or.set_input_state(0, nabu::on_state);
    my_or.set_input_state(1, nabu::on_state);
    my_and.set_input_state(1, nabu::on_state);
    print(my_and.out());
    print(my_or.out());
    return 0;
}
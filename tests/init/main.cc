
#include <print.h>
#include <range.h>

#include "nabu.h"

int main(int argc, char** argv)
{
    nabu::gate_t v0(nabu::op_inv);
    nabu::gate_t v1(nabu::op_inv);
    nabu::gate_t i0(nabu::op_id);
    nabu::gate_t i1(nabu::op_id);
    nabu::gate_t a0(nabu::op_and);
    
    nabu::edge_t e0;
    nabu::edge_t e1;
    nabu::edge_t e2;
    
    e0.attach(v0.out());
    e0.attach(a0.in(0));
    
    e1.attach(i0.out());
    e1.attach(a0.in(1));
    
    e2.attach(a0.out());
    e2.attach(i1.in(0));
    e2.attach(v1.in(0));
    
    print("===============================");
    v0.set_input_state(0, nabu::off_state);
    i0.set_input_state(0, nabu::off_state);
    print("output  ", i1.out()); //[S: 0]
    print("inverse ", v1.out()); //[S: 0] <-- should be [S: 1]
    print("===============================");
    
    print("");
    
    print("===============================");
    v0.set_input_state(0, nabu::off_state);
    i0.set_input_state(0, nabu::on_state);
    print("output  ", i1.out()); //[S: 1]
    print("inverse ", v1.out()); //[S: 0]
    print("===============================");
    return 0;
}
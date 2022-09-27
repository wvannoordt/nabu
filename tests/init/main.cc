
#include <print.h>
#include <range.h>

#include "nabu.h"

int main(int argc, char** argv)
{
    nabu::gate_t a0(nabu::op_and);
    nabu::gate_t o0(nabu::op_or);
    nabu::gate_t o1(nabu::op_or);
    nabu::gate_t o2(nabu::op_or);
    nabu::gate_t v0(nabu::op_inv);
    nabu::gate_t v1(nabu::op_inv);
    nabu::gate_t v2(nabu::op_inv);
    nabu::gate_t v3(nabu::op_inv);
    nabu::gate_t i0(nabu::op_id);
    nabu::gate_t i1(nabu::op_id);
    
    nabu::edge_t e0;
    nabu::edge_t e1;
    nabu::edge_t e2;
    nabu::edge_t e3;
    nabu::edge_t e4;
    nabu::edge_t e5;
    nabu::edge_t e6;
    nabu::edge_t e7;
    
    e0.attach(v0.out());
    e0.attach(o0.in(0));
    e0.attach(a0.in(0));
    
    e1.attach(v1.out());
    e1.attach(a0.in(1));
    e1.attach(o1.in(0));
    
    e2.attach(a0.out());
    e2.attach(o0.in(1));
    e2.attach(o1.in(1));
    e2.attach(i1.in(0));
    
    e3.attach(o0.out());
    e3.attach(v2.in(0));
    
    e4.attach(o1.out());
    e4.attach(v3.in(0));
    
    e5.attach(v2.out());
    e5.attach(o2.in(0));
    
    e6.attach(v3.out());
    e6.attach(o2.in(1));
    
    e7.attach(o2.out());
    e7.attach(i0.in(0));
    
    v0.set_input_state(0, nabu::on_state);
    // v1.set_input_state(1, nabu::off_state);
    // print(i0.out(), i1.out());
    return 0;
}
#include <print.h>
#include <range.h>
#include <fstream>
#include <sstream>

#include "nabu.h"

int main(int argc, char** argv)
{
    nabu::machine_t xor_g;
    
    auto o0 = xor_g.add_gate(nabu::op_or);
    auto o1 = xor_g.add_gate(nabu::op_or);
    auto o2 = xor_g.add_gate(nabu::op_or);
    auto o3 = xor_g.add_gate(nabu::op_or);
    
    auto v0 = xor_g.add_gate(nabu::op_inv);
    auto v1 = xor_g.add_gate(nabu::op_inv);
    auto v2 = xor_g.add_gate(nabu::op_inv);
    auto v3 = xor_g.add_gate(nabu::op_inv);
    auto v4 = xor_g.add_gate(nabu::op_inv);
    
    auto i0 = xor_g.add_gate(nabu::op_id);
    auto i1 = xor_g.add_gate(nabu::op_id);
    
    auto e0 = xor_g.add_edge();
    auto e1 = xor_g.add_edge();
    auto e2 = xor_g.add_edge();
    auto e3 = xor_g.add_edge();
    auto e4 = xor_g.add_edge();
    auto e5 = xor_g.add_edge();
    auto e6 = xor_g.add_edge();
    auto e7 = xor_g.add_edge();
    auto e8 = xor_g.add_edge();
    
    e0->attach(v0->out());
    e0->attach(o0->in(0));
    e0->attach(o3->in(0));
    
    e1->attach(v1->out());
    e1->attach(o3->in(1));
    e1->attach(o1->in(0));
    
    e2->attach(v4->out());
    e2->attach(o0->in(1));
    e2->attach(o1->in(1));
    e2->attach(i1->in(0));
    
    e3->attach(o0->out());
    e3->attach(v2->in(0));
    
    e4->attach(o1->out());
    e4->attach(v3->in(0));
    
    e5->attach(v2->out());
    e5->attach(o2->in(0));
    
    e6->attach(v3->out());
    e6->attach(o2->in(1));
    
    e7->attach(o2->out());
    e7->attach(i0->in(0));
    
    e8->attach(o3->out());
    e8->attach(v4->in(0));
    
    
    xor_g.add_input(v0->in(0));
    xor_g.add_input(v1->in(0));
    
    xor_g.add_output(i0->out());
    xor_g.add_output(i1->out());
    
    auto fff = [&](const int& si0, const int& si1) -> void
    {
        xor_g.set_input_state(0, (nabu::state)si0);
        xor_g.set_input_state(1, (nabu::state)si1);
        
        print(color::magenta, "i0   ", color::none, xor_g.in(0));
        print(color::magenta, "i1   ", color::none, xor_g.in(1));
        print(color::cyan,    "SUM  ", color::none, xor_g.out(0));
        print(color::cyan,    "CARRY", color::none, xor_g.out(1));
        print();
    };
    
    fff(0,0);
    fff(1,0);
    fff(0,1);
    fff(1,1);
    
    std::stringstream ss;
    nabu::write(xor_g, ss);
    std::string data = ss.str();
    
    std::istringstream iss(data);
    nabu::machine_t xxx;
    nabu::read(xxx, iss);

    std::stringstream ss2;
    nabu::write(xxx, ss2);
    std::string data2 = ss2.str();
    
    print(data);
    print(data2);
    print("(these two strings should be the same)");
    
    std::ofstream fout("xor.g");
    nabu::write(xxx, fout);
    
    return 0;
}

#include <print.h>
#include <range.h>
#include <fstream>

#include "nabu.h"

int main(int argc, char** argv)
{
    nabu::gate_t o3(nabu::op_or);
    nabu::gate_t o0(nabu::op_or);
    nabu::gate_t o1(nabu::op_or);
    nabu::gate_t o2(nabu::op_or);
    nabu::gate_t v0(nabu::op_inv);
    nabu::gate_t v1(nabu::op_inv);
    nabu::gate_t v2(nabu::op_inv);
    nabu::gate_t v3(nabu::op_inv);
    nabu::gate_t v4(nabu::op_inv);
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
    nabu::edge_t e8;
    
    e0.attach(v0.out());
    e0.attach(o0.in(0));
    e0.attach(o3.in(0));
    
    e1.attach(v1.out());
    e1.attach(o3.in(1));
    e1.attach(o1.in(0));
    
    e2.attach(v4.out());
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
    
    e8.attach(o3.out());
    e8.attach(v4.in(0));
    
    v0.set_input_state(0, nabu::on_state);
    v1.set_input_state(0, nabu::on_state);
    print(color::cyan, "SUM  ", color::none, i0.out());
    print(color::cyan, "CARRY", color::none, i1.out());
    
    std::ofstream myfile("graphic-debug/state.tex");
    auto get_state_str = [&](const nabu::state& st) -> std::string
    {
        switch ((int)st)
        {
            case (int)nabu::on_state: return "stateon";
            case (int)nabu::off_state: return "stateoff";
            case (int)nabu::bad_state: return "statebad";
            default: return "??";
        }
    };
    auto bind_state = [&](const auto& element, const std::string& id) -> void
    {
        myfile << "\\newcommand{\\" << id << "state}{\\" << get_state_str(element.get_state()) << "}\n";
    };
    
    bind_state(e0, "ea");
    bind_state(e1, "eb");
    bind_state(e2, "ec");
    bind_state(e3, "ed");
    bind_state(e4, "ee");
    bind_state(e5, "ef");
    bind_state(e6, "eg");
    bind_state(e8, "ej");
    
    bind_state(o0.in(0), "oaia");
    bind_state(o0.in(1), "oaib");
    bind_state(o0.out(), "oaoa");
    
    bind_state(o1.in(0), "obia");
    bind_state(o1.in(1), "obib");
    bind_state(o1.out(), "oboa");
    
    bind_state(o2.in(0), "ocia");
    bind_state(o2.in(1), "ocib");
    bind_state(o2.out(), "ocoa");
    
    bind_state(o3.in(0), "odia");
    bind_state(o3.in(1), "odib");
    bind_state(o3.out(), "odoa");
    
    bind_state(v0.in(0), "vaia");
    bind_state(v0.out(), "vaoa");
    
    bind_state(v1.in(0), "vbia");
    bind_state(v1.out(), "vboa");
    
    bind_state(v2.in(0), "vcia");
    bind_state(v2.out(), "vcoa");
    
    bind_state(v3.in(0), "vdia");
    bind_state(v3.out(), "vdoa");
    
    bind_state(v4.in(0), "veia");
    bind_state(v4.out(), "veoa");
    
    bind_state(i1.in(0), "ibia");
    bind_state(i1.out(), "iboa");
    
    return 0;
}
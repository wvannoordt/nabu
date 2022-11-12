#include <print.h>
#include <range.h>
#include <fstream>
#include <sstream>

#include "nabu.h"

int main(int argc, char** argv)
{
    nabu::machine_t xor_g, xor_g2;
    std::ifstream ifs("xor.g");
    nabu::io_status status = nabu::read(xor_g, ifs);
    
    std::ifstream ifs2("xor.g");
    nabu::io_status status2 = nabu::read(xor_g2, ifs2);
    
    if (status != nabu::io_success || (status2 != nabu::io_success))
    {
        print(status);
        print(status2);
        return 1;
    }
    
    auto fff = [&](nabu::machine_t& m, const int& si0, const int& si1) -> void
    {
        m.set_input_state(0, (nabu::state)si0);
        m.set_input_state(1, (nabu::state)si1);
        
        print(color::magenta, "i0   ", color::none, m.in(0));
        print(color::magenta, "i1   ", color::none, m.in(1));
        print(color::cyan,    "SUM  ", color::none, m.out(0));
        print(color::cyan,    "CARRY", color::none, m.out(1));
        print();
    };
    
    fff(xor_g,0,0);
    fff(xor_g,1,0);
    fff(xor_g,0,1);
    fff(xor_g,1,1);
    print("================");
    fff(xor_g2,0,0);
    fff(xor_g2,1,0);
    fff(xor_g2,0,1);
    fff(xor_g2,1,1);
    
    return 0;
}
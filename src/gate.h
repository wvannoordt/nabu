#pragma once

#include "edge.h"
#include "propagate_decl.h"

namespace nabu
{
    enum operation
    {
        op_or,
        op_and,
        op_id,
        op_inv
    };
    
    int get_num_inputs(const operation& oper)
    {
        switch (oper)
        {
            case op_or:  return 2;
            case op_and: return 2;
            case op_id:  return 1;
            case op_inv: return 1;
            default: return 0;
        }
    }
    
    state compute_or (const std::vector<inode_t>& inputs)
    {
        if ((inputs[0]==bad_state) || (inputs[1] == bad_state)) return bad_state;
        if ((inputs[0]== on_state) || (inputs[1] ==  on_state)) return  on_state;
        return off_state;
    }
    
    state compute_and(const std::vector<inode_t>& inputs)
    {
        if ((inputs[0]==bad_state) || (inputs[1] == bad_state)) return bad_state;
        if ((inputs[0]== on_state) && (inputs[1] ==  on_state)) return  on_state;
        return off_state;
    }
    
    state compute_id (const std::vector<inode_t>& inputs)
    {
        return inputs[0].node_state;
    }
    
    state compute_inv(const std::vector<inode_t>& inputs)
    {
        if (inputs[0] == bad_state) return bad_state;
        if (inputs[0] == off_state) return  on_state;
        if (inputs[0] ==  on_state) return off_state;
        return off_state;
    }    
    
    state compute_gate_output(const operation& gate_operation, const std::vector<inode_t>& inputs)
    {
        switch (gate_operation)
        {
            case op_or:  return compute_or (inputs);
            case op_and: return compute_and(inputs);
            case op_id:  return compute_id (inputs);
            case op_inv: return compute_inv(inputs);
            default: return bad_state;
        }
    }
    
    struct gate_t
    {
        operation gate_operation;
        onode_t output;
        std::vector<inode_t> inputs;
        gate_t(){}
        gate_t(const operation& gate_operation_in)
        {
            gate_operation = gate_operation_in;
            init_endpoints();
        }
        
        void init_endpoints()
        {
            int num_inputs = get_num_inputs(gate_operation);
            inputs.resize(num_inputs);
            for (auto& x: inputs)
            {
                x.node_state = off_state;
                x.owner = this;
            }
            output.node_state = off_state;
            output.owner = this;
        }
        
        inode_t&       in(const int& i)       {return inputs[i];}
        const inode_t& in(const int& i) const {return inputs[i];}
        onode_t& out() {return output;}
        const onode_t& out() const {return output;}
        void set_input_state(const int& i, const state i_state)
        {
            this->in(i) = i_state;
            propagate(*this);
        }
    };
}
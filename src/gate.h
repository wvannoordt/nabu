#pragma once

#include "edge.h"

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
    
    state compute_or (const std::vector<write_edge_endpoint_t>& inputs)
    {
        return off_state;
    }
    
    state compute_and(const std::vector<write_edge_endpoint_t>& inputs)
    {
        return off_state;
    }
    
    state compute_id (const std::vector<write_edge_endpoint_t>& inputs)
    {
        return off_state;
    }
    
    state compute_inv(const std::vector<write_edge_endpoint_t>& inputs)
    {
        return off_state;
    }    
    
    state compute_gate_output(const operation& gate_operation, const std::vector<write_edge_endpoint_t>& inputs)
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
        read_edge_endpoint_t output;
        std::vector<write_edge_endpoint_t> inputs;
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
            for (auto& x: inputs) x.edge_state = off_state;
            output.edge_state = off_state;
        }
        
        write_edge_endpoint_t&       in(int i)       {return inputs[i];}
        const write_edge_endpoint_t& in(int i) const {return inputs[i];}
        const read_edge_endpoint_t& out() const {return output;}
        bool propagate()
        {
            state output_state = compute_gate_output(gate_operation, inputs);
            if (output_state == this->output.edge_state)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    };
}
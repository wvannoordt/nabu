#pragma once

#include "state.h"
#include "edge.h"
#include "gate.h"

namespace nabu
{
    static void propagate(edge_t& edge)
    {
        // print("edge");
        for (auto n: edge.out)
        {
            //optimize later
            if (n->node_state != edge.control->node_state)
            {
                n->node_state = edge.control->node_state;
                propagate(*n);
            }
        }
    }
    
    static void propagate(inode_t& node)
    {
        // print("inode");
        if (node.owner != nullptr)
        {
            propagate(*(node.owner));
        }
    }
    
    static void propagate(onode_t& node)
    {
        // print("onode");
        if (node.edge != nullptr)
        {
            propagate(*(node.edge));
        }
    }
    
    static void propagate(gate_t& gate)
    {
        // print("gate");
        state output_state = compute_gate_output(gate.gate_operation, gate.inputs);
        //optimize later
        // if (output_state != gate.output.node_state)
        // {
            gate.output.node_state = output_state;
            propagate(gate.output);
        // }
    }
}
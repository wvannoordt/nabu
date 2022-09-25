#pragma once

#include <vector>

#include "state.h"

namespace nabu
{
    struct read_edge_endpoint_t
    {
        state edge_state = off_state;
        read_edge_endpoint_t& operator =(const state& state_in)
        {
            edge_state = state_in;
            return *this;
        }
    };
    
    struct write_edge_endpoint_t
    {
        state edge_state = off_state;
        write_edge_endpoint_t& operator =(const state& state_in)
        {
            edge_state = state_in;
            return *this;
        }
    };
    
    struct edge_t
    {
        state edge_state = off_state;
        
        //Every edge has a unique control point that sets the state
        read_edge_endpoint_t* control;
        std::vector<write_edge_endpoint_t*> out;
    };
}
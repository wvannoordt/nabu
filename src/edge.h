#pragma once

#include <vector>

#include "state.h"

namespace nabu
{
    struct gate_t;
    struct edge_t;
    
    enum node_type
    {
         in_node = 0,
        out_node = 1
    };
    
    template <const node_type n_type> struct node_t
    {
        gate_t* owner = nullptr;
        state node_state = off_state;
        node_t& operator =(const state& state_in)
        {
            node_state = state_in;
            return *this;
        }
        bool operator == (const state& state_in) const {return state_in == node_state;}
    };
    
    using inode_t = node_t< in_node>;
    using onode_t = node_t<out_node>;
    
    template <const node_type n_type> static std::ostream & operator<<(std::ostream & os, const node_t<n_type>& st)
    {
        os << st.node_state;
        return os;
    }
    
    struct edge_t
    {
        //Every edge has a unique control point that sets the state
        onode_t* control;
        std::vector<inode_t*> out;
    };
}
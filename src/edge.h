#pragma once

#include <vector>

#include "state.h"
#include "propagate_decl.h"

namespace nabu
{
    template <const node_type n_type> struct node_t
    {
        gate_t* owner = nullptr;
        edge_t* edge  = nullptr;
        state node_state = off_state;
        node_t& operator =(const state& state_in)
        {
            node_state = state_in;
            return *this;
        }
        bool operator == (const state& state_in) const {return state_in == node_state;}
    };
    
    template <const node_type n_type> static std::ostream & operator<<(std::ostream & os, const node_t<n_type>& st)
    {
        os << st.node_state;
        return os;
    }
    
    struct edge_t
    {
        //Every edge has a unique control point that sets the state
        onode_t* control = nullptr;
        std::vector<inode_t*> out;
        void attach(onode_t& onode)
        {
            control = &onode;
        }
        void attach(inode_t& inode)
        {
            for (auto p: out)
            {
                if (p == &inode) return;
            }
            out.push_back(&inode);
            
        }
    };
}
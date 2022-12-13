#pragma once

#include <vector>
#include <algorithm>

#include "state.h"
#include "propagate_decl.h"

namespace nabu
{
    static constexpr state get_default_ctr_state()
    {
        return bad_state;
    }
    
    template <const node_type n_type> struct node_t
    {
        gate_t* owner = nullptr;
        edge_t* edge  = nullptr;
        state node_state = get_default_ctr_state();
        node_t& operator =(const state& state_in)
        {
            node_state = state_in;
            return *this;
        }
        bool operator == (const state& state_in) const {return state_in == node_state;}
        state get_state() const {return node_state;}
        void  set_state(const state& st) {node_state = st;}
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
            onode.edge = this;
        }
        void attach(inode_t& inode)
        {
            for (auto p: out)
            {
                if (p == &inode) return;
            }
            out.push_back(&inode);
            inode.edge = this;
        }
        bool detach(inode_t& inode)
        {
            auto it = std::find(out.begin(), out.end(), &inode);
            if(it != out.end())
            {
                inode.edge = nullptr;
                out.erase(it);
            }
            return (out.size() == 0);
        }
        state get_state() const {return control->node_state;}
    };
}
#pragma once

#include "state.h"

namespace nabu
{
    enum node_type
    {
         in_node = 0,
        out_node = 1
    };
    struct gate_t;
    struct edge_t;
    template <const node_type n_type> struct node_t;
    using inode_t = node_t< in_node>;
    using onode_t = node_t<out_node>;
    static void propagate(edge_t& edge);
    static void propagate(inode_t& node);
    static void propagate(onode_t& node);
    static void propagate(gate_t& gate);
}
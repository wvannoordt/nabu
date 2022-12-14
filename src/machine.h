#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <array>

#include "gate.h"
#include "edge.h"

namespace nabu
{
    struct machine_t
    {
        std::vector<gate_t*>            gates;
        std::vector<edge_t*>            edges;
        
        std::vector<std::array<int, 2>> inputs;
        std::vector<int>                outputs;
        
        std::map<gate_t*, int>          gate_ids;
        std::map<edge_t*, int>          edge_ids;
        
        gate_t* add_gate(const operation& oper)
        {
            gates.push_back(new gate_t(oper));
            gate_t* output = gates.back();
            gate_ids[output] = gates.size()-1;
            return output;
        }
        
        edge_t* add_edge()
        {
            edges.push_back(new edge_t());
            edge_t* output = edges.back();
            edge_ids[output] = edges.size() - 1;
            return output;
        }
        
        void add_input(const inode_t& i)
        {
            int gate_id = this->id(i.owner);
            const inode_t* pi = &i;
            gate_t* gate = gates[gate_id];
            std::array<int, 2> elem;
            int in_id = -1000;
            if (pi == &(gate->inputs[0])) in_id = 0;
            if (pi == &(gate->inputs[1])) in_id = 1;
            if (in_id<0)
            {
                std::cout << "OH SHIT!!" << std::endl;
                abort();
            }
            elem[0] = gate_id;
            elem[1] = in_id;
            inputs.push_back(elem);
        }
        
        void add_output(const onode_t& o)
        {
            int gate_id = this->id(o.owner);
            outputs.push_back(gate_id);
        }
        
        int id(gate_t* g) const {return gate_ids.at(g);}
        int id(edge_t* e) const {return edge_ids.at(e);}
        
        inode_t& in(int i)             {return gates[inputs[i][0]]->in(inputs[i][1]); }
        const inode_t& in(int i) const {return gates[inputs[i][0]]->in(inputs[i][1]); }
        
        onode_t& out(int i)             {return gates[outputs[i]]->out();}
        const onode_t& out(int i) const {return gates[outputs[i]]->out();}
        
        void set_input_state(const int& i, const state i_state)
        {
            gate_t* gate = gates[inputs[i][0]];
            int id = inputs[i][1];
            gate->set_input_state(id, i_state);
        }
        
        void compute_maps()
        {
            gate_ids.clear();
            edge_ids.clear();
            for (int i = 0; i < gates.size(); ++i)
            {
                gate_ids.insert({gates[i], i});
            }
            for (int i = 0; i < edges.size(); ++i)
            {
                edge_ids.insert({edges[i], i});
            }
        }
        
        std::array<std::pair<edge_t*, bool>, 3> remove(gate_t* gate)
        {
            std::array<std::pair<edge_t*, bool>, 3> output;
            int id = gate_ids.at(gate);
            if (gate->output.edge != nullptr) gate->output.edge->control = nullptr;
            
            //we have to now destroy the edge that connected the output
            output[0].first  = gate->output.edge;
            output[0].second = gate->output.edge != nullptr; //true indicates invalid state
            for (int i = 0; i < 2; ++i)
            {
                inode_t* input = &gate->inputs[i];
                edge_t* in_edge = input->edge;
                output[1+i].first  = in_edge;
                output[1+i].second = false;
                if (in_edge != nullptr)
                {
                    auto it1 = std::find(in_edge->out.begin(), in_edge->out.end(), input);
                    in_edge->out.erase(it1);
                    output[1+i].second = in_edge->out.size() == 0;
                }
            }
            
            auto it = gates.begin() + id;
            gates.erase(it);
            auto it2 = gate_ids.find(gate);
            gate_ids.erase(it2);
            
            delete gate;
            compute_maps();
            return output;
        }
        
        void remove(edge_t* edge)
        {
            //problem, the edge ids actually have to update every time we remove an edge! same with gates.
            int id = edge_ids.at(edge);
            auto it1 = edges.begin() + id;
            edges.erase(it1);
            auto it2 = edge_ids.find(edge);
            if (it2 != edge_ids.end()) edge_ids.erase(it2);
            if (edge->control != nullptr) edge->control->edge = nullptr;
            for (auto o: edge->out) o->edge = nullptr;
            delete edge;
            compute_maps();
        }
        
        void refresh()
        {
            for (auto g: gates)
            {
                for (int i = 0; i < 2; ++i)
                {
                    if (g->in(i).edge == nullptr) g->set_input_state(i, state::off_state);
                }
            }
        }
        
        void clear()
        {
            for (auto g: gates) delete g;
            for (auto e: edges) delete e;
            gates.clear();
            edges.clear();
            inputs.clear();
            outputs.clear();
            edge_ids.clear();
            gate_ids.clear();
        }
        
        std::vector<gate_t*>& get_gates() {return gates;}
        std::vector<edge_t*>& get_edges() {return edges;}
        const std::vector<gate_t*>& get_gates() const {return gates;}
        const std::vector<edge_t*>& get_edges() const {return edges;}
        
        ~machine_t()
        {
            clear();
        }
    };
}
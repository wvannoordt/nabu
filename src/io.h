#pragma once

#include <iostream>
#include <string>

#include "machine.h"

namespace nabu
{    
    enum io_status
    {
        io_success,
        io_ill_formed,
        io_null_data,
        io_failed_parse
    };
    
    static std::string io_stat_string(const io_status& stat)
    {
        switch (stat)
        {
            case io_success:      return "success";
            case io_ill_formed:   return "ill-formed data in stream";
            case io_null_data:    return "no data in stream";
            case io_failed_parse: return "failed to parse data in stream";
        }
        return "[NABU: INVALID IO STATUS]";
    }
    
    std::ostream& operator << (std::ostream& os, const io_status& stat)
    {
        os << io_stat_string(stat);
        return os;
    }
    
    enum io_tokens
    {
        io_begin     = '<',
        io_separate  = '|',
        io_vec_start = '[',
        io_vec_end   = ']',
        io_delimit   = ',',
        io_end       = '>'
    };
    
    template <typename stream_t>
    static io_status write(const gate_t& data, stream_t& strm)
    {
        strm << (char)data.get_op();
        strm << (int)data.inputs[0].node_state;
        strm << (int)data.inputs[1].node_state;
        strm << (int)data.output.node_state;
        return io_success;
    }
    
    template <typename stream_t>
    static io_status write(const machine_t& data, stream_t& strm)
    {
        strm << (char)io_begin;
        //write gates
        const auto& gates = data.get_gates();
        for (auto g: gates)
        {
            write(*g, strm);
        }
        strm << (char)io_separate;
        //write edges
        const auto& edges = data.get_edges();
        for (auto e: edges)
        {
            strm << (char)io_vec_start;
            strm << (int)e->get_state();
            strm << (char)io_delimit;
            strm << data.id(e->control->owner);
            for (auto out: e->out)
            {
                gate_t* gate = out->owner;
                strm << (char)io_delimit;
                strm << data.id(gate);
                strm << (char)io_delimit;
                inode_t* i0 = &(gate->inputs[0]);
                inode_t* i1 = &(gate->inputs[1]);
                int pos = -1;
                if (i0 == out) pos = 0;
                if (i1 == out) pos = 1;
                if (pos < 0) return io_ill_formed;
                strm << pos;
            }
            strm << (char)io_vec_end;
        }
        strm << (char)io_separate;
        //write input ids
        strm << (char)io_vec_start;
        int ct = 0;
        for (auto& inp: data.inputs)
        {
            if (ct++!=0) strm << (char)io_delimit;
            strm << inp[0];
            strm << (char)io_delimit;
            strm << inp[1];
        }
        strm << (char)io_vec_end;
        //write output ids
        strm << (char)io_vec_start;
        ct = 0;
        for (auto& inp: data.outputs)
        {
            if (ct++!=0) strm << (char)io_delimit;
            strm << inp;
        }
        strm << (char)io_vec_end;
        //finish
        strm << (char)io_end;
        return io_success;
    }
    
    template <typename stream_t>
    static io_status read(gate_t& data, stream_t& strm)
    {
        char op, i0, i1, o;
        if (!strm.get(op)) return io_failed_parse;
        if (!strm.get(i0)) return io_failed_parse;
        if (!strm.get(i1)) return io_failed_parse;
        if (!strm.get(o)) return io_failed_parse;
        data.inputs[0].set_state((state)(i0-'0'));
        data.inputs[1].set_state((state)(i1-'0'));
        data.gate_operation = (operation)op;
        data.output.set_state((state)(o-'0'));
        return io_success;
    }
    
    template <typename stream_t>
    static io_status read(machine_t& data, stream_t& strm)
    {
        char c;
        if (!strm.get(c)) return io_null_data;
        if (c != (char)io_begin) return io_ill_formed;
        while (true)
        {
            char p = strm.peek();
            if (p == (char)io_separate)
            {
                strm.get(p);
                break;
            }
            gate_t* g = data.add_gate((operation)p);
            auto code = read(*g, strm);
            if (code != io_success) return code;
        }
        auto read_iv = [&](std::vector<int>& v, io_status& stat) -> bool
        {
            v.clear();
            char s;
            if (!strm.get(s))
            {
                stat = io_ill_formed;
                return false;
            }
            if (s != (char)io_vec_start)
            {
                stat = io_failed_parse;
                return false;
            }
            if (!strm.get(s))
            {
                stat = io_ill_formed;
                return false;
            }
            std::string num;
            while (s != io_vec_end)
            {
                num.clear();
                while (s != (char)io_delimit)
                {
                    num += s;                
                    strm.get(s);
                    if (s == (char)io_vec_end) break;
                }
                if (s != (char)io_vec_end) strm.get(s);
                std::istringstream iss(num);
                int i;
                iss >> i;
                if (iss.fail())
                {
                    stat = io_failed_parse;
                    return false;
                }
                v.push_back(i);
            }
            return true;
        };
        std::vector<int> iv;
        std::vector<std::vector<int>> ivs;
        io_status err = io_success;
        while(true)
        {
            char p = strm.peek();
            if (p == (char)io_separate) break;
            if (!read_iv(iv, err)) return err;
            ivs.push_back(iv);
            data.add_edge();
        }
        std::array<state, 3> state_ar;
        state_ar[0] = off_state;
        state_ar[1] = on_state;
        state_ar[2] = bad_state;
        for (int i = 0; i < ivs.size(); ++i)
        {
            auto& ivloc = ivs[i];
            if (ivs.size()<2) return io_ill_formed;
            edge_t* e = data.edges[i];
            state edge_state = state_ar[ivloc[0]];
            int input_id = ivloc[1];
            e->attach(data.gates[input_id]->out());
            for (int j = 2; j < ivloc.size(); j += 2)
            {
                gate_t* gate = data.gates[ivloc[j]];
                int in_id    = ivloc[j+1];
                e->attach(gate->in(in_id));
            }
        }
        char d;
        if (!strm.get(d)) return io_ill_formed;
        std::vector<int> inputs_raw;
        if (!read_iv(inputs_raw,   err)) return err;
        if (!read_iv(data.outputs, err)) return err;
        data.inputs.resize(inputs_raw.size()/2);
        for (int ppp = 0; ppp < data.inputs.size(); ++ppp)
        {
            data.inputs[ppp][0] = inputs_raw[2*ppp];
            data.inputs[ppp][1] = inputs_raw[2*ppp+1];
        }
        return io_success;
    }
}
#pragma once

#include <iostream>

namespace nabu
{
    enum state
    {
        off_state = 0,
        on_state  = 1,
        bad_state = 2
    };
    
    static std::ostream & operator<<(std::ostream & os, const state& st)
    {
        switch (st)
        {
            case off_state:
            {
                os << "[S: 0]";
                break;
            }
            case on_state:
            {
                os << "[S: 1]";
                break;
            }
            case bad_state:
            {
                os << "[S: -]";
                break;
            }
        }
        return os;
    }
}
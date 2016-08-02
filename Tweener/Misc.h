//
// Created by Matthew Gray on 7/3/16.
//

#ifndef TWEENER_MISC_H
#define TWEENER_MISC_H

// helper functions not included in sample

template<typename t_type> inline void SafeDelete(t_type*& pointer)
{
    if(0 != pointer)
    {
        delete pointer;
        pointer = 0;
    }
}
class MathUtil {
public:

    template<typename t_type>
    inline static t_type min(t_type a, t_type b) {
        return a < b ? a : b;
    }
};

const float kPi = 3.1415926535f;

#endif //TWEENER_MISC_H

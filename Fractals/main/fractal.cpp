#ifndef BU_FRACTAL
#define BU_FRACTAL

#include "header.hpp"

namespace buf{


class Fractal{
    public:

        Fractal(){};
        virtual void update(int paleta) = 0;
        virtual void input(float dt) = 0;
        virtual void gui() = 0;
        virtual ulm::String getName() = 0;
};

}

#endif
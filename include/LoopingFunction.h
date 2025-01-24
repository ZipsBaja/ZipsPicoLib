#pragma once

namespace uazips
{

    /*
    * This interface is used to create other polymorphous classes which all share
    * the capability of real-time analog or digital signals that need to be polled.
    */
    class LoopingFunction
    {
    public:
        virtual void Update() = 0;
    };

}
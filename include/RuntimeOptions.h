#pragma once

#include <unordered_map>

namespace uazips
{

    /*
    * This class is useful for creating certain options
    * for the codeflow, such as printing to the console,
    * or choosing to use the onboard LED for communication.
    * This is a slower but more convientent alternative to
    * preprocessor definitions. This is also a way to change
    * the options while the code is running, like disabling
    * console output after a certain requirement is met.
    */
    class RuntimeOptions
    {
    private:
        std::unordered_map<const char*, bool> options;

    public:
        inline RuntimeOptions()
        {
        }

        inline RuntimeOptions(const char** options, size_t options_len)
        {
            for (size_t i = 0; i < options_len; i++)
            {
                this->options[options[i]] = false;
            }
        }

        inline bool AddOption(const char* name, bool mode)
        {
            options[name] = mode;
        }

        inline bool RemoveOption(const char* name)
        {
            options.erase(name);
        }

        inline bool Check(const char* option) const
        {
            return options.at(option);
        }

        inline void Flip(const char* option)
        {
            bool* mode = &options.at(option);
            *mode = !*mode;
        }

        inline void Set(const char* option, bool new_mode)
        {
            options.at(option) = new_mode;
        }

    };


}
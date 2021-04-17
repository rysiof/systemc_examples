/*
MIT License

Copyright (c) 2020 rysiof@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef PARAMS_H
#define PARAMS_H

#include <unordered_map>
#include <string>
#include <any>

#include "logging.h"
/*!
    This structure is meant to be generic structure that holds parameters in
    neat, dictionary like, structure.

    @code
    module_params params;
    params.add<float>("cpu_frequency", 1000000.0f);
    params.add<uint32_t>("cpu_count", 4);
    params.add<std::vector<uint32_t> >("dcache_per_cpu_size", {0x10000, 0x10000, 0x4000, 0x4000}});
    @endcode
*/
class module_params
{
public:
    template<typename T>
    void add(std::string k, T v)
    {
        if (m_parameters_map.find(k) == m_parameters_map.end())
        {
            m_parameters_map[k] = v;
        }
        else
        {
            LOG(ERROR, "Cannot add parameter \"%s\" - already exists!", k.c_str());
            throw "Already added";
        }
    }
    
    template<typename T>
    void set(std::string k, T v)
    {
        if (m_parameters_map.find(k) == m_parameters_map.end())
        {
            LOG(ERROR, "Cannot update parameter \"%s\" - does not exist!", k.c_str());
            throw "Cannot update parameter that does not exist";
        }
        else
        {
            m_parameters_map[k] = v;
        }
    }

    template<typename T>
    T get(std::string k) const
    {
        auto it = m_parameters_map.find(k); 
        if (it == m_parameters_map.end())
        {
            LOG(ERROR, "Cannot retrieve parameter \"%s\" - does not exist!", k.c_str());
            throw "Not found";
        }
        return std::any_cast<T>(it->second);
    }
private:
    /*!
        @note   Let me explain std::any - we are using it just to keep single 
                hashmap. Also in that way it is relatively easy to add different
                types we want to support here. Since this shall be used only
                during initialziation phase there is limited (relatively low)
                impact on performance.
    */
    std::unordered_map<std::string, std::any> m_parameters_map;

};

#endif // PARAMS_H
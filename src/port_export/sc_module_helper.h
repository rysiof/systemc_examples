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

#ifndef SC_MODULE_HELPER_H
#define SC_MODULE_HELPER_H

#include <systemc.h>
#include <map>

struct sc_module_ext: sc_module
{
public:
    sc_module_ext(const sc_module_name& name): sc_module(name){}

    /*!
        Method for connecting modules based on port/export user friendly names.
    */
    static void connect_modules(
        sc_module_ext& src, const char* src_port,
        sc_module_ext& dst, const char* dst_export);

    /*!
        Logs all registered input ports.

        @note For debug purposes only.
    */
    void log_input_ports() const;

    /*!
        Logs all registered output ports.

        @note For debug purposes only.
    */
    void log_output_ports() const;

    /*!
        Logs all registered ports (both inputs and outputs).

        @note For debug purposes only.
    */
    void log_ports() const;
protected:
    void register_input_port(sc_export<sc_core::sc_interface>* input_port);

    void register_output_port(sc_port<sc_core::sc_interface>* output_port);

    /*!
        @note   We are sadly using 'std::string' because 'const char*' might
                not be good suitable type as key.
        @note   We are using 'sc_core::sc_interface' is order to access 'bind' method.
    */
    std::map<std::string, sc_export<sc_core::sc_interface>*> m_input_port_map;
    std::map<std::string, sc_port<sc_core::sc_interface>*> m_output_port_map;

private:
    /*!
        Simply concatenates module.name() + "." + pin_name.
    */
    static std::string get_pin_name(sc_module_ext& module, const char* pin_name);
    
    sc_port<sc_core::sc_interface>* get_port(const char* pin_name);

    sc_export<sc_core::sc_interface>* get_export(const char* pin_name);
};

void sc_module_ext::connect_modules(
    sc_module_ext& src, const char* src_port,
    sc_module_ext& dst, const char* dst_export)
{
    sc_port<sc_core::sc_interface>* src_if = src.get_port(src_port);
    if (src_if == NULL)
    {
        std::string exception_str = "Couldn't find port ";
        exception_str += src_port;
        exception_str += " in module ";
        exception_str += src.name();
        LOG(ERROR, "ERROR: %s", exception_str.c_str());
        throw exception_str.c_str();
    }
    LOG(LOW, "src_if = %s [%p]", src_if->name(), src_if);

    sc_export<sc_core::sc_interface>* dst_if = dst.get_export(dst_export);    
    if (dst_if == NULL)
    {
        std::string exception_str = "Couldn't find port ";
        exception_str += dst_export;
        exception_str += " in module ";
        exception_str += dst.name();
        LOG(ERROR, "ERROR: %s", exception_str.c_str());
        throw exception_str.c_str();
    }
    LOG(LOW, "dst_if = %s [%p]", dst_if->name(), dst_if);

    src_if->bind(*dst_if);
}

void sc_module_ext::log_ports() const
{
    log_input_ports();
    log_output_ports();
}

void sc_module_ext::log_input_ports() const
{
    for (auto item : m_input_port_map)
    {
        LOG(DEBUG, "module \"%s\" has input port: \"%s\"", name(), item.first.c_str());
    }
}

void sc_module_ext::log_output_ports() const
{
    for (auto item : m_output_port_map)
    {
        LOG(DEBUG, "module \"%s\" has output port: \"%s\"", name(), item.first.c_str());
    }
}

void sc_module_ext::register_input_port(sc_export<sc_core::sc_interface>* input_port)
{
    std::string s = input_port->name();
    LOG(LOW, "register_input_port = %s [%p]", s.c_str(), input_port);

    m_input_port_map[s] = input_port;
}

void sc_module_ext::register_output_port(sc_port<sc_core::sc_interface>* output_port)
{
    std::string s = output_port->name();
    LOG(LOW, "register_output_port = %s [%p]", s.c_str(), output_port);
    m_output_port_map[s] = output_port;
}

std::string sc_module_ext::get_pin_name(sc_module_ext& module, const char* pin_name)
{
    std::string retval = module.name();
    retval += ".";
    retval += pin_name;
    return retval;
}

sc_port<sc_core::sc_interface>* sc_module_ext::get_port(const char* pin_name)
{
    std::string port_str = get_pin_name(*this, pin_name);
    
    auto pair = m_output_port_map.find(port_str);
    if (pair == m_output_port_map.end())
    {
        return NULL;
    }
    return pair->second;
}

sc_export<sc_core::sc_interface>* sc_module_ext::get_export(const char* pin_name)
{
    std::string port_str = get_pin_name(*this, pin_name);
    
    auto pair = m_input_port_map.find(port_str);
    if (pair == m_input_port_map.end())
    {
        return NULL;
    }
    return pair->second;
}
#endif // SC_MODULE_HELPER_H
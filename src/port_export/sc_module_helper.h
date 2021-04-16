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

protected:
    void register_input_port(sc_export<sc_core::sc_interface>* input_port);

    void register_output_port(sc_port<sc_core::sc_interface>* output_port);

    /*!
        @note   We are sadly using 'std::string' because 'const char*' might not be good suitable type
                as key.
        @note   We are using 'sc_core::sc_interface' is order to access 'bind' method.
    */
    std::map<std::string, sc_export<sc_core::sc_interface>*> m_input_port_map;
    std::map<std::string, sc_port<sc_core::sc_interface>*> m_output_port_map;

private:
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
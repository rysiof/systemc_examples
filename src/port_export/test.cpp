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

#include <systemc.h>
#include "logging.h"
#include "sc_module_helper.h"
/*!
  This is write interface. We are going to use it to connect source and
  destination modules.
*/
struct write_if: public sc_core::sc_interface
{
  virtual void nb_write(const sc_dt::sc_unsigned& value) = 0; 
};

struct source: sc_module_ext
{
  SC_HAS_PROCESS(source);
  source(const sc_module_name& name) :
    sc_module_ext(name),
    m_port("output") // note: m_port.name() will be $name + "." + "output"
  {
    SC_THREAD(thread);
    // need to register if we want later to utilize sc_module_ext API
    register_output_port((sc_port<sc_core::sc_interface>*)&m_port);
  }
  void thread()
  {
    for (uint32_t idx = 0; idx < 10; ++idx)
    {
      sc_dt::sc_unsigned u; u = idx;
      m_port->nb_write(u);
      wait(10, SC_NS);
    }
  }
private:
  sc_port<write_if> m_port;
};

/*!
  This implementation of write interface. This can be private within
  destination module. We made it public to increase reusability.
*/
struct write_if_impl: public write_if
{
public:
  write_if_impl(sc_event& event, sc_dt::sc_unsigned& value) :
    m_event(event), m_value(value)
  {
  }

  /*!
    Simple non-blocking method. It is going to be used in source module,
    in order to pass value to destination.
  */
  virtual void nb_write(const sc_dt::sc_unsigned& value) override
  {
    LOG(LOW, "nb_write(%X)", value.to_uint());
    m_value = value;
    m_event.notify(1, SC_NS);
  }
private:
  sc_event& m_event;
  sc_dt::sc_unsigned& m_value;
};

struct destination: sc_module_ext
{
  SC_HAS_PROCESS(destination);

  destination(const sc_module_name& name) :
    sc_module_ext(name),
    m_export("input"), // note: m_port.name() will be $name + "." + "input"
    m_if(m_event, m_value)
  {
    m_export.bind(m_if);
    SC_THREAD(thread);
    // need to register if we want later to utilize sc_module_ext API
    register_input_port((sc_export<sc_core::sc_interface>*)&m_export);
  }
  void thread()
  {
    while(true)
    {
      wait(m_event);
      LOG(LOW, "Received event in src, value is now %X", m_value.to_uint());
    }
  }
private:
  sc_export<write_if> m_export;
  write_if_impl m_if;
  sc_event m_event;
  sc_dt::sc_unsigned m_value;
};

int sc_main(int argc, char** argv)
{ 
  source src("source");
  destination dst("destination");
  sc_module_ext::connect_modules(src, "output", dst, "input");

  sc_start();
  return 0;
}

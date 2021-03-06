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
#include "sc_module_ext.h"
#include "params.h"

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
  source(const sc_module_name& name, const module_params& params) :
    sc_module_ext(name),
    m_port("output"), // note: m_port.name() will be $name + "." + "output"
    m_period(params.get<sc_time>("thread_period"))
  {
    SC_THREAD(thread);
    // need to register if we want later to utilize sc_module_ext API
    register_output_port((sc_port<sc_core::sc_interface>*)&m_port);
  }
  void thread()
  {
    uint32_t idx = 0;
    while(true)
    {
      sc_dt::sc_unsigned u; u = idx++;
      m_port->nb_write(u);
      wait(m_period);
    }
  }
private:
  sc_port<write_if> m_port;
  sc_time m_period;
};

/*!
  This implementation of write interface. This can be private within
  destination module. We made it public to increase reusability.
*/
struct write_if_impl: public write_if
{
public:
  write_if_impl(sc_event& event, sc_time write_delay, sc_dt::sc_unsigned& value) :
    m_event(event), m_write_delay(write_delay), m_value(value)
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
    // As this is sample code we don't care about configurability.
    // For "pro" code you can add some parameters, runtime config etc.
    m_event.notify(m_write_delay);
  }
private:
  sc_event& m_event;
  sc_time m_write_delay;
  sc_dt::sc_unsigned& m_value;
};

/*!
  This structure models something that is suppose to resemble input port. In this
  case we can imagine 'in std_logic_vector(31 downto 0)' in VHDL.
*/
struct input:
  public sc_export<write_if>,
  public write_if_impl
{
  /*!
    @param event Event to be trigerred when nb_write is called from source module.
  */
  input(const char* name, sc_time write_delay, sc_event& event):
    sc_export<write_if>(name),
    write_if_impl(event, write_delay, m_value)
  {
    m_value = 0;
    bind(*this);
  }
  sc_dt::sc_unsigned m_value;
};

struct destination: sc_module_ext
{
  SC_HAS_PROCESS(destination);

  destination(const sc_module_name& name, const module_params& params) :
    sc_module_ext(name),
    m_export("input", params.get<sc_time>("write_delay"), m_event) // note: m_port.name() will be $name + "." + "input"
  {
    SC_THREAD(thread);
    // need to register if we want later to utilize sc_module_ext API
    register_input_port((sc_export<sc_core::sc_interface>*)&m_export);
  }
  void thread()
  {
    while(true)
    {
      wait(m_event);
      LOG(LOW, "Received event in src, value is now %X", m_export.m_value.to_uint());
    }
  }
private:
  input m_export;
  sc_event m_event;
};

int sc_main(int argc, char** argv)
{ 
  module_params params;
  params.add("thread_period", sc_time(10, SC_NS));
  params.add("write_delay", sc_time(1, SC_NS));

  source src("source", params);
  destination dst("destination", params);
  // this is the reason why we introduced 'sc_module_ext' - to make out lifes
  // a little easier. Because alternatively we would have to call:
  // src.m_port.bind(dst.m_export);
  // on surface it does not looks that bad. but imagine having multiple
  // different pins and trying to figure out how to connect them. Here we can
  // simply say 'connect "input" from dst to "output" from src'.
  // additionally we can hide from module user m_port and m_export used in this
  // example. and with getters it will loose its simplicity.
  sc_module_ext::connect_modules(src, "output", dst, "input");
  sc_start(100, SC_NS); // runs for 100 ns
  return 0;
}

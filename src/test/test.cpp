#include "systemc.h"
#include "logging.h"


class write_if: public sc_core::sc_interface{
public:
  write_if(sc_event& event) : m_event(event)
  {
  }
  void nb_write(const sc_dt::sc_unsigned& value)
  {
    LOG(LOW, "%p -> nb_write(%X)", this, value.to_uint());
    m_event.notify(1, SC_NS);
  }
  sc_event& m_event;
};
struct Src: sc_module
{
  SC_CTOR(Src): m_port("exp")
  {
    SC_THREAD(thread);
  }
  void thread()
  {
    for (size_t idx = 0; idx < 1000; ++idx)
    {
      sc_dt::sc_unsigned u(idx);
      m_port->nb_write(u);
      wait(10, SC_NS);
    }
  }
  sc_port<write_if> m_port;
};

struct Dst: sc_module
{

  SC_CTOR(Dst): m_export("exp"), m_if(event)
  {
    m_export.bind(m_if);
    SC_THREAD(thread);
  }
  void thread()
  {
    for (size_t idx = 0; idx < 1000; ++idx)
    {
      wait(event);
      LOG(INFO, "Received event in src");
    }
  }
  sc_export<write_if> m_export;
  write_if m_if;
  sc_event event;
};

int sc_main(int argc, char** argv)
{ 
  Src s("s");
  Dst d("d");
  s.m_port.bind(d.m_export);

  sc_start();
  return 0;
}

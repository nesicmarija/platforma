#include "vp.hpp"

Vp::Vp (sc_core::sc_module_name name)
  : sc_module (name)
  , soft("Soft")
  , hard("Hard")
  , bram_m("Bram_M")
  , bram_ctrl("BramCtrl")
  , interconnect("Interconnect")
{
  soft.interconnect_socket.bind(interconnect.soft_socket);
  interconnect.hard_socket.bind(hard.soft_socket);
  hard.bram_m_socket.bind(bram_m.bram_port);
  bram_ctrl.bram_m_socket.bind(bram_m.bram_port); 


  SC_REPORT_INFO("Virtual Platform", "Constructed.");
}

Vp::~Vp()
{
  SC_REPORT_INFO("Virtual Platform", "Destroyed.");
}

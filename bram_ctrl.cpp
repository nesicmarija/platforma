#include "bram_ctrl.hpp"

BramCtrl::BramCtrl(sc_core::sc_module_name name) : sc_module(name)
{
  soft_socket.register_b_transport(this, &BramCtrl::b_transport);
  SC_REPORT_INFO("BRAM Controller", "Constructed.");
}

BramCtrl::~BramCtrl()
{
  SC_REPORT_INFO("BRAM Controller", "Destroyed.");
}

void BramCtrl::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
  tlm::tlm_command cmd = pl.get_command();
  sc_dt::uint64 addr = pl.get_address();
  unsigned int len = pl.get_data_length();
  unsigned char *buf = pl.get_data_ptr();

  pl_m.set_command(cmd);
  pl_m.set_address(addr);
  pl_m.set_data_length(BUFF_SIZE);
  pl_m.set_data_ptr(buf);
  pl_m.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );

  bram_socket->b_transport(pl_m,offset);
  if (pl_m.is_response_error()) SC_REPORT_ERROR("Bram",pl_m.get_response_string().c_str());


}

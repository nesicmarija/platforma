#ifndef VP_HPP_
#define VP_HPP_

#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "typedefs.hpp"
#include "bram.hpp"
#include "soft.hpp"
#include "hard.hpp"
#include "interconnect.hpp"

class Vp :  public sc_core::sc_module
{
public:
  Vp(sc_core::sc_module_name name);
  ~Vp();

protected:
  Soft soft;
  Bram bram;
  Hard hard;
  Interconnect interconnect;
};

#endif // VP_HPP_

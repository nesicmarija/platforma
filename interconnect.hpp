/*interface: A class derived from class sc_interface. An interface proper is an interface, and in the object-
oriented sense a channel is also an interface. However, a channel is not an interface proper.
*/
#ifndef INTERCONNECT_HPP_
#define INTERCONNECT_HPP_
                             
#include <iostream>
#include <fstream>
#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "typedefs.hpp"
#include "utils.hpp"

class Interconnect :
         public sc_core::sc_module   //class tlm_blocking_transport_if: public virtual sc_core:: sc_interface 
{
public:
  Interconnect(sc_core::sc_module_name name);
  ~Interconnect();
  tlm_utils::simple_initiator_socket<Interconnect> bram_socket;
  tlm_utils::simple_initiator_socket<Interconnect> hard_socket;
  tlm_utils::simple_target_socket<Interconnect> soft_socket;
 /*
     tlm_utils::simple_target_socket<Interconnect> targ_socket;
     tlm_utils::simple_initiator_socket<Interconnect>init_socket;
  */
protected:

/* typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
 void b_transport(pl_t&, sc_core::sc_time&);   */
  //void b_transport(TRANS&, sc_time&);
 pl_t pl;
 sc_core::sc_time offset;
 void b_transport(pl_t &pl, sc_core::sc_time &offset);

};

#endif // INTERCONNECT_HPP_

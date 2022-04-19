#include "interconnect.hpp"

Interconnect::Interconnect(sc_core::sc_module_name name) :
     sc_module(name)
     , offset(sc_core::SC_ZERO_TIME)
{
  soft_socket.register_b_transport(this, &Interconnect::b_transport);
                                 //targ_socket.register_b_transport( this, &Interconnect::b_transport);
  SC_REPORT_INFO("Interconnect", "Constructed.");
}

Interconnect::~Interconnect()
{
  SC_REPORT_INFO("Interconnect", "Destroyed.");  
}


void Interconnect::b_transport(pl_t &pl, sc_core::sc_time &offset)  
/*b_transport method has a single transaction argument passed by non-const reference and a second argument to annotate timing

The b_transport method has a timing annotation argument. This single argument is used on both the call to
and the return from b_transport to indicate the time of the start and end of the transaction, respectively,
relative to the current simulation time.*/ 

{
  sc_dt::uint64 addr = pl.get_address();  
  sc_dt::uint64 taddr = addr & 0x00FFFFFF;  //na osnovu prva dva bajta vidi se da li radimo sa bramom ili hardom
  

	if(addr >= VP_ADDR_BRAM_L && addr <= VP_ADDR_BRAM_H)
    {
      // std::cout << "sending to bram" << std::endl;
      pl.set_address(taddr);
      bram_socket->b_transport(pl, offset);
      pl.set_address(addr);
    }
	else if(addr >= VP_ADDR_HARD_L && addr <= VP_ADDR_HARD_H)
    {
      // std::cout << "sending to hard" << std::endl;
      pl.set_address(taddr);
      hard_socket->b_transport(pl, offset);  //init_socket->b_transport(trans, delay);
     pl.set_address(addr); 
       /*The method operator-> of the target socket shall call method operator-> of the port in the target socket
       (on the backward path), and shall return the value returned by operator-> of the port.*/
    } 
    
	else
    {
      SC_REPORT_ERROR("Interconnect", "Wrong address.");
      pl.set_response_status ( tlm::TLM_ADDRESS_ERROR_RESPONSE ); 
      //Target was unable to act upon the address attribute, or address out-of-range
    }

  offset += sc_core::sc_time(10, sc_core::SC_NS);
}

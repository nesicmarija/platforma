#include "hard.hpp"

Hard::Hard(sc_core::sc_module_name name)
  : sc_module(name)
  , width(0)
  , height(0)
  , start(0)
  , ready(0)
  , pivotRow(0)
  , pivotCol(0)
{
  soft_socket.register_b_transport(this, &Hard::b_transport);
  
  SC_REPORT_INFO("Hard", "Constructed.");
}

Hard::~Hard()
{
  SC_REPORT_INFO("Hard", "Destroyed.");
}

void Hard::b_transport(pl_t &pl, sc_core::sc_time &offset)  //zakazuje transakciju, pl je deo transakcije, ono sto prebacujemo, a offset je vreme pocetka
{
  tlm::tlm_command cmd = pl.get_command();  //objekat klase tlm, cmd , 
  sc_dt::uint64 addr = pl.get_address();    //dobija adresu, 
  unsigned int len = pl.get_data_length();  //dobija duzinu podatka
  unsigned char *buf = pl.get_data_ptr();   //pointer na adresu
  pl.set_response_status( tlm::TLM_OK_RESPONSE );    //sve okej

	switch(cmd)   //u zavisnosti od komande
    {
    case tlm::TLM_WRITE_COMMAND:  //write
      switch(addr)
        {
        case ADDR_WIDTH:
          width = to_int(buf)+1;  //pointer pretvori u broj i pomera za 1
          break;

        case ADDR_HEIGHT:
          height = to_int(buf)+1;
          break;
        
        case ADDR_CMD:
          start = to_int(buf);
          doPivoting(wv_fixed,pivotRow,pivotCol,pivot_fixed);
          break;
        default:
          pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
        }
      if (len != BUFF_SIZE) pl.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );  //do kraja bafera
      break;
    case tlm::TLM_READ_COMMAND:  //read
      switch(addr)
        {
        case ADDR_STATUS:
          to_uchar(buf, ready);    //kaze da je spreman, dd
          break;
         
        default:
          pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
        }
      if (len != BUFF_SIZE) pl.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
      break;
    default:
      pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
    }
  offset += sc_core::sc_time(10, sc_core::SC_NS);
}


void Hard::doPivoting(num_t wv[ROWSIZE][COLSIZE],int pivotRow,int pivotCol,num_t pivot)
{
    num_t wv_fixed[ROWSIZE][COLSIZE];
    num_t pivot_fixed;
    num_t newRow[COLSIZE];
    num_t pivotColVal[ROWSIZE];
    num_t nr;
    num_t pcv;
    for(int i=0;i<COLSIZE;i++)
        {
            nr = wv[pivotRow][i]/pivot;
            offset += sc_core::sc_time(10, sc_core::SC_NS);
            newRow[i]= nr;
            offset += sc_core::sc_time(10, sc_core::SC_NS);
        }

        for(int j=0;j<ROWSIZE;j++)
        {
            pcv = wv[j][pivotCol];
            offset += sc_core::sc_time(10, sc_core::SC_NS);
            pivotColVal[j]=pcv;
            offset += sc_core::sc_time(10, sc_core::SC_NS);
        }

        for(int j=0;j<ROWSIZE;j++)
        {
            if(j==pivotRow)
            {
                for(int i=0;i<COLSIZE;i++)
                {
                    wv[j][i]=wv[j][i]/pivot;
                    offset += sc_core::sc_time(10, sc_core::SC_NS);
                }
            }
            else
            {
                for(int i=0;i<COLSIZE;i++)
                {
                    wv[j][i]=wv[j][i]-newRow[i]*pivotColVal[j];
                    offset += sc_core::sc_time(10, sc_core::SC_NS);
                }
            }
        }
}


num_t Hard::read_bram(int addr) 
{
  pl_t pl;
  unsigned char buf[BUFF_SIZE];
  pl.set_address(addr*BUFF_SIZE);
  pl.set_data_length(BUFF_SIZE);
  pl.set_data_ptr(buf);
  pl.set_command( tlm::TLM_READ_COMMAND );
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
  sc_core::sc_time offset = sc_core::SC_ZERO_TIME;

    bram_socket->b_transport(pl, offset);
  
  return to_fixed(buf);
}

void Hard::write_bram(int addr, num_t val)
{
  pl_t pl;
  unsigned char buf[BUFF_SIZE];
  to_uchar(buf,val);
  pl.set_address(addr*BUFF_SIZE);  //postavi adresu u bram na svaka 4 
  pl.set_data_length(BUFF_SIZE);  //velicina podatka=4
  pl.set_data_ptr(buf);      //pointuje na adresu
  pl.set_command( tlm::TLM_WRITE_COMMAND ); //komanda za upis
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );  //ili ne
  
    bram_socket->b_transport(pl, offset);
 
}


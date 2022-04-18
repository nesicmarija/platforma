#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#define SC_INCLUDE_FX
#include <systemc>
#include <tlm>

#define NUMOFVAR 50
#define NUMOFSLACK 50
#define ROWSIZE 51 //(NUMOFSLACK+1)
#define COLSIZE 101 //(NUMOFSLACK+NUMOFVAR+1)
#define WIDTH 40
#define FIXED_POINT 19

typedef sc_dt::sc_fixed_fast <WIDTH, FIXED_POINT> num_t;
typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;


#define ADDR_WIDTH 0x08
#define ADDR_HEIGHT 0x10
#define ADDR_CMD 0x18
#define ADDR_STATUS 0x1c

#define VP_ADDR_BRAM_BASE 0x01000000
#define VP_ADDR_BRAM_L 0x01000000
#define VP_ADDR_BRAM_H 0x01100000
#define VP_ADDR_HARD_BASE 0x02000000
#define VP_ADDR_HARD_L 0x02000008
#define VP_ADDR_HARD_H 0x0200001c

#define MAX_SIZE 64
#define MEM_RESERVED 100000

#endif // TYPEDEFS_HPP

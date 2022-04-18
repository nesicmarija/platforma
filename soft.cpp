#include "soft.hpp"
#include "typedefs.hpp"
#include "utils.hpp"



SC_HAS_PROCESS(Soft);

Soft::Soft(sc_core::sc_module_name name)  
  : sc_module(name)
  , offset(sc_core::SC_ZERO_TIME)
{
  baza.open("baza.txt");
  if(!baza.is_open())
    SC_REPORT_ERROR("Soft", "Cannot open file.");

  SC_THREAD(simplex);
  SC_REPORT_INFO("Soft", "Constructed.");
}

Soft::~Soft()
{
  baza.close();
  SC_REPORT_INFO("Soft", "Destroyed.");
}

void Soft::simplex()
{
<<<<<<< HEAD
     num_t wv_fixed[ROWSIZE][COLSIZE];
     num_t pivot_fixed;

=======
       bool passCheck(const float wv[ROWSIZE][COLSIZE], const float wv_cpy[ROWSIZE][COLSIZE], double delta);
       bool checkOptimality(float wv[ROWSIZE][COLSIZE]);
       bool isUnbounded(float wv[ROWSIZE][COLSIZE],int pivotCol);
       void print(float wv[ROWSIZE][COLSIZE]);
       void matrixZero(float wv[ROWSIZE][COLSIZE]);
       void addOnesDiagonal(float wv[ROWSIZE][COLSIZE]);
       void copyMatrix(float wv_cpy[ROWSIZE][COLSIZE],float wv[ROWSIZE][COLSIZE]);
       int findPivotCol(float wv[ROWSIZE][COLSIZE]);
       int findPivotRow(float wv[ROWSIZE][COLSIZE],int pivotCol);
       void solutions(float wv[ROWSIZE][COLSIZE]);
       void simplexCalculate(float wv[ROWSIZE][COLSIZE]);
       
}
num_t wv_fixed[ROWSIZE][COLSIZE];
num_t pivot_fixed;
>>>>>>> 16f89b657ab3111d2ffa4635d7508bd69c1fe06e

/*
    ---------------------------------------------
    WRITE PART - read from files, write into BRAMs   //MALO PREPRAVITI!!
    ---------------------------------------------
  */
  baza >> COLSIZE >> ROWSIZE;

  for (int i = 0; i<COLSIZE; ++i) // add A
    for (int j = 0; j < ROWSIZE; ++j)
      {
       
            baza >> write_val;
            write_bram(i*ROWSIZE+j,write_val);
        
      }
    //MakeMatrix
    float wv[ROWSIZE][COLSIZE];
	for(int j=0;j<ROWSIZE; j++)
	{
		for(int i =0;i<COLSIZE;i++)
		{
			wv[j][i]=0;
		}
	}

	fstream myFile;
    myFile.open("baza.txt",ios::in); //otvaram fajl u read modu
	if(myFile.is_open())
    {
        for(int j = 0; j < ROWSIZE; j++)
        {
            for(int i = 0; i< NUMOFVAR; i++)
            {
              myFile >> wv[j][i];
            }
        }
		for(int j = 0;j< NUMOFSLACK;j++)
		{
			myFile >> wv[j][COLSIZE-1];
		}
    }
    myFile.close();

	for(int j=0;j<ROWSIZE-1;j++)
	{
		{
			wv[j][NUMOFVAR+j]=1;  
		}
	}
    //CalculateSimplex 
    int pivotRow;
    int pivotCol;
    bool unbounded=false;
    bool optimality=false;
    float pivot;

    while(!optimality)
    {
        //checkOptimality(wv)
        optimality=true;
        for(int i=0;i<COLSIZE-1;i++)
        {
            if(wv[ROWSIZE-1][i]<0)//min> max<
                optimality=false;
        }
        //findPivotCol(wv);
        float minnegval=wv[ROWSIZE-1][0];
        int loc=0;
        for(int i=1;i<COLSIZE-1;i++)
        {
            if(wv[ROWSIZE-1][i]<minnegval)
            {
                minnegval=wv[ROWSIZE-1][i];
                loc=i;
            }
        }
        pivotCol=loc;
        //isUnbounded(wv,pivotCol)
        unbounded=true;
        for(int j=0;j<ROWSIZE-1;j++)
        {
            if(wv[j][pivotCol]>0)
                unbounded=false;
        }
        if(unbounded)
        {
            break;
        }
        //findPivotRow(wv,pivotCol);
        float rat[ROWSIZE-1];
        for(int j=0;j<ROWSIZE-1;j++)
        {
            if(wv[j][pivotCol]>0)
            {
                rat[j]=wv[j][COLSIZE-1]/wv[j][pivotCol];
            }
            else
            {
                rat[j]=0;
            }
        }

        float minpozval=99999999;
        loc=0;
        for(int j=0;j<ROWSIZE-1;j++)
        {
            if(rat[j]>0)
            {
                if(rat[j]<minpozval)
                {
                    minpozval=rat[j];
                    loc=j;
                }
            }
        }
        pivotRow=loc;

        pivot=wv[pivotRow][pivotCol];

        //doPivoting(wv,pivotRow,pivotCol,pivot);
        
        
       /*
    --------------------------------------------
    WRITE PART - write dimensions of A into HARD IP   
    --------------------------------------------
  */

  write_hard(ADDR_WIDTH, width-1); // -1 because thats how the IP was implemented
  write_hard(ADDR_HEIGHT, height-1);

   write_hard(ADDR_CMD, 1);
  int ready = 1;
  while (ready)
    {
      ready = read_hard(ADDR_STATUS);
    }
  write_hard(ADDR_CMD, 0);
  while (!ready)
    {
      ready = read_hard(ADDR_STATUS);
    }

//-------------------------------------------------------------------------------------------------
    //Ispisivanje rezultata
    if(unbounded)
    {
        cout<<"Unbounded"<<endl;
    }
    else
    {
        //solutions(wv);
        for(int i=0;i<NUMOFVAR; i++)  //every basic column has the values, get it form B array
        {
            int count0 = 0;
            int index = 0;
            for(int j=0; j<ROWSIZE-1; j++)
            {
                if(wv[j][i]==0.0)
                {
                    count0 = count0+1;
                }
                else if(wv[j][i]==1)
                {
                    index = j;
                }
            }
            if(count0 == ROWSIZE - 2 )
            {
                cout<<"variable"<<i+1<<": "<<wv[index][COLSIZE-1]<<endl;  //every basic column has the values, get it form B array
            }
            else
            {
                cout<<"variable"<<i+1<<": "<<0<<endl;
            }
        }

        cout<<""<<endl;
        cout<<endl<<"Optimal solution is "<<wv[ROWSIZE-1][COLSIZE-1]<<endl;
    }

    return 0;
}
       
}

  


 //READ: Read data from BRAM
 
 void Soft::read_bram(sc_dt::uint64 addr, num_t &valM)
{
  pl_t pl;
  unsigned char buf[5];
  pl.set_address((addr*5) | VP_ADDR_BRAM_BASE);
  pl.set_data_length(5);
  pl.set_data_ptr(buf);
  pl.set_command( tlm::TLM_READ_COMMAND );
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
  interconnect_socket->b_transport(pl,offset);

  valM = to_fixed(buf);

}

//WRITE: Write data into BRAM

void Soft::write_bram(sc_dt::uint64 addr, num_t valM)
{
  pl_t pl;
  unsigned char buf[5];
  to_uchar(buf,valM);
  pl.set_address((addr*5) | VP_ADDR_BRAM_BASE);
  pl.set_data_length(5);
  pl.set_data_ptr(buf);
  pl.set_command( tlm::TLM_WRITE_COMMAND );
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
  interconnect_socket->b_transport(pl,offset);
}


int Soft::read_hard(sc_dt::uint64 addr)
{
  pl_t pl;
  unsigned char buf[5];
  pl.set_address((addr*5) | VP_ADDR_HARD_BASE);
  pl.set_data_length(5);
  pl.set_data_ptr(buf);
  pl.set_command( tlm::TLM_READ_COMMAND );
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
  sc_core::sc_time offset = sc_core::SC_ZERO_TIME;
  interconnect_socket->b_transport(pl,offset);
  return to_int(buf);
}

void Soft::write_hard(sc_dt::uint64 addr, int val)
{
  pl_t pl;
  unsigned char buf[5];
  to_uchar (buf, val);
  pl.set_address((addr*5) | VP_ADDR_HARD_BASE);
  pl.set_data_length(5);
  pl.set_data_ptr(buf);
  pl.set_command( tlm::TLM_WRITE_COMMAND );
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
  interconnect_socket->b_transport(pl,offset);
}





 
 

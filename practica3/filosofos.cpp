#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>

using namespace std;

void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   if( size!=10)
   {
      if( rank == 0) 
         cout<<"El numero de procesos debe ser 10" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }
   
   if ((rank%2) == 0)  
      Filosofo(rank,size); // Los pares son Filosofos 
   else 
      Tenedor(rank,size);  // Los impares son Tenedores 
   
   MPI_Finalize( );
   return 0;
}  
// ---------------------------------------------------------------------

void Filosofo( int id, int nprocesos )
{
   int izq = (id+1) % nprocesos;
   int der = ((id+nprocesos)-1) % nprocesos;
   int solicitud;
   
   while(1)
   {
      // Solicita tenedor izquierdo

   	  // Como el primer filósofo debe solicitar primero el tenedor derecho
   		if ( id == 0) {	// Si el id es 0
   			cout << "Filosofo 1 solicita tenedor der ..." << der << endl << flush;
   			MPI_Ssend( &solicitud, 1, MPI_INT, der, 0, MPI_COMM_WORLD);
   		} else {
   			cout << "Filósofo " << id << " solicita tenedor izq ..." << izq << endl << flush;
   			MPI_Ssend( &solicitud, 1, MPI_INT, izq, 0, MPI_COMM_WORLD);
   		}

   		if ( id == 0 ) {	// Si es el primer fílósofo
   			cout << "Filósofo " << id << " solicita tenedor izq ..." << izq << endl<< flush;
   			MPI_Ssend( &solicitud, 1, MPI_INT, izq, 0, MPI_COMM_WORLD);
   		} else {
   			cout << "Filósofo " << id << " solicita tenedor der ..." << der << endl <<flush;
   			MPI_Ssend( &solicitud, 1, MPI_INT, der, 0, MPI_COMM_WORLD);
   		}

   		cout << "Filosofo " << id << " COMIENDO" << endl << flush;
   		slee
      	sleep((rand() % 3)+1);  //comiendo
      
      // Suelta el tenedor izquierdo
      cout <<"Filosofo "<<id<< " suelta tenedor izq ..." << izq << endl << flush;
      // ...
      
      // Suelta el tenedor derecho
      cout <<"Filosofo "<<id<< " suelta tenedor der ..." << der << endl << flush;
      // ...
      
      // Piensa (espera bloqueada aleatorio del proceso)
      cout << "Filosofo " << id << " PENSANDO" << endl << flush;
      
      // espera bloqueado durante un intervalo de tiempo aleatorio 
      // (entre una décima de segundo y un segundo)
      usleep( 1000U * (100U+(rand()%900U)) );
 }
}
// ---------------------------------------------------------------------

void Tenedor(int id, int nprocesos)
{
  int buf; 
  MPI_Status status; 
  int Filo;

   int izq = (id+1) % nprocesos;
   int der = ((id+nprocesos)-1) % nprocesos;  
  
  while( true )
  {
    // Espera un peticion desde cualquier filosofo vecino ...
    // ...
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    if (status.MPI_SOURCE)
    // Recibe la peticion del filosofo ...
    // ...
    
    cout << "Ten. " << id << " recibe petic. de " << Filo << endl << flush;
    
    // Espera a que el filosofo suelte el tenedor...
    // ...
    cout << "Ten. " << id << " recibe liberac. de " << Filo << endl << flush; 
  }
}
// ---------------------------------------------------------------------

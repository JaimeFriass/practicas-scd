#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>
#define soltar     0
#define coger      1

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
   
   // Comprobamos que se introducen los correspondientes procesos
   if( size!=10)
   {
      if( rank == 0) 
         cout<<"El numero de procesos debe ser 10" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }
   
   if ((rank%2) == 0)
   	  Filosofo(rank, size); // Los pares son Filosofos 
   else 
      Tenedor(rank,size);  // Los impares son Tenedores 
   
   MPI_Finalize( );
   return 0;
}  
// ---------------------------------------------------------------------

void Filosofo( int id, int nprocesos )
{
   int izq = (id+1) % 10;
   int der = ((id+10)-1) % 10;
   int solicitud;
   
   while(1)
   {
   		solicitud = 1;
   	    // Como el primer filósofo debe solicitar primero el tenedor derecho
   		if ( id == 0) {	// Si es el primer filósofo

   			cout << "Filosofo " << id << " solicita tenedor der ..." << der << endl << flush;
   			MPI_Ssend( &solicitud, 0, MPI_INT, der, coger, MPI_COMM_WORLD);	// Envia solicitud 1 a der

   		} else {

   			cout << "Filósofo " << id << " solicita tenedor izq ..." << izq << endl << flush;
   			MPI_Ssend( &solicitud, 0, MPI_INT, izq, coger, MPI_COMM_WORLD); // Envia solicitud 1 a izq

   		}

   		if ( id == 0 ) {	// Si es el primer fílósofo

   			cout << "Filósofo " << id << " solicita tenedor izq ..." << izq << endl<< flush;
   			MPI_Ssend( &solicitud, 0, MPI_INT, izq, coger, MPI_COMM_WORLD);  // Envia solicitud 1 a izq

   		} else {

   			cout << "Filósofo " << id << " solicita tenedor der ..." << der << endl <<flush;
   			MPI_Ssend( &solicitud, 0, MPI_INT, der, coger, MPI_COMM_WORLD);	// Envia solicitud 1 a der

   		}

   		// Empieza a comer
   		cout << "Filosofo " << id << " COMIENDO" << endl << flush;
      	sleep((rand() % 3)+1);	// Intervalo de tiempo aleatorio
      	solicitud = 0;	// Se cambia la solicitud
      	cout << "Filósofo " << id << " TERMINA DE COMER" << endl << flush;

      	cout << "Filósofo " << id << " suelta tenedor izq " << izq << endl << flush;
      	MPI_Ssend( &solicitud, 0, MPI_INT, izq, soltar, MPI_COMM_WORLD);

      	cout << "Filósofo " << id << " suelta tenedor der ..." << der << endl << flush;
      	MPI_Ssend( &solicitud, 0, MPI_INT, der, soltar, MPI_COMM_WORLD);

      	// PIENSA
        cout << "Filosofo " << id << " PENSANDO" << BLACK << endl;
        sleep((rand()%3)+1);

 }
}
// ---------------------------------------------------------------------

void Tenedor(int id, int nprocesos)
{
  int buf; 
  MPI_Status status; 
  int Filosofo;

   int izq = (id+1) % nprocesos;
   int der = ((id+nprocesos)-1) % nprocesos;  
  
  while( true )
  {
    // Espera un peticion desde cualquier filosofo vecino ...
    MPI_Recv( &buf, 1, MPI_INT, MPI_ANY_SOURCE, coger, MPI_COMM_WORLD, &status);

    // Recibe la peticion del filosofo ...
    Filosofo = status.MPI_SOURCE;
    cout << "Ten. " << id << " recibe petic. de " << Filosofo << endl << flush;
    
    // Espera a que el filosofo suelte el tenedor...
    MPI_Recv(&Filosofo, 1, MPI_INT, Filosofo, soltar, MPI_COMM_WORLD, &status);
    cout << "Ten. " << id << " recibe liberac. de " << Filosofo << endl << flush; 
  }
}

// ---------------------------------------------------------------------

#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>
#define Camarero 10
#define soltar     0
#define coger      1
#define sentarse   2
#define levantarse 3

using namespace std;

void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);
void Camarero( int id, int nprocesos);

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   if( size!=11)
   {
      if( rank == 0) 
         cout<<"El numero de procesos debe ser 11" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }
   
   if ((rank%2) == 0) {
   	if (rank == 10)
   		Camarero(); // 10 -> Camarero
   	else
   		Filosofo(rank, size); // Los pares son Filosofos 
   }
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
   
   while(1)
   {
        // Filósofo solicita sentarse
        cout << "Filosofo " << id << " solicita sentarse " << endl;
        MPI_Send(NULL, 0, MPI_INT, camarero, sentarse, MPI_COMM_WORLD);

        // Filósofo espera confirmación
        MPI_Recv(NULL, 0, MPI_INT, camarero, sentarse, MPI_COMM_WORLD, &status);
        // El filosofo se sienta
        cout << "Filosofo " << id << " se sienta " << endl;
        
        // Filósofo solicita tenedor izq
        cout << "Filosofo " << id << " solicita tenedor izq " << izq << BLACK << endl;
        MPI_Ssend(NULL, 0, MPI_INT, izq, coger, MPI_COMM_WORLD);

        // Filósofo solicita tenedor der
        cout << "Filosofo " << id << " coge tenedor der " << der << BLACK << endl;
        MPI_Ssend(NULL, 0, MPI_INT, der, coger, MPI_COMM_WORLD);

        // EMPIEZA A COMER
        cout << "Filosofo " << id << " COMIENDO" << endl;
        sleep((rand()%3)+1); // comiendo
        cout << "Filosofo " << id << " TERMINA DE COMER" << endl;

        // Filosofo suelta el tenedor izq
        cout << "Filosofo "<<id<< " suelta tenedor izq. " << izq << endl;
        MPI_Ssend(NULL, 0, MPI_INT, izq, soltar, MPI_COMM_WORLD);

        // Filosofo suelta el tenedor der
        cout << "Filosofo " << id << " suelta tenedor der. " << der << endl;
        MPI_Ssend(NULL, 0, MPI_INT, der, soltar, MPI_COMM_WORLD);


        // Filosofo se levanta
        cout << "Filosofo " << id << " se levanta " << endl;
        MPI_Ssend(NULL, 0, MPI_INT, camarero, levantarse, MPI_COMM_WORLD );

        // FILOSOFO PIENSA
        cout << "Filosofo " << id << " PENSANDO" << endl;
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
    // Recibe la peticion del filosofo ...
    MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, coger, MPI_COMM_WORLD, &status);
    Filosofo = status.MPI_SOURCE;
    cout << "Ten. " << id << " recibe petic. de " << Filosofo << endl;
    
    // Espera a que el filosofo suelte el tenedor...
    MPI_Recv(&Filosofo, 1, MPI_INT, Filosofo, soltar, MPI_COMM_WORLD, &status);
    cout << "Ten. " << id << " recibe liberac. de " << Filosofo << endl;
  }
}

void Camarero(int id, int nprocesos)
{
	int Filo;
	int tag;
	int n_filo = 0;
	MPI_Status status;

	while(true) {
		if (n_filo < 4) // Si el numero de filosofos es menor de 4
      // Es posible que se siente y se levante
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		else  // Si no
      // Solo es posible irse
			MPI_Probe(MPI_ANY_SOURCE,      2     , MPI_COMM_WORLD, &status);

		// Lee etiqueta
		tag  = status.MPI_TAG;
		cout << "Se recibe " << tag << " del filosofo " << Filosofo << endl;

		if (tag == sentarse) {			// Deja sentarse

      Filo = status.MPI_SOURCE;
			MPI_Recv(NULL, 0, MPI_INT, Filo, sentarse, MPI_COMM_WORLD, &status);
      n_filo++;
      MPI_Send(NULL, 0, MPI_INT, Filo, sentarse, MPI_COMM_WORLD);
			cout << "Filosofo " << Filo << " se sienta. Quedan " << n_filo << " filosofos sentados " << endl;

		} else if (tag == levantarse) {	// Levantarse

      Filo = status.MPI_SOURCE
			MPI_Recv( NULL, 0, MPI_INT, Filo, levantarse, MPI_COMM_WORLD, &status);
      n_filo--;
      cout << "Filosofo " << Filo << " se levanta. Quedan " << n_filo << " filosofos sentados " << endl;

		}
	}
}
// ---------------------------------------------------------------------

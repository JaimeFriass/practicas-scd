// *****************************************************************************
//
// Prácticas de SCD. Práctica 1. Jaime Frías
//
// *****************************************************************************

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std ;
// Se crean los semáforos, utilizamos un array para los fumadores por simplificar el
// código o para modificar este si hubiera más fumadores.
sem_t fumador[3],
      estanquero;
int ingrediente; // Cada ingrediente será asociado a un fumador, ya que cada uno necesita sólo 1.



// ---------------------------------------------------------------------
// introduce un retraso aleatorio de duración comprendida entre
// 'smin' y 'smax' (dados en segundos)

void retraso_aleatorio( const float smin, const float smax )
{
  static bool primera = true ;
  if ( primera )        // si es la primera vez:
  {  srand(time(NULL)); //   inicializar la semilla del generador
     primera = false ;  //   no repetir la inicialización
  }
  // calcular un número de segundos aleatorio, entre {\ttbf smin} y {\ttbf smax}
  const float tsec = smin+(smax-smin)*((float)random()/(float)RAND_MAX);
  // dormir la hebra (los segundos se pasan a microsegundos, multiplicándos por 1 millón)
  usleep( (useconds_t) (tsec*1000000.0)  );
}

// ----------------------------------------------------------------------------
// función que simula la acción de fumar, como un retardo aleatorio de la hebra.
// recibe como parámetro el numero de fumador
// el tiempo que tarda en fumar está entre dos y ocho décimas de segundo.

void  *fumar( void * num_fumador )
{  

   unsigned long argumento = (unsigned long) num_fumador;
   
   while(true) {
      sem_wait(&fumador[argumento]);

      cout << "Fumador número " << argumento << ": comienza a fumar." << endl << flush ;
      sem_post(&estanquero);
      retraso_aleatorio( 0.2, 0.8 );
      cout << "Fumador número " << argumento << ": termina de fumar." << endl << flush ;

   }
}


void * producir( void * ) {
   const string productos[] = {"papel", "cerillas", "tabaco"};  // Array de strings para imprimir en pantalla los ingredientes.
   while(1) {
      sem_wait(&estanquero);
      ingrediente = rand()%3; // Se genera un número aleatorio entre 0 y 2 para elegir un ingrediente
      cout << "Estanquero produce: " << productos[ingrediente] << endl << flush ;
      sem_post(&fumador[ingrediente]);
   }

}

int main()
{
   srand( time(NULL) ); // inicializa semilla aleatoria para selección aleatoria de fumador
      
   pthread_t fumadores[3], estanquero_p;

   for (int i= 0; i < 3; i++) {     // Inicialización de semáforos
      sem_init(&fumador[i], 0, 0);
   }
   sem_init(&estanquero, 0, 1);


   for (unsigned long i = 0; i < 3; i++) {      // Creación de hebras
      void * argumento = (void *) i;      // Al necesitar como argumento un puntero, se convierte el unsigned long a puntero.
      pthread_create(&fumadores[i], NULL, fumar, argumento);
   }
   pthread_create(&estanquero_p, NULL, producir, NULL);

   for (int i = 0; i < 3; i++) {
      pthread_join(fumadores[i], NULL);
   }
   pthread_join(estanquero_p, NULL);

   for (int i = 0; i < 3; i++)
      sem_destroy(&fumador[i]);
   sem_destroy(&estanquero);
   
  return 0 ;
}

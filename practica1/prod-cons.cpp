// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Plantilla de código para el ejercicio del productor-consumidor con
// buffer intermedio.
//
// *****************************************************************************

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // necesario para {\ttbf usleep()}
#include <stdlib.h> // necesario para {\ttbf random()}, {\ttbf srandom()}
#include <time.h>   // necesario para {\ttbf time()}

using namespace std ;

// ---------------------------------------------------------------------
// constantes configurables:

const unsigned
  num_items  = 40 ,    // numero total de items que se producen o consumen
  tam_vector = 10 ;    // tamaño del vector, debe ser menor que el número de items
int buf[tam_vector];
int cont = 0;
sem_t puede_producir;
sem_t puede_consumir;
sem_t mutex;

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

// ---------------------------------------------------------------------
// función que simula la producción de un dato

unsigned producir_dato()
{
  static int contador = 0 ;	
  contador = contador + 1 ;
  retraso_aleatorio( 0.1, 0.5 );
  cout << "Productor : dato producido: " << contador << endl << flush ;
  return contador ;
}
// ---------------------------------------------------------------------
// función que simula la  consumición de un dato

void consumir_dato( int dato )
{
   retraso_aleatorio( 0.1, 1.5 );
   cout << "Consumidor:                              dato consumido: " << dato << endl << flush ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del productor

void * funcion_productor( void * p)
{
  for( unsigned i = 0 ; i < num_items ; i++ )
  {
		 sem_wait(&puede_producir);
		 int dato = producir_dato() ;
		 sem_wait(&mutex);
		 buf[cont] = dato; // Inserta el valor en el buffer
 		 cont++;
		 sem_post(&mutex);
		 cout << "Productor : dato insertado: " << dato << endl << flush ;

	    sem_post(&puede_consumir);

  }
  return NULL ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del consumidor

void * funcion_consumidor( void * p )
{
	int dato;
  for( unsigned i = 0 ; i < num_items ; i++ )
  {
    
    sem_wait(&puede_consumir);
  
	 sem_wait(&mutex);
	 cont--;
	 dato = buf[cont];      // Consigue el dato desde el búfer
 	 sem_post(&mutex);
	 consumir_dato( dato ) ;
	 cout << "Consumidor:" << "                          dato extraído : " << dato << endl << flush ;
	
    sem_post(&puede_producir);
  

  }
  return NULL ;
}
//----------------------------------------------------------------------

int main()
{
  pthread_t hebra_productora, hebra_consumidora;

  sem_init( &mutex,         0, 1 ); // semáforo para EM: inicializado a 1
  sem_init(&puede_producir, 0, tam_vector);  // inicialmente se puede producir
  sem_init(&puede_consumir, 0, 0);  // inicialmente no se puede consumir
  
  pthread_create(&hebra_productora, NULL, funcion_productor, NULL);
  pthread_create(&hebra_consumidora, NULL, funcion_consumidor, NULL);
  
  pthread_join( hebra_productora, NULL);
  pthread_join( hebra_consumidora, NULL);
  
  sem_destroy( &puede_producir);
  sem_destroy( &puede_consumir);
  sem_destroy( &mutex );

   return 0 ;
}

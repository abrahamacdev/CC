#include "ram.h"
#include <iostream>
#include <iomanip>
#include <chrono>

/*
reset(i)        ▶ r[i] ← 0                  Setea a 0 el registro i
inc(i)          ▶ r[i] ← r[i] + 1           Incrementa en 1 lo que haya en el registro i
dec(i)          ▶ r[i] ← r[i] − 1           Decrementa en 1 lo que haya en el registro i
load(i, j)      ▶ r[i] ← m[r[j]]            Carga en el registro i lo que haya en la posición r[j] de la memoria (doble link)
store(i, j)     ▶ m[r[j]] ← r[i]            Almacena en la posición r[j] de la memoria el contenido del registro r[i] (doble link)
cgoto(i, l)     ▶ if r[i] ≥ 0 then pc ← l   Si el contenido del registro r[i] >= 0 => va a la posición l
*/

using namespace std;
using namespace std::chrono;

void program(){
  
  // 2 -> Inicial
  // 1 -> Resto = 1
  // 0 -> Resto = 2

  reset(0);   // Estado actual
  inc(0);
  inc(0);

  reset(1);   // Posicion actual de memoria
  inc(1);

  reset(2);   // Caracter leído
  reset(3);   // Salto incondicional

  read:
    load(2, 1);   
    inc(1);               // Leemos el siguiente caracter y avanzamos la posicion

    //cout << "Caracter leído '" << r[2] << "'" << endl;

    cgoto(2, process);    // Si el simbolo leido es 0 o 1 => procesa
    cgoto(3, end);        // Si no, termina
    
  process:

    dec(0);
    dec(0);
    cgoto(0, process_initial);  // Estamos en el estado inicial (2)

    inc(0);
    cgoto(0, process_mod_one);  // Estamos en el estado resto = 1 (1)

    inc(0);
    cgoto(0, process_mod_two);  // Estamos en el estado resto = 2 (0)

  // Inicial
  process_initial:

    inc(0);
    inc(0);

    //cout << "Estado inicial [" << r[0] << "]" << endl;

    dec(2);
    cgoto(2, dec_one);    // EL símbolo leído era un 1
    cgoto(3, read);       // El símbolo leído era un 0

  // Resto = 1 (central)
  process_mod_one:

    inc(0);

    //cout << "Estado central (mod==1)  [" << r[0] << "]" << endl;

    dec(2);
    cgoto(2, inc_one);    // EL símbolo leído era un 1
    cgoto(3, dec_one);    // El símbolo leído era un 0

  // Resto = 2 (derecha del todo)
  process_mod_two:

    //cout << "Estado derecho (mod==2) [" << r[0] << "]" << endl;

    dec(2);
    cgoto(2, read);       // EL símbolo leído era un 1
    cgoto(3, inc_one);    // El símbolo leído era un 0

  inc_one:
    inc(0);
    cgoto(3, read);

  dec_one:
    dec(0);
    cgoto(3, read);

  end:
    
    //cout << "Estado final " << r[0] << endl;

    dec(0);              
    dec(0);               // Si el estado actual es == inicial (2) => termina true
    cgoto(0, end_true);

    reset(0);           // Estado actual es != inicial (2) => termina false
    store(0, 3);
    return;

  end_true:

    reset(0);
    inc(0);
    store(0, 3);
    return;
}

// Driver program.

memory r, m(1024);

void guardar_cadena(string& cadena){

  int i=0;
  for(auto c : cadena){
    m[i+1] = static_cast<unsigned char>(c-'0');
    i++;
  }

  // We set last position to -1 to indicate end of string
  m[cadena.size() + 1] = -1;
}

int64_t make_test(string& cadena, int64_t N){

  int64_t average = 0;
  int64_t result = 0;

  if (N == 1){
    cout << "Entrada '" << cadena << "'" << endl;
  }
  

  for (size_t i = 0; i < N; i++){

    guardar_cadena(cadena);

    auto beg = high_resolution_clock::now();
    program();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - beg);

    average += duration.count();
  }
  
  if (N == 1){
    cout << "Salida para '" << cadena << "' = " << m[0] << endl;
    cout << "-------------------" << endl;
  }

  //cout << "Resultado promedio de '" << cadena << "' --> " << (result/N) << endl;
  return average / N;
}

string& generar_cadena(int64_t n){

  string& cadena = *(new string());

  int64_t middle = n/2;

  for (size_t i = 0; i < n; i++){
    if(i < middle) cadena += '1';
    else cadena += '0';
  }
  
  //cout << n << " -- " << cadena << endl;

  return cadena;
}

vector<string>& generar_cadenas(vector<int64_t> tamanios_cadenas){

  int64_t n_cadena = 1;

  vector<string>& cadenas = *(new vector<string>());

  for(auto tamanio : tamanios_cadenas){
    cadenas.push_back(generar_cadena(tamanio));
  }

  return cadenas;
}


int main() {

  /**
   * The alphabet that i will use for this program will be: 0 and 1.
   * The expected output will be 0 if there are mores 0's than 1's or viceversa. Otherwise it return 1.
   *   
  */

  // Tests to verify the actual output is equal to the expected one
  /*
  // RAM initialization.
  initialize(4);		// Four registers.

  vector<string> CADENAS = {"0", "11", "110", "1011", "10101", "101110", "1001010", "10110010", "101100111", "1011010010"};
  vector<int64_t> tiempos = {};

  for (auto cadena : CADENAS){
    tiempos.push_back(make_test(cadena, 1));
  }

  for (auto t : tiempos){
    cout << t << ", ";
  }
  cout << endl;
  */


 // Test to see time taken to process input
 // RAM initialization.
  initialize(4);		// Four registers.
  m = vector<integer>(10e5);

  int64_t N = 1000;
  vector<int64_t> tamanios = vector<int64_t>({10, 50, 100, 250, 500, 750, 1000, 2000, 3000, 5000});
  //vector<int64_t> tamanios = vector<int64_t>({10});
  vector<string>& cadenas = generar_cadenas(tamanios);

  vector<int64_t> medias;
  for(auto c : cadenas){
    medias.push_back(make_test(c, N));
  }

  // Cronometramos el tiempo de procesar cada ejemplar
  for(auto v : medias) cout << v << ", ";
  cout << endl;
}

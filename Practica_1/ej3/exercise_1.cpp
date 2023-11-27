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
  
  reset(0);   // Contador de 0's y 1's
  reset(1);   // Posición del símbolo a leer (inicialmente m[1])
  inc(1);
  reset(2);   // Símbolo leído
  reset(3);

  read:     // Lee el siguiente símbolo de la memoria
    load(2, 1);  // Lee el símbolo
    inc(1);      // Aumenta en 1 la posición del siguiente que buscaremos

    //cout << "Simbolo leído " << r[2] << endl;

    // Si el leído es 0 o 1, lo procesamos
    cgoto(2, process);

    // Si no, acabamos
    reset(3);
    cgoto(3, postprocess);
    
  
  process_zero:
    dec(0);

    // Leemos el siguiente símbolo
    reset(3);
    cgoto(3, read);

  process_one:
    inc(0);

    // Leemos el siguiente símbolo
    reset(3);
    cgoto(3, read);

  process:
    dec(2);   // Decrementamos en 1 el valor leído para diferenciar 0 de 1 con la function cgoto
    
    // Si r[2] == 0 => el símbolo leído era 1
    cgoto(2, process_one);

    // Si no => el símbolo leído era 0
    reset(3);
    cgoto(3, process_zero);


  postprocess:

    dec(0);    // Más 1's
    cgoto(0, end_false);

    inc(0);    // Iguales 
    cgoto(0, end_true);

    reset(0); 
    cgoto(0, end_false)  // Más 0's

  end_true:
    reset(0);
    inc(0);
    
    reset(3);
    store(0, 3);

    //cout << "El valor de r0 es " << r[0] << endl;
    //cout << "El valor de m0 es " << m[0] << endl;

    return;

  end_false:

    reset(0);

    reset(3);
    store(0, 3);

    //cout << "El valor de r0 es " << r[0] << endl;
    //cout << "El valor de m0 es " << m[0] << endl;

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

  for (size_t i = 0; i < N; i++){

    guardar_cadena(cadena);

    auto beg = high_resolution_clock::now();
    program();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - beg);

    average += duration.count();
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

  // Data input through the standard input.
  /*
  string s = "";
  cout << "Input string? ";
  getline(cin, s);
  */

  // RAM initialization.
  initialize(4);		// Four registers.
  m = vector<integer>(10e7);


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

  /*
  // RAM data output (memory reading).
  integer l = m[0];

  // Data output through the standard output. 
  cout << "Representation: ";
  for (string::size_type i = 1; i != s.size() + 2; ++i) {
    cout << setw(3) << setfill('0') << m[i] << ' ';
  }

  cout << "\nOutput " << l << '.' << endl;  
  */
}

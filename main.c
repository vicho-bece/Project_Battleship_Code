#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "Map.h"
#include "list.h"

//Estructura para los tableros de ataque y posicion, cuyo "nombre" hace la diferencia para el usuario y la computadora...
typedef struct{
  char tablero [10][10];
  char* nombre;
}terreno;

//Estructura para registrar los puntajes cuando uno gana la partida...
typedef struct{
  int puntuacion;
  char* capitan;
}puntajes;

//Estructura para el registro de ataques...
typedef struct{
  int a;
  int b;
  int c;
  int fallos;
  char* nombre;
}turnos;

//FUNCIONES APARTES
int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2) == 0) return 1;
    return 0;
}

int is_equal_int(void * key1, void * key2) {
    if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

//Es para ordenar de mayor a menor, se modifico desde el mismo "lower_than_int"
int more_than_int(void * key1, void * key2) {
    if(*(int*)key1 > *(int*)key2) return 1;
    return 0;
}

//OPCIONES DEL MENU
void jugarPARTIDA(Map*, Map*, Map*);
void mostrarPUNTAJES(Map*);
void menu();

//OPCIONES DE JUGAR PARTIDA
void nuevaPARTIDA(Map*, Map*, Map*);
void cargarPARTIDA(Map*, Map*);
void menu1();

//OPCIONES DE MOSTRAR PUNTAJES
void cargarPUNTAJES(Map*);
void guardarPUNTAJES(Map*);
void borrarPuntajes();
void menu2();
void show(Map*);
int conversion(char *);

//FUNCIONES APARTES DE NUEVA PARTIDA
void agregarBARCOS(int, int, terreno*, int, int, int);
void crearMATRIZ(terreno*);
void barcos(Map*);
void barcosMAQUINA(Map*);
bool vertical(int, int, terreno*);
bool barcosLibres(int, int, terreno*, int, int, int);
bool medida(int, int, int, int, int);

//FUNCIONES APARTES DURANTE LA PARTIDA
void partida(Map*, Map*, Map*);
bool finish(terreno*);
void attack(terreno*, terreno*, turnos*, int, int);
void attack_tablero(terreno*);
void guardarPARTIDA(Map*, Map*);
void nuevoRECORD(Map*, Map*);
void menu3();

//MENU DE OPCIONES...
int main()
{
  //Se declara los mapas para cada uso...
  Map* tableros = createMap(is_equal_string);//Guardar talberos de flota y ataque
  Map* scores = createMap(is_equal_int);//Guardar los puntajes desde el archivo y/o del juego
  Map* registro = createMap(is_equal_string);//Guardar los ataques realizados
  setSortFunction(scores, more_than_int);//Ordenar los puntajes de mayor a menor
  
  //Variable tipo char para evitar bucles cuando ingresa una letra en vez un numero. Solo debe ingresar un solo caracter
  char num[2];
  menu();
  scanf("%1s", num);  
  
  //Verificar si es un numero, en caso contrario se le pide ingresar otro...
  while(isdigit(num[0]) == 0)
  {
    printf("\nOpción no válida, elija otra.\nOPCION = ");
    scanf("%1s", num);
  }

  //1 = Jugar Una Partida
  //2 = Mostrar Los Puntajes
  //3 = Para Salir Del Juego
  //Cualquier numero distinto a los anteriores = Ingresar uno de esos 3
  while(num[0] != '3')
  {
    switch(num[0])
    {
      case '1': jugarPARTIDA(tableros, scores, registro); break;
      case '2': mostrarPUNTAJES(scores); break;
      default: printf("\nLa opcion escogida no es valida. Favor de ingresar una dentro del rango\n\nOPCION = "); break;
    }
    //Una vez que se termina una de las 2 operaciones, vuelve para que el usuario haga otra operacion del menu
    system("clear");
    menu();
    scanf("%1s", num);
    
    while(isdigit(num[0]) == 0)
    {
      printf("\nOpción no válida, elija otra.\nOPCION = ");
      scanf("%1s", num);
    }
  }

  printf("\nUsted ha finalizado la aplicacion. Muchas gracias por jugar y hasta luego...");
  
  return 0;
}

//OPCIONES DE JUGAR PARTIDA...
void jugarPARTIDA(Map* tableros, Map* scores, Map* registro)
{
  system("clear");

  char num[2];
  menu1();
  scanf("%1s", num);
  
  while(isdigit(num[0]) == 0)
  {
    printf("\nOpción no válida, elija otra.\nOPCION = ");
    scanf("%1s", num);
  }
  
  while(num[0] != '3')
  {
    //Se abre el archivo en modo de LECTURA que contiene el progreso de una partida para verificar el contenido
    FILE* archivo = fopen("progreso.txt", "r");
    if(archivo == NULL)
    {
      printf("Error al abrir el archivo, se forzara el cierre del juego...");
      exit(EXIT_FAILURE);
    }
    //Se situa el cursor al final del archivo
    fseek(archivo, 0, SEEK_END);
    
    switch(num[0])
    {
      //NUEVA PARTIDA
      case '1':
        //Verifica si el archivo NO esta vacio...
        if(ftell(archivo) != 0)
        {
          //Antes de crear una nueva partida, se pregunta al usuario si desea borrar el progreso...
          printf("Al parecer dejaste una partida sin finalizar.\n¿Desea borrar el progreso igualmente?\n\n1 = SI (Lo llevara de inmediato a la nueva partida)\n2 = NO (Seguira en este menu)\n\nOPCION = ");

          getchar();
          scanf("%1[^\n]s", num);
          
          while(num[0] != '1' && num[0] != '2')
          {
            printf("\nOpcion no valido.\n\nOPCION = ");
            scanf("%1[^\n]s", num);
          }

          //Al querrer borrar el progreso, se cierra el archivo para abrirlo en modo ESCRITURA...
          if(num[0] == '1')
          {
            fclose(archivo);
            archivo = fopen("progreso.txt", "w");
            //Situa el curso al inicio del archivo para borrar todo el contenido...
            fseek(archivo, 0, SEEK_SET);

            fclose(archivo);

            //Se inicia una nueva partida...
            nuevaPARTIDA(tableros, scores, registro);
            break;
          }
          else
          {
            //En caso contrario se sale del ciclo "switch"
            fclose(archivo);
            system("clear");
            break;
          }
        }
        else
        {
          //Sie el archivo esta vacio, se inicia una nueva partida de inmediato
          fclose(archivo);
          nuevaPARTIDA(tableros, scores, registro); 
          break;
        }

      //CARGAR PARTIDA
      case '2':
        //Pregunta si el archivo NO esta vacio
        if(ftell(archivo) != 0)
        {
          //Se carga el contenido  en los mapas y reanuda la partida anterior...
          fclose(archivo);
          cargarPARTIDA(tableros, registro);
          partida(tableros, scores, registro); 
          break;
        }
        else
        {
          //No hay progreso
          printf("\nNo hay una partida guardada anteriormente...\n\n");
          fclose(archivo);
          break;
        }

      //Opcion no valido
      default:
        printf("\nOpcion no valido...\n\n");
        break;
    }
    menu1();
    scanf("%1s", num);
    
    while(isdigit(num[0]) == 0)
    {
      printf("\nOpción no válida, elija otra.\nOPCION = ");
      scanf("%1s", num);
    }
  }
}

//OPCIONES DE MOSTRAR LOS PUNTAJES...
void mostrarPUNTAJES(Map* score)
{
  //Se muestra los puntajes guardados durante la ejecucion...
  puntajes* datos = firstMap(score);
  system("clear");
  
  //Pregunta si hay puntajes guardados...
  if(datos != NULL)
  {
    //Se muestra todos los puntajes guardados
    printf("Estos son los puntajes que sean obtenido despues de ganar una partida...\n");

    show(score);
  }
  else
    printf("\nNo hay datos de los puntajes. Si has guardados los puntajes anteriormente, ingrese la opcion de cargar los puntajes guardados...\n\n");

  menu2();
  char num[2];
  scanf("%1s", num);
  
  while(isdigit(num[0]) == 0)
  {
    printf("\nOpción no válida, elija otra5:\n");
    scanf("%1s", num);
  }

  while(num[0] != '4')
  {
    switch(num[0])
    {
      case '1': cargarPUNTAJES(score); break;
      case '2': guardarPUNTAJES(score); break;
      case '3': borrarPuntajes(); break;
      default: printf("Ingreso una opcion no valida.\n\n"); break;
    }
    
    show(score);
    menu2();
    scanf("%1s", num);
    
    while(isdigit(num[0]) == 0)
    {
      printf("\nOpción no válida, elija otra6:\n");
      scanf("%1s", num);
    }
  }
  system("clear");
}

//Muestra los puntajes almacenados en el mapa....
void show(Map* score)
{
  puntajes* datos = firstMap(score);
  while(datos != NULL)
  {
    printf("%i %s\n", datos->puntuacion, datos->capitan);
    datos = nextMap(score);
  }
}

//Cargar los puntajes desde el archivo score.txt
void cargarPUNTAJES(Map* score)
{
  //Se abre en modo de lectura
  FILE* points = fopen("score.txt", "r");

  if(points == NULL)
  {
    printf("Error al abrir el archivo, se forzara el cierre del juego...");
    exit(EXIT_FAILURE);
  }

  system("clear");

  //Situa el cursor al final del archivo para verificar si esta vacio...
  fseek(points, 0, SEEK_END);
  if(ftell(points) != 0)
  {
    //Se situa al comienzo del archivo para leer todo el contenido hasta el EOF
    fseek(points, 0, SEEK_SET);
    char linea[30];
    int i, k, tipoINT;
    puntajes* loading;

    while( fscanf(points, "%39[^\n]s", linea) != EOF)
    {
      fgetc(points);

      char capitan[21];
      char numero[10];
      loading = malloc(sizeof(puntajes));
      i = 0;

      //Se lee el nombre del Capitan...
      for(; linea[i] != ' ' && i < 19 ; i++)
        capitan[i] = linea[i];
      capitan[i] = '\0';

      loading->capitan = strdup(capitan);
      i++;

      //Se lee el numero del Puntaje
      for(k = 0; linea[i] != EOF && k < 8; i++, k++)
        numero[k] = linea[i];
      numero[k] = '\0';

      loading->puntuacion = conversion(numero);

      insertMap(score, &loading->puntuacion, loading);
    }
  }
  else
    printf("\nNo hay puntajes guardados, el archivo se encuentra vacio :(...\n\n");

  fclose(points);
}

int conversion(char *numero)
{
  int i;
  int digito, decimas, aux;
  int suma = 0, potencia = -1;

  //Calculo la cantidad de digitos que ocupa el char...
  for(i = 0; numero[i] != '\0'; i++)
    potencia++;

  aux = potencia;

  if(potencia == 0)
  {
    //El caso de que el numero sea de un digito...
    digito = numero[0] - 48;
    return digito;
  }
  else
  {
    //El caso de que el numero sea mayor o igual a 2 digitos..
    for(int k = 0; k < i; k++)
    {
      decimas = 1;
      digito = numero[k] - 48;

      //Ajusto las decimas dependiendo la posicion del string...
      for(;aux > 0; aux--)
        decimas *= 10;

      digito *= decimas;
      suma += digito;
      potencia--;
      aux = potencia;
    }

    return suma;
  }
}

void guardarPUNTAJES(Map* score)
{
  //Se abre en modo ADICION, situa al final del archivo...
  FILE* points = fopen("score.txt", "a");
  if(points == NULL)
  {
    printf("Error al abrir el archivo. Se forzara el cierre del juego...");
    exit(EXIT_FAILURE);
  }
  system("clear");

  //Verifica si contenido en el mapa de puntajes...
  puntajes* datos = firstMap(score);
  if(datos != NULL)
  {
    //Recorre el mapa...
    while(datos != NULL)
    {
      fputs(datos->capitan, points);
      fputc(' ', points);
      fprintf(points, "%i", datos->puntuacion);
      fputc('\n', points);
      datos = nextMap(score);
    }
  }
  else
    printf("\nNo hay puntajes guardados, debido a que no has ganado una partida todavia...\n\n");

  fclose(points);
}

void borrarPuntajes()
{
  //Se abre en modo de lectura...
  FILE* points = fopen("score.txt", "r");
  if(points == NULL)
  {
    printf("Error al abrir el archivo, se forzara el cierre del juego...");
    exit(EXIT_FAILURE);
  }
  system("clear");

  //Se verifica el contenido...
  fseek(points, 0, SEEK_END);
  if(ftell(points) != 0)
  {
    //Se cierra para abrirlo en modo escritura y situa el curso al inicio...
    fclose(points);
    points = fopen("score.txt", "w");
    fseek(points, 0, SEEK_SET);
    
    printf("\nSe borro el contenido de los puntajes en el archivo de forma exitosa...\n\n");
  }
  else
    printf("\nEl archivo donde se guardan puntajes se encuentra vacio...\n\n");
  

  fclose(points);
}

void nuevaPARTIDA(Map* tableros, Map* scores, Map* registro)
{
  system("clear");

  int i, j, num;

  //Crear las 4 matriz, 2 de ellas para posicionar flotas y las otras el registro de ataque...
  for(i = 0; i < 4; i++)
  {
    terreno* matriz = malloc(sizeof(terreno));

    switch(i)
    {
      case 0: matriz->nombre = strdup("user"); break; 
      case 1: matriz->nombre = strdup("enemy"); break; 
      case 2: matriz->nombre = strdup("userATTACK"); break;
      case 3: matriz->nombre = strdup("enemyATTACK"); break;
    }
    
    //Se pobla la matriz con ' '...
    crearMATRIZ(matriz);
    insertMap(tableros, matriz->nombre, matriz); 
  }
  //Funcion para evitar numeros aleatorios repetidos...
  srand(time(NULL));

  //Funcion para asignar barcos del usuario (manual) y del computador (aleatorio)
  barcos(tableros);
  barcosMAQUINA(tableros);

  //Crear 2 registros de ataques del usuario y pc...
  for(i = 0; i < 2; i++)
  {
    turnos* map = malloc(sizeof(turnos));
    
    map->fallos = 0;
    map->a = 0;
    map->b = 0;
    map->c = 0;

    if(i == 0)
      map->nombre = strdup("user");
  
    if(i == 1)
      map->nombre = strdup("enemy");
      
    insertMap(registro, map->nombre, map);
  }
  //Da inicio la partida
  partida(tableros, scores, registro);
}

//Funcion para poblar la matriz
void crearMATRIZ(terreno* matriz)
{
  int i, j;

  for(i = 0; i <= 9; i++)
    for(j = 0; j <= 9; j++)
      matriz->tablero[i][j] = ' ';
}

//Funcion para que el usuario posicione sus barcos
void barcos(Map* tableros)
{
  terreno *p = searchMap(tableros, "user");
  int cdx, cdy, i, j, k = 3, cdx2, cdy2;
  
  //Imprime el tablero del usuario (de los barcos)
  printf("Este es tu tablero Usuario(a)...\n\n");
  printf("   0  1  2  3  4  5  6  7  8  9\n");

  for(i = 0; i < 10; i++)
  {
    printf("%i ", i);

    for(j = 0; j < 10; j++)
      printf("[%c]", p->tablero[i][j]);

    printf("\n");
  }

  //La K representa el tamano del barco. 3, 4 y 5 
  while(k <= 5)
  {
    //Indica las coordenadas para la punta inicial del barco
    printf("\n\nAgregue coordenada de inicio para barco de %i casillas.\nX = ", k);
    scanf("%d", &cdy);
    printf("Y = ");
    scanf("%d", &cdx);

    //Indica las coordenadas para la punta final del barco
    printf("\n\nAgregue coordenadas finales para barco de %i casillas.\nX = ", k);
    scanf("%d", &cdy2);
    printf("Y = ");
    scanf("%d", &cdx2);
    
    //Se pregunta si las supuestas casillas estan ocupadas y las medidas del barco sean correctas
    while((barcosLibres(cdx,cdy,p,k,cdx2,cdy2) == false) || medida(cdx, cdy, cdx2, cdy2, k))
    {
      //Se le pide ingresar nuevamente las coordenadas de inicio y final de un barco
      printf("\n¡Comandante!, El barco que desea poner esta interceptando con otro barco, se encuentra fuera del rango o lo esta posicionando de manera incorrecta. Favor de ingresar otra.\n");

      printf("\n\nAgregue coordenada de inicio para barco de %i casillas.\nX = ", k);
      scanf("%d", &cdy);
      printf("Y = ");
      scanf("%d", &cdx);

      printf("\n\nAgregue coordenadas finales para barco de %i casillas.\nX = ", k);
      scanf("%d", &cdy2);
      printf("Y = ");
      scanf("%d", &cdx2);
      
    }
    system("clear");

    //Se coloca el barco en la matriz...
    agregarBARCOS(cdx,cdy,p,k,cdx2,cdy2);
    
    k++;
  }
  system("clear");
}

//Funcion para verificar si el barco es "deforme"...
bool medida(int x, int y, int x2, int y2, int k)
{
  int ejeX, ejeY;

  //Se hace la diferencia en cada eje...
  ejeX = x - x2;
  ejeY = y - y2;

  //Se ajusta el signo...
  if(ejeY < 0) ejeY *= -1;
  if(ejeX < 0) ejeX *= -1;

  //Pregunta si el barco es una linea...
  if(ejeX == 0 && ejeY + 1 == k) return false;
  if(ejeX + 1 == k && ejeY == 0) return false;

  //Es un barco diagonal o deforme...
  return true;
}

//Verifica si las casillas que ocupan el barco estan disponibles
bool barcosLibres(int x, int y, terreno* p, int k, int x2, int y2)
{
  int i;
  //Se pregunta dependiendo los valores de los X's e Y's. Para evitar que se salga de la matriz/tablero y para verificar si hay espacio

  if(x > x2) for(i = 0; i < k; i++) if(p->tablero[x-i][y] != ' ') return false;

  if(x < x2) for(i = 0; i < k; i++) if(p->tablero[x+i][y] != ' ') return false;
  
  if(y > y2) for(i = 0; i < k; i++) if(p->tablero[x][y-i] != ' ') return false;
  
  if(y < y2) for(i = 0; i < k; i++) if(p->tablero[x][y+i] != ' ') return false;
  
  return true;
}

//Funcion para agregar los barcos...
void agregarBARCOS(int x, int y, terreno* p, int k, int x2, int y2)
{
  int i, j, c;
  
  //Se posiciona el barco marcando en las casilla segun el tamano...
  for(i = 0 ; i <= 9 ; i++)
  {
    for(j = 0 ; j <= 9 ; j++)
    {
      if(i == x && j == y)
      {
        switch(k)
        {
          case 3:
          
          if(x > x2){ for(c = 0; c < 3; c++) p->tablero [i-c][j] = 'A'; break;}
            
          if(x < x2){ for(c = 0; c < 3; c++) p->tablero [i+c][j] = 'A'; break;}
            
          if(y > y2){ for(c = 0; c < 3; c++) p->tablero [i][j-c] = 'A'; break;}
           
          if(y < y2){ for(c = 0; c < 3; c++) p->tablero [i][j+c] = 'A'; break;}
          
          case 4:
          
          if(x > x2){ for(c = 0; c < 4; c++) p->tablero [i-c][j] = 'B'; break;}
            
          if(x < x2){ for(c = 0; c < 4; c++) p->tablero [i+c][j] = 'B'; break;}
           
          if(y > y2){ for(c = 0; c < 4; c++) p->tablero [i][j-c] = 'B'; break;}
           
          if(y < y2){ for(c = 0; c < 4; c++) p->tablero [i][j+c] = 'B'; break;}
            
          case 5:
          
          if(x > x2){ for(c = 0; c < 5; c++) p->tablero [i-c][j] = 'C'; break;}
            
          if(x < x2){ for(c = 0; c < 5; c++) p->tablero [i+c][j] = 'C'; break;}
            
          if(y > y2){ for(c = 0; c < 5; c++) p->tablero [i][j-c] = 'C'; break;}
            
          if(y < y2){ for(c = 0; c < 5; c++) p->tablero [i][j+c] = 'C'; break;}
            
        }
        
      }
    }
  }
  //Se informa al usuario el resultado del barco...
  printf("\n");
  printf("   0  1  2  3  4  5  6  7  8  9\n");
  for(i = 0; i < 10; i++)
  {
    printf("%i ", i);

    for(j = 0; j < 10; j++)
      printf("[%c]", p->tablero[i][j]);

    printf("\n");
  }
  printf("\n");
}

//Funcion para asignar los barcos del pc 
void barcosMAQUINA(Map* tableros){
  terreno* p = searchMap(tableros, "enemy");
  int x, y, i, j, aux;

  //Posicionar el barco de tamano 3 en forma horizontal, restrigiendo en las columnas. Se escribe de izquierda a derecha...
  x = rand() % 10;
  y = rand() % 8;
  aux = x;

  for( i=0 ; i<=9 ; i++)
  {  
    for( j=0 ; j<=9 ; j++)
    {
      
      if(i == x){
        
        if(j == y){
          for(int k = 0; k < 3; k++){
            
            p->tablero [i][j+k] = 'A';
          
          }
        
        }
      }
    }
  }

  //Posicionar el barco de tamano 5 en forma horizontal, restrigiendo en las columnas. Se escribe de izquierda a derecha...
  x = rand() % 10;
  y = rand() % 6;

  //Se restringe la fila que se ingreso para el barco de tamano 3 para evitar intercepciones...
  while(x == aux)
    x = rand() % 10;

  for( i=0 ; i<=9 ; i++)
  {
    for( j=0 ; j<=9 ; j++)
    {
      
      if(i == x){
        
        if(j == y){
          
          for(int k = 0; k < 5; k++){
            
            p->tablero [i][j+k] = 'C';
          
          }
        }
      }
    }
  }

  //Posicionar el barco de tamano 4 en forma vertical, restrigiendo en las filas. Se escribe de arriba hacia abajo...
  x = rand() % 7;
  y = rand() % 10;
  
  //Revisa si no se intercepta con uno o 2 barcos...
  while(vertical(x,y,p) == false)
  {
    x = rand() % 7;
    y = rand() % 10;
  }

  for( i=0 ; i<=9 ; i++)
  {  
    for( j=0 ; j<=9 ; j++)
    {
      if(i == x){
        
        if(j == y){
          
          for(int k = 0; k < 4; k++){
            
            p->tablero [i+k][j] = 'B';
          
          }
          
        }
      }
    }
  }
}

//Funcion para verificar que estan libres los espacion para el barco vertical...
bool vertical(int x, int y, terreno* p)
{
  int i;
  for(i = 0; i < 4; i++)
  {
    if(p->tablero[x+i][y] == 'A' || p->tablero[x+i][y] == 'C')
      return false;
  }
  return true;
}

//Funcion para cargar una partida iniciada anteriormente...
void cargarPARTIDA(Map* tableros, Map* registro)
{
  system("clear");

  //Se el archivo en modo de LECTURA
  FILE* archivo = fopen("progreso.txt", "r");
  char name[12];
  char linea[114];
  char num[3];

  //Se declara las variables para el mapa de tablero y registro
  terreno* data;
  turnos* player;
  int i, k, j, cont = 1, barcos;

  while( fscanf(archivo, "%113[^\n]s", linea) != EOF)
  {
    fgetc(archivo);
    i = 0;
    //Se lee los tableros de los ataques y las posiciones de los barcos...
    if(cont <= 4)
    {
      data = malloc(sizeof(terreno));
      
      //Se pobla la matriz...
      for(k = 0; k < 10; k++)
        for(j = 0; j < 10; j++, i++)
          data->tablero[k][j] = linea[i];

      i++;

      //Se escanea el nombre...
      for(k = 0; linea[i] != '\0';i++,k++)
        name[k] = linea[i];
      name[k] = '\0';

      data->nombre = strdup(name);
      insertMap(tableros, data->nombre, data);
      cont++;
    }
    else
    {
      player = malloc(sizeof(turnos));

      //Se lee el numero de aciertos en barcos de tamano 3(A), 4(B) y 5(C).
      for(barcos = 1; barcos < 4;barcos++)
      {
        if(barcos == 1)
        {
          num[0] = linea[i];
          num[1] = '\0';
          player->a = conversion(num);
          i+=2;
        }

        if(barcos == 2)
        {
          num[0] = linea[i];
          num[1] = '\0';
          player->b = conversion(num);
          i+=2;
        }

        if(barcos == 3)
        {
          num[0] = linea[i];
          num[1] = '\0';
          player->c = conversion(num);
          i+=2;
        }
      }
      
      //Se lee el numero de fallos...
      for(k = 0; linea[i] != ','; i++, k++)
        num[k] = linea[i];
      num[k] = '\0';
      player->fallos = conversion(num);

      i++;
      
      //El nombre de quien pertenece el registro...
      for(k = 0; linea[i] != '\0'; i++, k++)
        name[k] = linea[i];
      name[k] = '\0';
      player->nombre = strdup(name);
      
      insertMap(registro, player->nombre, player);
    }
  }
  fclose(archivo);
}

//Funcion para Jugar...
void partida(Map* tableros, Map* scores,Map* registro)
{
  int x, y, i;
  char opcion[2];

  
  terreno* user = searchMap(tableros, "user");
  terreno* enemy = searchMap(tableros, "enemy");
  terreno* userATTACK = searchMap(tableros, "userATTACK");
  terreno* enemyATTACK = searchMap(tableros, "enemyATTACK");
  turnos* player = searchMap(registro, "user"); 
  turnos* pc = searchMap(registro, "enemy");

  printf("Este es tu tablero de ataque...\n");
  attack_tablero(userATTACK);
  printf("\n\nEste es el tablero de tus barcos...\n");
  attack_tablero(user);

  //Se pregunta si algunos de los 2, el usuario y el pc, se hundieron todos los barcos...
  while(finish(user) && finish(enemy))
  {
    menu3();
    //Se pregunta si el usuario va atacar o salir c/s guardar la partida...
    scanf("%1s", opcion);
    
    while(isdigit(opcion[0]) == 0)
    {
      printf("\nOpción no válida, elija otra7:\n");
      scanf("%1s", opcion);
    }

    while(opcion[0] < '1' || opcion[0] > '3')
    {
      printf("Opcion no valida.\n\nOPCION = ");
      scanf("%1s", opcion);

      while(isdigit(opcion[0]) == 0)
      {
        printf("\nOpción no válida, elija otra7:\n");
        scanf("%1s", opcion);
      }
    }

    //Esta opcion guarda la partida, guardando los tableros y los registros...
    if(opcion[0] == '2')
    {
      guardarPARTIDA(tableros, registro);
      break;
    }

    //Esta opcion se sale de la partida sin guardar...
    if(opcion[0] == '3')
    {
      system("clear");
      break;
    }

    //Pide ingresar las coordenadas X e Y, por separado...
    printf("Comandante, ingrese coordenada X a la cual atacar\nX = ");
    scanf("%d", &y);
    printf("Comandante, ingrese coordenada Y a la cual atacar\nY = ");
    scanf("%d", &x);

    //Se verifica si la casilla del tablero fue atacada...
    while(userATTACK->tablero[x][y] != ' ')
    {
      
      printf("Comandante, la coordenada que ingreso ya fue atacada o está fuera del rango.\n\n");
      printf("Comandante, ingrese coordenada X a la cual atacar\nX = ");
      scanf("%d", &y);
      printf("Comandante, ingrese coordenada Y a la cual atacar\nY = ");
      scanf("%d", &x);
    }

    system("clear");

    //Se marca el ataque en el tablero de ataque del usuario y se refleja en el tablero de barcos de la computadora...
    attack(userATTACK, enemy, player, x, y);
    printf("\nEste es tu tablero de ataque...\n\n");
    attack_tablero(userATTACK);
    
    //Ataca el pc, cuyas corrdenadas X e Y son aleatorias...
      x = rand() % 10;
      y = rand() % 10;

    //Se verifica si esa casilla fue atacada...
    while(enemyATTACK->tablero[x][y] != ' ')
    {
      x = rand() % 10;
      y = rand() % 10;
    }

    //Se marca el ataque para el caso de la computadora...
    attack(enemyATTACK, user, pc, x, y);
    printf("\n");
    attack_tablero(user);
  }
  system("clear");

  //Pregunta si el usuario gano la partida...
  if(finish(enemy) == false)
  {
    printf("VICTORIA COMANDANTE!, HA GANADO LA PARTIDA.\nA CONTINUACION LOS RESULTADOS Y PUNTAJES...\n\n");
    nuevoRECORD(registro, scores);
    FILE* progress = fopen("progreso.txt", "w");
    fseek(progress, 0, SEEK_SET);
    fclose(progress);
  }

  //Pregunta si el pc gano la partida...
  if(finish(user) == false)
  {
    printf("GAME OVER COMANDANTE,\nFUE VENCIDO POR EL ENEMIGO, MEJOR SUERTE PARA LA PROXIMA...\n\n");
    FILE* progress = fopen("progreso.txt", "w");
    fseek(progress, 0, SEEK_SET);
    fclose(progress);
  }
  //Se el caso que termine la partida o que se suspende, se vacia el contenido de los mapas de tablero y registro de ataque...
  for(i = 0; i < 6; i++)
  {
    if(i == 0) eraseMap(tableros, "user");
    if(i == 1) eraseMap(tableros, "enemy");
    if(i == 2) eraseMap(tableros, "userATTACK");
    if(i == 3) eraseMap(tableros, "enemyATTACK");
    if(i == 4) eraseMap(registro, "user");
    if(i == 5) eraseMap(registro, "enemy");
  }
}

//Funcion para los puntos solo cuando gana el usuario...
void nuevoRECORD(Map* registro, Map* scores)
{
  puntajes* nuevo = malloc(sizeof(puntajes));
  turnos* dataENEMY = searchMap(registro, "enemy");
  turnos* dataUSER = searchMap(registro, "user");
  
  char nombre[21];
  int puntos = 1000, vidas = 15, cada4;

  printf("Puntos base por ganar = %i\n", puntos);
  vidas-= (dataENEMY->a + dataENEMY->b + dataENEMY->c);

  if(vidas == 15)
  {
    printf("Bonus por no perder vidas = 2000\n");
    puntos += 2000;
  }
  else
  {
    puntos += (300 * (16 - vidas));
    printf("Bonus por haber sobrevivido con %i = %i\n",vidas, (puntos-1000));
  }

  if(dataUSER->fallos == 0)
  {
    printf("Bonus por no fallar un ataque = 3000\n");
    puntos += 3000;
  }
  
  if(dataUSER->fallos >= 4)
  {
    cada4 = dataUSER->fallos / 4;
    printf("Descuento por fallar cada 4 veces = %i\n\n", cada4*25);
    puntos -= (25 * cada4);
  }

  printf("PUNTAJE FINAL = %i", puntos);
  printf("\n\n¿Cual es tu nombre, Comandante? (Maximo 20 letras/caracteres)\n\n NOMBRE = ");

  getchar();
  scanf("%20[^\n]s", nombre);

  nuevo->puntuacion = puntos;
  nuevo->capitan = strdup(nombre);
  insertMap(scores, &nuevo->puntuacion, nuevo);
}

//Funcion cuando el usuario quiere salir y guardar la partida...
void guardarPARTIDA(Map* tableros, Map* registro)
{
  FILE* archivo = fopen("progreso.txt", "w");
  if(archivo == NULL)
  {
    printf("Error al abrir el archivo. Se forzara el cierre del juego...");
    exit(EXIT_FAILURE);
  }
  //Se borra el progreso anterior para guardar en el instante...
  fseek(archivo, 0, SEEK_SET);
  terreno* data = firstMap(tableros);
  turnos* player = firstMap(registro);
  int i, j, cont = 1;

  //Se guarda primero los tableros/matrices junto con los nombres.
  while(cont <= 4)
  {
    for(i = 0; i < 10; i++)
      for(j = 0; j < 10; j++)
        fputc(data->tablero[i][j], archivo);

    fputc(' ', archivo);
    fputs(data->nombre, archivo);
    fputc('\n', archivo);

    cont++;
    data = nextMap(tableros);
  }
  system("clear");
  
  //Desues se guarda los 2 registros de ataques certeros y fallos junto con el nombre.
  for(i = 0; i < 2; i++)
  {
    fprintf(archivo, "%i", player->a);
    fputc(',', archivo);

    fprintf(archivo, "%i", player->b);
    fputc(',', archivo);

    fprintf(archivo, "%i", player->c);
    fputc(',', archivo);

    fprintf(archivo, "%i", player->fallos);
    fputc(',', archivo);

    fputs(player->nombre, archivo);
    player = nextMap(registro);
    fputc('\n', archivo);
  }

  fclose(archivo);
  
  //Se vacia los mapas...
  for(i = 0; i < 6; i++)
  {
    if(i == 0) eraseMap(tableros, "user");
    if(i == 1) eraseMap(tableros, "enemy");
    if(i == 2) eraseMap(tableros, "userATTACK");
    if(i == 3) eraseMap(tableros, "enemyATTACK");
    if(i == 4) eraseMap(registro, "user");
    if(i == 5) eraseMap(registro, "enemy");
  }
}

//Funcion para marcar/registar los ataques...
void attack(terreno* attack, terreno* oponente, turnos* grafo, int x, int y)
{
  printf("\n");
  //Condicion para diferenciar quien hizo el ataque...
  if(strcmp(attack->nombre, "enemyATTACK") == 0)
    printf("(EL OPONENTE) ");

  //Sea acierto o fallo se registrara en el tablero de ataque(quien ataca) y en el tablero de barcos(quien fue impactado)

  //Si la casilla es un espacio, se entiende como un fallo
  if(oponente->tablero[x][y] == ' '){
    grafo->fallos++;
    printf("HAS FALLADO!:(\n");
    attack->tablero[x][y] = 'O';
    oponente->tablero[x][y] = '!';
  }
  
  //Si acerto a uno de los 3 barcos, se entiende como un acierto (no se dara informacion cual barco es...). Lo que va a variar es que si llega a hundir un barco que se informara el barco...
  if(oponente->tablero[x][y] == 'A')
  {
    grafo->a++;
    attack->tablero[x][y] = 'X';
    oponente->tablero[x][y] = 'X';
    printf("LE HAS DADO A UN BARCO!\n");
    
    if(grafo->a == 3)
      printf("\nEL BARCO A HA SIDO HUNDIDO!\n");
  }
  
  if(oponente->tablero[x][y] == 'B')
  {
    grafo->b++;
    attack->tablero[x][y] = 'X';
    oponente->tablero[x][y] = 'X';
    printf("LE HAS DADO A UN BARCO!\n");

    if(grafo->b == 4)
      printf("\nEL BARCO B HA SIDO HUNDIDO!\n");
  }
  
  if(oponente->tablero[x][y] == 'C')
  {
    grafo->c++;
    attack->tablero[x][y] = 'X';
    oponente->tablero[x][y] = 'X';
    printf("LE HAS DADO A UN BARCO!\n");

    if(grafo->c == 5)
      printf("\nEL BARCO C HA SIDO HUNDIDO!\n");
  }
}

//Funcion para verificar si los barcos de uno de los 2 fueron hundidos...
bool finish(terreno* matriz)
{
  int cont = 0, i, j;

  //Se recorre la matriz para contar las X, en total son 12...
  for(i = 0; i < 10; i++)
  {
    for(j = 0; j < 10; j++)
      if(matriz->tablero[i][j] == 'X')
        cont++;
  }

  if(cont == 12) return false;
  return true;
}

//Se imprime el tablero despues de los ataques...
void attack_tablero(terreno* matriz)
{
  int i, j;

  printf("   0  1  2  3  4  5  6  7  8  9\n");
  for(i = 0; i < 10; i++)
  {
    printf("%i ", i);

    for(j = 0; j < 10; j++)
      printf("[%c]", matriz->tablero[i][j]);

    printf("\n");
  }
}

//Los Menus para el principal, los puntajes, jugar partida y durante la partida...
void menu()
{
  printf(" ______________________________________\n|         Bienvenido al Menu de        |\n|              BATTLESHIP              |\n|______________________________________|\n| 1) Jugar Partida                     |\n| 2) Mostrar Los Puntajes              |\n| 3) Salir De La Aplicacion            |\n|______________________________________|\n\n¿Que deseas hacer hoy?\n\nOPCION = ");
}

void menu1()
{
  printf(" ______________________________________\n| 1) Nueva Partida                     |\n| 2) Cargar Partida                    |\n| 3) Volver Al Menu Principal          |\n|______________________________________|\n\n¿Que desea hacer?\n\nOPCION = ");
}

void menu2()
{
  printf("\n ______________________________________\n| 1) Cargar Puntajes Guardados         |\n| 2) Guardar Puntajes Nuevos           |\n| 3) Borrar Puntajes Guardados         |\n| 4) Volver Al Menu Principal          |\n|______________________________________|\n\n¿Que deseas hacer?\n\nOPCION = ");
}

void menu3()
{
  printf(" ______________________________________\n| 1) Atacar Al Oponente                |\n| 2) Guardar Y Salir De La Partida     |\n| 3) Salir De La Partida Sin Guardar   |\n|______________________________________|\n\n¿Que desea hacer, Capitan?\n\nOPCION = ");
}
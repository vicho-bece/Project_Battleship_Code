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
  Map* tableros = createMap(is_equal_string);
  Map* scores = createMap(is_equal_int);
  Map* registro = createMap(is_equal_string);
  setSortFunction(scores, more_than_int);
  
  char num[2];
  menu();

  
  scanf("%1s", num);  
  
  
  while(isdigit(num[0]) == 0)
  {
    printf("\nOpción no válida, elija otra1:\n");
    
    scanf("%1s", num);
  }

  while(num[0] != '3')
  {
    switch(num[0])
    {
      case '1': jugarPARTIDA(tableros, scores, registro); break;
      case '2': mostrarPUNTAJES(scores); break;
      default: printf("\nLa opcion escogida no es valida. Favor de ingresar una dentro del rango\n\nOPCION = "); break;
    }
    system("clear");
    menu();
    
    
    scanf("%1s", num);
    
    while(isdigit(num[0]) == 0)
    {
      printf("\nOpción no válida, elija otra2:\n");
      
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
    printf("\nOpción no válida, elija otra3:\n");
    
    scanf("%1s", num);
  }
  
  while(num[0] != '3')
  {
    FILE* archivo = fopen("progreso.txt", "r");
    if(archivo == NULL)
    {
      printf("Error al abrir el archivo, se forzara el cierre del juego...");
      exit(EXIT_FAILURE);
    }
    fseek(archivo, 0, SEEK_END);
    
    switch(num[0])
    {
      case '1':
        if(ftell(archivo) != 0)
        {
          printf("Al parecer dejaste una partida sin finalizar.\n¿Desea borrar el progreso igualmente?\n\n1 = SI (Lo llevara de inmediato a la nueva partida)\n2 = NO (Seguira en este menu)\n\nOPCION = ");

          getchar();
          scanf("%1[^\n]s", num);
          
          while(num[0] != '1' && num[0] != '2')
          {
            printf("\nOpcion no valido.\n\nOPCION = ");
            scanf("%1[^\n]s", num);
            
          }

          if(num[0] == '1')
          {
            fclose(archivo);
            archivo = fopen("progreso.txt", "w");
            fseek(archivo, 0, SEEK_SET);

            fclose(archivo);

            nuevaPARTIDA(tableros, scores, registro);
            break;
          }
          else
          {
            fclose(archivo);
            system("clear");
            break;
          }
        }
        else
        {
          fclose(archivo);
          nuevaPARTIDA(tableros, scores, registro); 
          break;
        }

      case '2':
        if(ftell(archivo) != 0)
        {
          fclose(archivo);
          cargarPARTIDA(tableros, registro);
          partida(tableros, scores, registro); 
          break;
        }
        else
        {
          printf("\nNo hay una partida guardada anteriormente...\n\n");
          fclose(archivo);
          break;
        }

      default:
        printf("\nOpcion no valido...\n\n");
        break;
    }
    menu1();
    
    scanf("%1s", num);
    
    while(isdigit(num[0]) == 0)
    {
      printf("\nOpción no válida, elija otra4:\n");
      
      scanf("%1s", num);
      
    }
  }
}

//OPCIONES DE MOSTRAR LOS PUNTAJES...
void mostrarPUNTAJES(Map* score)
{
  puntajes* datos = firstMap(score);
  system("clear");
  
  if(datos != NULL)
  {
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

void show(Map* score)
{
  puntajes* datos = firstMap(score);
  while(datos != NULL)
  {
    printf("%i %s\n", datos->puntuacion, datos->capitan);
    datos = nextMap(score);
  }
}

void cargarPUNTAJES(Map* score)
{
  FILE* points = fopen("score.txt", "r");

  if(points == NULL)
  {
    printf("Error al abrir el archivo, se forzara el cierre del juego...");
    exit(EXIT_FAILURE);
  }
  system("clear");
  fseek(points, 0, SEEK_END);
  if(ftell(points) != 0)
  {
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

      for(; linea[i] != ' ' && i < 19 ; i++)
        capitan[i] = linea[i];
      capitan[i] = '\0';

      loading->capitan = strdup(capitan);
      i++;

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
  FILE* points = fopen("score.txt", "a");
  if(points == NULL)
  {
    printf("Error al abrir el archivo. Se forzara el cierre del juego...");
    exit(EXIT_FAILURE);
  }
  system("clear");
  puntajes* datos = firstMap(score);
  if(datos != NULL)
  {
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
  FILE* points = fopen("score.txt", "r");
  if(points == NULL)
  {
    printf("Error al abrir el archivo, se forzara el cierre del juego...");
    exit(EXIT_FAILURE);
  }
  system("clear");
  fseek(points, 0, SEEK_END);

  if(ftell(points) != 0)
  {
    fclose(points);
    points = fopen("score.txt", "w");
    fseek(points, 0, SEEK_SET);
    
    printf("\nSe borro el contenido de los puntajes en el archivo de forma exitosa...\n\n");
  }
  else
  {
    printf("\nEl archivo donde se guardan puntajes se encuentra vacio...\n\n");
  }

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
    
    crearMATRIZ(matriz);
    insertMap(tableros, matriz->nombre, matriz); 
  }
  
  srand(time(NULL));
  barcos(tableros);
  barcosMAQUINA(tableros);

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
  
  partida(tableros, scores, registro);
}

void crearMATRIZ(terreno* matriz)
{
  int i, j;

  for(i = 0; i <= 9; i++)
    for(j = 0; j <= 9; j++)
      matriz->tablero[i][j] = ' ';
}


void barcos(Map* tableros)
{
  terreno *p = searchMap(tableros, "user");
  int cdx, cdy, i, j, k = 3, cdx2, cdy2;
  
  printf("Este es tu tablero Usuario(a)...\n\n");
  printf("   0  1  2  3  4  5  6  7  8  9\n");

  for(i = 0; i < 10; i++)
  {
    printf("%i ", i);

    for(j = 0; j < 10; j++)
      printf("[%c]", p->tablero[i][j]);

    printf("\n");
  }

  while(k <= 5)
  {  
      printf("\n\nAgregue coordenada de inicio para barco de %i casillas.\nX = ", k);
      scanf("%d", &cdy);
      printf("Y = ");
      scanf("%d", &cdx);

      printf("\n\nAgregue coordenadas finales para barco de %i casillas.\nX = ", k);
      scanf("%d", &cdy2);
      printf("Y = ");
      scanf("%d", &cdx2);
      
      while((barcosLibres(cdx,cdy,p,k,cdx2,cdy2) == false) || medida(cdx, cdy, cdx2, cdy2, k))
      {
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

      agregarBARCOS(cdx,cdy,p,k,cdx2,cdy2);
    
    k++;
  }
  system("clear");
}

bool medida(int x, int y, int x2, int y2, int k)
{
  int ejeX, ejeY;

  ejeX = x - x2;
  ejeY = y - y2;

  if(ejeY < 0) ejeY *= -1;
  if(ejeX < 0) ejeX *= -1;

  if(ejeX == 0 && ejeY + 1 == k) return false;
  if(ejeX + 1 == k && ejeY == 0) return false;

  return true;
}

bool barcosLibres(int x, int y, terreno* p, int k, int x2, int y2)
{
  int i, c;

  if(x > x2) for(i = 0; i < k; i++) if(p->tablero[x-i][y] != ' ') return false;

  if(x < x2) for(i = 0; i < k; i++) if(p->tablero[x+i][y] != ' ') return false;
  
  if(y > y2) for(i = 0; i < k; i++) if(p->tablero[x][y-i] != ' ') return false;
  
  if(y < y2) for(i = 0; i < k; i++) if(p->tablero[x][y+i] != ' ') return false;
  
  return true;
}

void agregarBARCOS(int x, int y, terreno* p, int k, int x2, int y2)
{
  int i, j, c;
  
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


void barcosMAQUINA(Map* tableros){
  terreno* p = searchMap(tableros, "enemy");
  int x, y, i, j, aux;

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

  x = rand() % 10;
  y = rand() % 6;

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

  x = rand() % 7;
  y = rand() % 10;
  
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

void cargarPARTIDA(Map* tableros, Map* registro)
{
  system("clear");
  FILE* archivo = fopen("progreso.txt", "r");
  char name[12];
  char linea[114];
  char num[3];
  terreno* data;
  turnos* player;
  int i, k, j, cont = 1, barcos;

  while( fscanf(archivo, "%113[^\n]s", linea) != EOF)
  {
    fgetc(archivo);
    i = 0;

    if(cont <= 4)
    {
      data = malloc(sizeof(terreno));
      
      for(k = 0; k < 10; k++)
        for(j = 0; j < 10; j++, i++)
          data->tablero[k][j] = linea[i];

      i++;

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

      for(k = 0; linea[i] != ','; i++, k++)
        num[k] = linea[i];
      num[k] = '\0';
      player->fallos = conversion(num);

      i++;
      
      for(k = 0; linea[i] != '\0'; i++, k++)
        name[k] = linea[i];
      name[k] = '\0';
      player->nombre = strdup(name);
      
      insertMap(registro, player->nombre, player);
    }
  }
  fclose(archivo);
}

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

  while(finish(user) && finish(enemy))
  {
    menu3();
    
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
    }

    if(opcion[0] == '2')
    {
      guardarPARTIDA(tableros, registro);
      break;
    }
    if(opcion[0] == '3')
    {
      system("clear");
      break;
    }

    printf("Comandante, ingrese coordenada X a la cual atacar\nX = ");
    scanf("%d", &y);
    printf("Comandante, ingrese coordenada Y a la cual atacar\nY = ");
    scanf("%d", &x);

    while(userATTACK->tablero[x][y] != ' ')
    {
      
      printf("Comandante, la coordenada que ingreso ya fue atacada o está fuera del rango.\n\n");
      printf("Comandante, ingrese coordenada X a la cual atacar\nX = ");
      scanf("%d", &y);
      printf("Comandante, ingrese coordenada Y a la cual atacar\nY = ");
      scanf("%d", &x);
    }

    system("clear");
    attack(userATTACK, enemy, player, x, y);
    printf("\nEste es tu tablero de ataque...\n\n");
    attack_tablero(userATTACK);
    
    
      x = rand() % 10;
      y = rand() % 10;

    while(enemyATTACK->tablero[x][y] != ' ')
    {
      x = rand() % 10;
      y = rand() % 10;
    }

    attack(enemyATTACK, user, pc, x, y);
    printf("\n");
    attack_tablero(user);
  }
  system("clear");
  if(finish(enemy) == false)
  {
    printf("VICTORIA COMANDANTE!, HA GANADO LA PARTIDA.\nA CONTINUACION LOS RESULTADOS Y PUNTAJES...\n\n");
    nuevoRECORD(registro, scores);
    FILE* progress = fopen("progreso.txt", "w");
    fseek(progress, 0, SEEK_SET);
    fclose(progress);
  }
  if(finish(user) == false)
  {
    printf("GAME OVER COMANDANTE,\nFUE VENCIDO POR EL ENEMIGO, MEJOR SUERTE PARA LA PROXIMA...\n\n");
    FILE* progress = fopen("progreso.txt", "w");
    fseek(progress, 0, SEEK_SET);
    fclose(progress);
  }
    
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

void nuevoRECORD(Map* registro, Map* scores)
{
  puntajes* nuevo = malloc(sizeof(puntajes));
  turnos* dataENEMY = searchMap(registro, "enemy");
  turnos* dataUSER = searchMap(registro, "user");
  
  char nombre[21];
  int puntos = 1000, vidas = 15, cada5;

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
  
  if(dataUSER->fallos >= 5)
  {
    cada5 = dataUSER->fallos / 5;
    printf("Descuento por fallar cada 5 veces = %i\n\n", cada5*25);
    puntos -= (25 * cada5);
  }

  printf("PUNTAJE FINAL = %i", puntos);
  printf("\n\n¿Cual es tu nombre, Comandante? (Maximo 20 letras/caracteres)\n\n NOMBRE = ");

  getchar();
  scanf("%20[^\n]s", nombre);

  nuevo->puntuacion = puntos;
  nuevo->capitan = strdup(nombre);
  insertMap(scores, &nuevo->puntuacion, nuevo);
}

void guardarPARTIDA(Map* tableros, Map* registro)
{
  FILE* archivo = fopen("progreso.txt", "w");
  if(archivo == NULL)
  {
    printf("Error al abrir el archivo. Se forzara el cierre del juego...");
    exit(EXIT_FAILURE);
  }
  fseek(archivo, 0, SEEK_SET);
  terreno* data = firstMap(tableros);
  turnos* player = firstMap(registro);
  int i, j, cont = 1;

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

void attack(terreno* attack, terreno* oponente, turnos* grafo, int x, int y)
{
  printf("\n");

  if(strcmp(attack->nombre, "enemyATTACK") == 0)
    printf("(EL OPONENTE) ");

  if(oponente->tablero[x][y] == ' '){
    grafo->fallos++;
    printf("HAS FALLADO!:(\n");
    attack->tablero[x][y] = 'O';
    oponente->tablero[x][y] = '!';
  }
  
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

bool finish(terreno* matriz)
{
  int cont = 0, i, j;

  for(i = 0; i < 10; i++)
  {
    for(j = 0; j < 10; j++)
      if(matriz->tablero[i][j] == 'X')
        cont++;
  }

  if(cont == 12) return false;
  return true;
}

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
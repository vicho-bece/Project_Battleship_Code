#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "Map.h"
#include "list.h"

typedef struct{
  char tablero [10][10];
  char* nombre;
}terreno;

typedef struct{
  int puntuacion;
  char comandante;
}puntajes;

//Estructura para el mapa del grafo
typedef struct{
  int a;
  int b;
  int c;
  int fallos;
  char* nombre;
}turnos;

//Estructura para la lista del grafo
typedef struct{
  int *x;
  int *y;
  char* nombre;
}coordenadas;

//FUNCIONES APARTES


int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

int is_equal_int(void * key1, void * key2) {
    if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

int lower_than_int(void * key1, void * key2) {
    if(*(int*)key1 < *(int*)key2) return 1;
    return 0;
}

void agregarBARCOS(int, int, terreno*, int, int, int);
void crearMATRIZ(terreno*);
void barcos(Map*);
void barcosMAQUINA(Map*);
bool vertical(int, int, terreno*);
void partida(Map*, Map*, List*);
bool finish(terreno*);
void attack(terreno*, terreno*, turnos*, coordenadas*, int, int);
void attack_tablero(terreno* );

int main()
{
  
  Map* tableros = createMap(is_equal_string);
  Map* scores = createMap(is_equal_int);
  Map* grafo = createMap(is_equal_string);//GRAFO IMPLICITO
  List* ataques = create_list();//LISTA GRAFO
  

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

  /*printf("   0  1  2  3  4  5  6  7  8  9\n");

  terreno* m = searchMap(tableros, "enemy");
  for( i=0 ; i<=9 ; i++)
  {
    printf("%d ", i);

    for(j=0 ; j<=9 ; j++)
      printf("[%c]", m->tablero[i][j]);
      
    printf("\n");
  }*/

  for(i = 0; i < 2; i++)
  {
    turnos* map = malloc(sizeof(turnos));
    coordenadas* xy = malloc(sizeof(coordenadas));
    map->fallos = 0;
    map->a = 0;
    map->b = 0;
    map->c = 0;

    if(i == 0)
    {
      map->nombre = strdup("user");
      xy->nombre = strdup("user");
    } 
    if(i == 1)
    {
      map->nombre = strdup("enemy");
      xy->nombre = strdup("enemy");
    } 

    xy->x = malloc(0 * sizeof(int));
    xy->y = malloc(0 * sizeof(int));

    insertMap(grafo, map->nombre, map);
    push_back(ataques, xy);
  }
  

  partida(tableros, grafo, ataques);
  
  return 0;
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
  int cdx, cdy, i, j, k=3,cdx2, cdy2;
  
  printf("Este es tu tablero Usuario(a)...\n\n");
  printf("   0  1  2  3  4  5  6  7  8  9\n");
  for(i = 0; i < 10; i++)
  {
    printf("%i ", i);

    for(j = 0; j < 10; j++)
      printf("[%c]", p->tablero[i][j]);

    printf("\n");
  }
  while(k<=5){
    
      printf("\n\nAgregue coordenada de inicio para barco de %i casillas\nX = ", k);
      scanf("%d", &cdy);
      printf("Y = ");
      scanf("%d", &cdx);
      printf("\n\nAgregue coordenadas finales para barco de %i casillas\nX = ", k);
      scanf("%d", &cdy2);
      printf("Y = ");
      scanf("%d", &cdx2);
      
      while(p->tablero[cdx][cdy] != ' ')
      {
        printf("Comandante,La coordenada (%i,%i) se encuentra ocupada.Favor de ingresar otro\nX = ", cdy, cdx);
        scanf("%d", &cdy);
        printf("Y = ");
        scanf("%d", &cdx);
      
      }
      while(p->tablero[cdx2][cdy2] != ' ')
      {
        printf("Comandante,La coordenada (%i,%i) se encuentra ocupada.Favor de ingresar otro\nX = ", cdy2, cdx2);
        scanf("%d", &cdy2);
        printf("Y = ");
        scanf("%d", &cdx2);
      
      }
      system("clear");
      agregarBARCOS(cdx,cdy,p,k,cdx2,cdy2);
    
    k++;
  }
}

void agregarBARCOS(int x, int y, terreno* p, int k, int x2, int y2)
{
  int i, j, auxX, auxY;
  auxX = x2;
  auxY = y2;
  for(i = 0 ; i <= 9 ; i++)
  {
    for(j = 0 ; j <= 9 ; j++)
    {
      if(i == x && j == y)
      {
        switch(k)
        {
          case 3:
          if(x > x2){
            for(int c = 0; c < 3; c++) p->tablero [i-c][j] = 'A'; break;
          }
          if(x < x2){
            for(int c = 0; c < 3; c++) p->tablero [i+c][j] = 'A'; break;
          }
          if(y > y2){
            for(int c = 0; c < 3; c++) p->tablero [i][j-c] = 'A'; break;
          }
          if(y <y2){
            for(int c = 0; c < 3; c++) p->tablero [i][j+c] = 'A'; break;
          }

          case 4: 
          if(x > x2){
            for(int c = 0; c < 4; c++) p->tablero [i-c][j] = 'B'; break;
          }
          if(x < x2){
            for(int c = 0; c < 4; c++) p->tablero [i+c][j] = 'B'; break;
          }
          if(y > y2){
            for(int c = 0; c < 4; c++) p->tablero [i][j-c] = 'B'; break;
          }
          if(y <y2){
            for(int c = 0; c < 4; c++) p->tablero [i][j+c] = 'B'; break;
          }
          
          case 5:
          if(x > x2){
            for(int c = 0; c < 5; c++) p->tablero [i-c][j] = 'C'; break;
          }
          if(x < x2){
            for(int c = 0; c < 5; c++) p->tablero [i+c][j] = 'C'; break;
          }
          if(y > y2){
            for(int c = 0; c < 5; c++) p->tablero [i][j-c] = 'C'; break;
          }
          if(y <y2){
            for(int c = 0; c < 5; c++) p->tablero [i][j+c] = 'C'; break;
          }
        }
        
      }
    }
  }
  printf("\n");
  printf("   0  1  2  3  4  5  6  7  8  9\n");
  for(int i = 0; i < 10; i++)
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

  //printf("   0  1  2  3  4  5  6  7  8  9\n");
  for( i=0 ; i<=9 ; i++)
  {  
    //printf(" %d", i);
    for( j=0 ; j<=9 ; j++)
    {
      
      if(i == x){
        
        if(j == y){
          for(int k = 0; k < 3; k++){
            
            p->tablero [i][j+k] = 'A';
          
          }
        
        }
      }
      
      //printf("[%c]", p->tablero[i][j]);
    }
    //printf("\n");

  }
  //printf("-------------------------------------------\n");

  x = rand() % 10;
  y = rand() % 6;

  while(x == aux)
    x = rand() % 10;

  //printf("   0  1  2  3  4  5  6  7  8  9\n");
  for( i=0 ; i<=9 ; i++)
  {
    //printf("%d ", i);
    for( j=0 ; j<=9 ; j++)
    {
      
      if(i == x){
        
        if(j == y){
          
          for(int k = 0; k < 5; k++){
            
            p->tablero [i][j+k] = 'C';
          
          }
            
        }
      }
      
      //printf("[%c]", p->tablero[i][j]);
    }
    //printf("\n");

  }
  //printf("---------------------------------------------\n");
  x = rand() % 7;
  y = rand() % 10;
  
  while(vertical(x,y,p) == false)
  {
    x = rand() % 7;
    y = rand() % 10;
  }

  //printf("   0  1  2  3  4  5  6  7  8  9\n");
  for( i=0 ; i<=9 ; i++)
  {  
    
    //printf("%d ", i);
    for( j=0 ; j<=9 ; j++)
    {
      
      if(i == x){
        
        if(j == y){
          
          for(int k = 0; k < 4; k++){
            
            p->tablero [i+k][j] = 'B';
          
          }
          
        }
      }
      
      //printf("[%c]", p->tablero[i][j]);
    }
    //printf("\n");

  }
  

  //printf("\n");
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


void partida(Map* tableros,Map* grafo, List* ataques){

  int x, y;

  terreno* user = searchMap(tableros, "user");
  terreno* enemy = searchMap(tableros, "enemy");
  terreno* userATTACK = searchMap(tableros, "userATTACK");
  terreno* enemyATTACK = searchMap(tableros, "enemyATTACK");
  turnos* player = searchMap(grafo, "user"); 
  turnos* pc = searchMap(grafo, "enemy");
  coordenadas* usuario = first(ataques);
  coordenadas* enemigo = first(ataques); enemigo = next(ataques);


  while(finish(user) && finish(enemy))
  { 
	  
    printf("ingrese coordenada X a la cual atacar\nX = ");
    scanf("%d", &y);
    printf("ingrese coordenada Y a la cual atacar\nY = ");
    scanf("%d", &x);

    while(userATTACK->tablero[x][y] != ' ')
    {
      
      printf("La coordenada que ingreso ya fue atacado o fuera del rango.\n\n");
      printf("Ingrese coordenada X a la cual atacar\nX = ");
      scanf("%d", &y);
      printf("Ingrese coordenada Y a la cual atacar\nY = ");
      scanf("%d", &x);
    }

    system("clear");
    attack(userATTACK, enemy, player, usuario, x, y);
    printf("\nEste es tu tablero de ataque...\n\n");
    attack_tablero(userATTACK);

    x = rand() % 10;
    y = rand() % 10;

    while(enemyATTACK->tablero[x][y] != ' ')
    {
      x = rand() % 10;
      y = rand() % 10;
    }

    attack(enemyATTACK, user, pc, enemigo, x, y);
    
    printf("\n");
    attack_tablero(user);
  }

  printf("\nFIN EL JUEGO\n");
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

void attack(terreno* attack, terreno* oponente, turnos* grafo, coordenadas* registro, int x, int y)
{
  //coordenadas* turno = malloc(sizeof(coordenadas));

  if(oponente->tablero[x][y] == ' '){
    grafo->fallos++;
    printf("\nHAS FALLADO!:(\n");
    attack->tablero[x][y] = 'O';
    oponente->tablero[x][y] = '!';
  }
  
  if(oponente->tablero[x][y] == 'A')
  {
    grafo->a++;
    attack->tablero[x][y] = 'X';
    oponente->tablero[x][y] = 'X';
    printf("\nLE HAS DADO A UN BARCO!\n");
    if(grafo->a == 3){
      printf("\nEL BARCO A HA SIDO HUNDIDO!\n");

    }
  }
  
  if(oponente->tablero[x][y] == 'B')
  {
    grafo->b++;
    attack->tablero[x][y] = 'X';
    oponente->tablero[x][y] = 'X';
    printf("\nLE HAS DADO A UN BARCO!\n");
    if(grafo->b == 4){
      printf("\nEL BARCO B HA SIDO HUNDIDO!\n");
    }
  }
  
  if(oponente->tablero[x][y] == 'C')
  {
    grafo->c++;
    attack->tablero[x][y] = 'X';
    oponente->tablero[x][y] = 'X';
    printf("\nLE HAS DADO A UN BARCO!\n");
    if(grafo->c == 5){
      printf("\nEL BARCO C HA SIDO HUNDIDO!\n");
    }
  }
}

void attack_tablero(terreno* matriz){
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
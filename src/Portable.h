/////////////////////////////////////////////////////////////////////
// Portable.h - Declaracions necessaries per suport multiplataforma
/////////////////////////////////////////////////////////////////////
// Descripcio del modul
//   Recull les declaracions i definicions que poden variar d'una
//   plataforma a una altra.
/////////////////////////////////////////////////////////////////////
// Change Log:
// 1996???? VoK - Creat

// Cal definir els seguents tipus per que tinguin el numero
// de bits indicats a la plataforma on es vol compilar.

typedef signed long  int int32;
typedef signed short int int16;
typedef signed char      int8;

typedef unsigned long  int uint32;
typedef unsigned short int uint16;
typedef unsigned char      uint8;


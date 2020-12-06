/**
 * @file serial_sw.c
 * @brief Librería que implementa comunicación serial por software para microcontrolladores de 8 bits en su modalidad 8N1
 * @author Ing. José Roberto Parra Trewartha
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../utils/utils.h"
#include "serial_sw.h"

/**
  * @brief Función para configurar los pines RX y TX como E/S, y dichos pines se definien en el archivo serial_sw.h.
  * La velocidad de comunicación se define en el archivo serial_sw.h mediante la macro SW_BAUDRATE
  * @param (void)
  * @return (void)
*/
void serial_sw_init() {
  //Configuración de pines RX y TX
  SW_TX_TRIS = 0;
  SW_TX = 1;
  SW_RX_TRIS = 1;
}

/**
  * @brief Función que transmite un byte vía USART por software 
  * @param dato: (uint8_t) Byte a transmitir 
  * @return (void) retval
*/
void serial_sw_writeByte(uint8_t dato) {
  uint8_t bit_count = 8;
  //START
  SW_TX = 0;
  _delay(CYCLES_DELAY_TX_BIT);

  while(bit_count--) {
      SW_TX = dato & 0x01;
      _delay(CYCLES_DELAY_TX_BIT);
      dato >>= 1;
  }
  //STOP
  SW_TX = 1;
  _delay(CYCLES_DELAY_TX_BIT);
}

/**
  * @brief Función para recepción de un byte vía USART por software.                       
  * Importante desactivar interrupciones durante su ejecución para evitar errores de sincronización.
  * @param (void)
  * @return (uint8_t) Dato de 8 bits que ha sido recibido mediante USART por software
*/
uint8_t serial_sw_readByte() {
  uint8_t bit_count;
  uint8_t dato=0;
  //Espera condición START
  while(SW_RX);
  _delay(CYCLES_DELAY_RX_BIT);
  _delay(CYCLES_DELAY_RX_HALFBIT);
  for(bit_count=0; bit_count!=8;bit_count++) {
      if(SW_RX)
          dato|=1<<bit_count;
      _delay(CYCLES_DELAY_RX_BIT);
  }
  return dato;
}

/**
  * @brief Función de escritura de una cadena de caracteres mediante USART por software.
  * @param cadena: (const char *) Apuntador a la cadena de caracteres que se desea transmitir.
  * @return (void)
*/
void serial_sw_puts(const char *cadena) {
    while(*cadena) { //Mientras el valor de la cadena no sea \0, escribir caracter en la posición del apuntador
        serial_sw_writeByte(*cadena); //Escribe bytes secuencialmente hasta terminar con la cadena
        cadena++;
    }
}

/**
  * @brief Función de escritura de una cadena de caracteres, con retorno de carro (CR) y nueva línea (NL) mediante USART por software.
  * @param cadena: (const char *) Apuntador a la cadena de caracteres que se desea transmitir.
  * @return (void)
*/
void serial_sw_writeLine(const char *cadena)
{
    serial_sw_puts(cadena);    //Imprime cadena normalmente
    serial_sw_writeByte('\r'); //Retorno de carro CR
    serial_sw_writeByte('\n'); //Nueva línea NL
}

/**
  * @brief Función para lectura de una cantidad de elementos, para ponerlos en un buffer (arreglo) .
  * @param buff: (const char *) Apuntador al arreglo en el cual se desean copiar los datos.
  * @param len: (uint16_t) Cantidad de elementos a copiar de un buffer a otro
  * @return (void)
*/
void serial_sw_gets(uint8_t *buffer, uint16_t len) {
    for(;len;len--) {
        *buffer = serial_sw_readByte();    //Obtención de caracter del buffer serial, se almacena en el arreglo de caracteres
        buffer++; //se incremento de apuntador al arreglo
    }
}

/**
  * @brief Función para escribir un dato entero de 2 bytes por medio de USART por software
  * @param dato: (uint16_t) Dato de 16 bits a trasmitir mediante USART por software
  * @return (void)
*/
void serial_sw_writeInt16(uint16_t dato) {
    for(uint8_t i=0;i!=sizeof(uint16_t);i++) {
        serial_sw_writeByte(*((uint8_t *)&dato+i));
    }
}

/**
  * @brief Función para escribir un dato entero de 3 bytes por medio de USART por software
  * @param dato: (uint24_t) Dato de 24 bits a trasmitir mediante USART por software
  * @return (void)
*/
void serial_sw_writeInt24(uint24_t dato) {
    for(uint8_t i=0;i!=sizeof(uint24_t);i++) {
        serial_sw_writeByte(*((uint8_t *)&dato+i));
    }
}

/**
  * @brief Función para escribir un dato entero de 4 bytes por medio de USART por software
  * @param dato: (uint32_t) Dato de 32 bits a trasmitir mediante USART por software
  * @return (void)
*/
void serial_sw_writeInt32(uint32_t dato) {
    for(uint8_t i=0;i!=sizeof(uint32_t);i++) {
        serial_sw_writeByte(*((uint8_t *)&dato+i));
    }
}

/**
  * @brief Función para escribir un dato flotante de 4 bytes (o 3) por medio de USART por software
  * @param dato: (float) Dato flotante de 32(24) bits a trasmitir mediante USART por software
  * @return (void)
*/
void serial_sw_writeFloat(float dato) {
    for(uint8_t i=0;i!=sizeof(float);i++) {
        serial_sw_writeByte(*((uint8_t *)&dato+i));
    }
}

/**
  * @brief Función para escribir un dato de cualquier tipo por medio de USART por software
  * @param datos: (void*) Dato de cualquier tipo a trasmitir mediante USART por software
  * @param len: (uint16_t) Cantidad de bytes a transmitir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial_sw_write(void* datos, uint16_t len) {
	uint8_t* _datos = (uint8_t *)datos;
	while(len--)
		serial_sw_writeByte(*_datos++);
}

/**
  * @brief Función para leer un dato entero de 2 bytes por medio de USART por software.
  * @param (void)  
  * @return (uint16_t) Dato de 16 bits a recibir mediante USART por software.
*/
uint16_t serial_sw_readInt16() {
    uint16_t dato_leido;
    uint8_t * p_lectura;                    //Apuntador a variable entera de 16 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 16 bits
    for(uint8_t i=0;i!=sizeof(uint16_t);i++) {
        *(p_lectura++) = serial_sw_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído 
}

/**
  * @brief Función para leer un dato entero de 3 bytes por medio de USART por software.
  * @param (void)  
  * @return (uint24_t) Dato de 24 bits a recibir mediante USART por software.
*/
uint24_t serial_sw_readInt24() {
    uint24_t dato_leido;
    uint8_t * p_lectura;                    //Apuntador a variable entera de 24 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 24 bits
    for(uint8_t i=0;i!=sizeof(uint24_t);i++) {
        *(p_lectura++) = serial_sw_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído 
}

/**
  * @brief Función para leer un dato entero de 4 bytes por medio de USART por software.
  * @param (void)  
  * @return (uint32_t) Dato de 32 bits a recibir mediante USART por software.
*/
uint32_t serial_sw_readInt32() {
    uint32_t dato_leido;
    uint8_t * p_lectura;                    //Apuntador a variable entera de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 32 bits
    for(uint8_t i=0;i!=sizeof(uint32_t);i++) {
        *(p_lectura++) = serial_sw_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 16 bits leído 
}

/**
  * @brief Función para leer un dato flotante de 4(3) bytes por medio de USART por software
  * @param (void)  
  * @return (float) Dato flotante de 32(24) bits a recibir mediante USART por software
*/
float serial_sw_readFloat() {
    float dato_leido;
    uint8_t * p_lectura;                    //Apuntador a variable flotante de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo flotante de 32(24) bits
    for(uint8_t i=0;i!=sizeof(float);i++) {
        *(p_lectura++) = serial_sw_readByte();      //Recepción de datos
    }
    return dato_leido;    //Retorno de valor de 32(24) bits leído 
}

/**
  * @brief Función para leer un dato de cualquier tipo por medio de USART por software
  * @param datos: (void*) Dato de cualquier tipo a recibir mediante USART por software
  * @param len: (uint16_t) Cantidad de bytes a recibir. Usado generalmente con la función sizeof() y un tipo de datos no estándar,
  * como una estructura de datos.
  * @return (void)
*/
void serial_sw_read(void* datos, uint16_t len) {
	uint8_t* _datos = (uint8_t*)datos;           //Apuntador a variable
	while(len--) {
		*(_datos++) = serial_sw_readByte();      //Recepción de datos
	}
}
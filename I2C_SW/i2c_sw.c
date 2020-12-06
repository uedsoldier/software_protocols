/**
 * @file i2c_sw.c
 * @brief Librería para emulación de protocolo i2c por software mediante manejo de pines (polling).
 * En este caso es posible utilizar como SDA y SCL cualquier pin de propósito general del microcontrolador, aunque es importante señalar que éste debe poder ser entrada o salida.
 * @author Ing. José Roberto Parra Trewartha
*/
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "i2c_sw.h"
#include "../pconfig.h"
#include "../../utils/utils.h"

#ifndef SLAVE_MODE_SW
/**
 * @brief Función para generar condición START (S) en el bus i2c por software
 * @param (void)
 * @return (void)
*/
void i2c_sw_start() {
    SW_SDA_1
    __delay_us(4);
    SW_SCL_1
    __delay_us(4);
    SW_SDA_0
    __delay_us(4);
    SW_SCL_0
    //Secuencia de START
}

/**
 * @brief Función para generar condición STOP (P) en el bus i2c por software
 * @param (void)
 * @return (void)
*/
void i2c_sw_stop() {
    SW_SDA_0
    __delay_us(4);
    SW_SCL_1
    __delay_us(4);
    SW_SDA_1
    __delay_us(4);
    //Secuencia de STOP
}

/**
 * @brief Función para generar condición RESTART (Sr) en el bus i2c por software
 * @param (void)
 * @return (void)
*/
void i2c_sw_restart() {
    SW_SDA_1
    __delay_us(4);
    SW_SCL_1
    __delay_us(4);
    SW_SDA_0
    __delay_us(4);
    //Secuencia RESTART
}

/**
 * @brief Función para escribir un byte en el bus i2c por software
 * @param dato (uint8_t): Dato de 8 bits a trasmitir mediante el bus i2c por software
 * @return (bool)  bit !ACK (acknowledge), 0 indica ACK, mientras 1 indica NACK
*/
bool i2c_sw_writeByte(uint8_t dato) {
    uint8_t i,ack_bit;
    for(i=0;i!=8;i++) {
        SW_SCL_0;       //Reloj en bajo
        __delay_us(4);
        if(dato&0x80) //Si el último bit de dato es 1
            SW_SDA_1   //Pon en alto la línea de datos
        else
            SW_SDA_0   //Si no, ponla en bajo
        SW_SCL_1      //Reloj en alto
        __delay_us(4);
        dato<<=1;     //Corre dato un lugar a la izquierda
    }
    SW_SCL_0
    ack_bit=I2C_SW_SDA_PIN; //Lee estado del bit ack de la línea de datos
    SW_SCL_1
    SW_SCL_0  //Pulso de reloj
    return !ack_bit;    //Devuelve valor de bit ack
}

/**
 * @brief Función para leer un byte del bus i2c por software
 * @param ack (bool): Bit ACK a enviar a los dispositivos esclavos, según su lógica interna
 * @return (uint8_t) Dato de 8 bits desde algún dispositivo esclavo
*/
uint8_t i2c_sw_readByte(bool ack) {
    uint8_t i,dato=0;
    for(i=0;i!=8;i++) {
        SW_SCL_0
        __delay_us(4);
        SW_SCL_1       //Pulso de reloj para que entre dato
        __delay_us(4);
        if(I2C_SW_SDA_PIN)
            dato|=0x01;
        if(i<7)
            dato<<=1;   
    }
    SW_SCL_0
    __delay_us(4);
    if(ack==0) //NACK
        SW_SDA_1
    else //ACK
        SW_SDA_0
    __delay_us(4);
    SW_SCL_1
    __delay_us(4);
    SW_SCL_0;
    __delay_us(4);
    SW_SDA_1;   //Cambia a entrada para que el esclavo pueda mandar más datos, si es necesario
    __delay_us(4);
    return dato;
}

/**
 * @brief Función de configuración de modo i2c por software, en la que los pines SDA y SCL son configurados como entradas, cuyo estado lógico
 * default es alto por los resistores pullup, simulando terminales de colector/drenaje abierto (open-collector u open-drain).
 * @param (void)
 * @return (void)
*/
void i2c_sw_init() {
    I2C_SW_SDA_TRIS = 1;
    I2C_SW_SCL_TRIS = 1;
}
#endif

/**
 * @brief Función para escribir un dato entero de 2 bytes en el bus i2c por software
 * @param dato (uint16_t): Dato de 16 bits a trasmitir mediante el bus i2c por software
 * @return (void)
*/
void i2c_sw_writeInt16(uint16_t dato) {
    uint8_t i;
    for(i=0;i!=sizeof(int16_t);i++)
    {
        i2c_sw_writeByte(*((uint8_t *)&dato+i));
    }
}


/**
 * @brief Función para escribir un dato entero de 4 bytes en el bus i2c por software
 * @param dato (uint32_t): Dato de 32 bits a trasmitir mediante el bus i2c por software
 * @return (void)
*/
void i2c_sw_writeInt32(uint32_t dato) {
    uint8_t i;
    for(i=0;i!=sizeof(int32_t);i++) {
        i2c_sw_writeByte(*((uint8_t *)&dato+i));
    }
}

/**
 * @brief Función para escribir un dato flotante de 4 bytes (o 3) en el bus i2c por software
 * @param dato (float): Dato flotante de 32(24) bits a trasmitir mediante el bus i2c por software
 * @return (void)
*/
void i2c_hw_writeFloat(float dato) {
    uint8_t i;
    for(i=0;i!=sizeof(float);i++) {
        i2c_sw_writeByte(*((uint8_t *)&dato+i));
    }
}

#ifdef SLAVE_MODE_SW
/*
	Aún no implementado
*/
void i2c_sw_slave()
{

}
void i2c_sw_slave_write()
{

}
#endif



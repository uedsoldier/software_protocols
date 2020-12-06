/**
 * @file i2c_sw.h
 * @brief Librería para emulación de protocolo i2c por software mediante manejo de pines (polling).
 * En este caso es posible utilizar como SDA y SCL cualquier pin de propósito general del microcontrolador, aunque es importante señalar que éste debe poder ser entrada o salida.
 * @author Ing. José Roberto Parra Trewartha
*/

#ifndef I2C_SW_H
#define	I2C_SW_H

/**
 * Utilización de modalidad i2c por software (aún no implementada)
*/
//#define SLAVE_MODE_SW

/**
 * Definición de pines SDA y SCL para comunicación i2c por software.
 * Configuración de pin físico y bit del registro tri-estado correspondiente.
*/

#ifndef I2C_SW_SDA_PIN
#define I2C_SW_SDA_PIN PORTBbits.RB0
#define I2C_SW_SDA_TRIS TRISBbits.TRISB0
#endif

#ifndef I2C_SW_SCL_PIN
#define I2C_SW_SCL_PIN PORTBbits.RB1
#define I2C_SW_SCL_TRIS TRISBbits.TRISB1
#endif

/*
	Definición de macros para las distintas configuraciones posibles en el modo i2c por software
*/
#define SW_SDA_1 I2C_SW_SDA_TRIS=1;
#define SW_SDA_0 I2C_SW_SDA_TRIS=0;I2C_SW_SDA_PIN=0;
#define SW_SCL_1 I2C_SW_SCL_TRIS=1;while(!I2C_SW_SCL_PIN){} //Prueba clock stretching
#define SW_SCL_0 I2C_SW_SCL_TRIS=0;I2C_SW_SCL_PIN=0;

/*
	Funciones prototipo
*/
void i2c_sw_start();
void i2c_sw_stop();
void i2c_sw_restart();
bool i2c_sw_writeByte(uint8_t dato);
uint8_t i2c_sw_readByte(bool ack);
void i2c_sw_init();
void i2c_sw_ack();
void i2c_sw_nack(); 
#ifdef SLAVE_MODE_SW
void i2c_sw_slave();
void i2c_sw_slave_write();
#endif


/**
 * Funciones para envío de datos de 2 o más bytes: enteros de 16 y 32 bits, y números flotantes de 
 * 24 bits (al parecer en un formato minifloat) o de 32 bits (Estándar IEEE 754 para precisión simple)
*/
#ifdef i2c_sw_INT16_TX
void i2c_sw_writeInt16(int16_t dato);
#endif
#ifdef i2c_sw_INT32_TX
void i2c_sw_writeInt32(int32_t dato);
#endif
#ifdef i2c_sw_FLOAT_TX
void i2c_sw_writeFloat(float dato);
#endif


#endif	/* I2C_SW_H */


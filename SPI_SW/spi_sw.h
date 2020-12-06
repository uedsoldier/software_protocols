/*
    Librería de comunicación spi por software para microcontroladores PIC de 8 bits
    Autor: Ing. José Roberto Parra Trewartha
    Compilador: XC8
 */

#ifndef SPI_SW_H
#define	SPI_SW_H

/*
	Número de bits por transmisión/recepción SPI, cuidado con utilizar un dato mayor 
	a 8 bits y utilizar las funciones para enteros de 16 y 32 bits, y flotantes
*/

#define SPI_SW_NUM_BITS_TX 16
#define SPI_SW_NUM_BITS_RX 16

/*
Modos de comunicacion SPI: (CPOL,CPHA) (Polaridad de reloj, Fase de reloj)
 * 
 * _______________________________________________________________________________
 * Modo |  Motorola  | Microchip |  Muestreo   |  Propagacion   | Estado inactivo |
 *      |  CPOL CPHA | CKP CKE   |  en flanco  |   en flanco    |   de reloj      |
 * _____|____________|___________|_____________|________________|_________________|
 *   0       0,0          0,1      ascendente      descendente         bajo
 *   1       0,1          0,0      descendente     ascendente          bajo
 *   2       1,0          1,1      descendente     ascendente          alto
 *   3       1,1          1,0      ascendente      descendente         alto
 * --------------------------------------------------------------------------------
 * 
 * Los modos más utilizados son el 0 (0,0 También conocido como MicroWire) 
 * y el 3 (1,1). Maestro y esclavo deben estar en el mismo modo para poder 
 * comunicarse adecuadamente.
*/
#define SPI_SW_MODE_00	0 //Modo conocido tambien como MicroWire
#define SPI_SW_MODE_01	1
#define SPI_SW_MODE_10	2
#define SPI_SW_MODE_11	3

//Definición de tipos de datos según bits necesarios
#if SPI_SW_NUM_BITS_TX > 8
	#if SPI_SW_NUM_BITS_TX > 16
		typedef uint32_t tipo_dato_tx_spi_sw; 
	#else
		typedef uint16_t tipo_dato_tx_spi_sw; 
	#endif
#else
typedef uint8_t tipo_dato_tx_spi_sw;
#endif

#if SPI_SW_NUM_BITS_RX > 8
	#if SPI_SW_NUM_BITS_RX > 16
		typedef uint32_t tipo_dato_rx_spi_sw;
	#else
		typedef uint16_t tipo_dato_rx_spi_sw; 
	#endif
#else
typedef uint8_t tipo_dato_rx_spi_sw;
#endif

#ifndef SPI_SW_MOSI
#define SPI_SW_MOSI 		LATBbits.LATB0
#define SPI_SW_MOSI_TRIS TRISBbits.TRISB0
#endif

#ifndef SPI_SW_MISO
#define SPI_SW_MISO 		PORTBbits.RB1
#define SPI_SW_MISO_TRIS    TRISBbits.TRISB1
#endif

#ifndef SPI_SW_SCK
#define SPI_SW_SCK 		LATBbits.LATB2
#define SPI_SW_SCK_TRIS TRISBbits.TRISB2
#endif


/*
	Prototipos de funciones  
*/
void spi_sw_init(uint8_t mode);
void spi_sw_setMode(uint8_t newMode);        //Establece nuevo modo SPI en caso de usar dispositivos con otros modos de funcionamiento
uint8_t spi_sw_getMode();                     //Lectura del modo actual SPI
void spi_sw_writeByte(uint8_t dato_tx);       //Escribe un byte
uint8_t spi_sw_readByte();                   //Lee un byte
void spi_sw_write(tipo_dato_tx_spi_sw dato_tx);	   //Escribe un dato de longitud predefinida SPI_SW_NUM_BITS_TX
tipo_dato_rx_spi_sw spi_sw_read();					//Lee un dato de longitud predefinida SPI_SW_NUM_BITS_RX

//Funcion de transmisión y recepción simultánea de datos. Por el momento solo se puede utilizar con bytes individuales
uint8_t spi_sw_xmit(uint8_t dato_tx);

//Macros de reloj SCK para transmision de datos en flanco ascendente/descendente
#define SPI_SW_SCK_PULSE_HIGH	SPI_SW_SCK=1;SPI_SW_SCK=0;
#define SPI_SW_SCK_PULSE_LOW	SPI_SW_SCK=0;SPI_SW_SCK=1;

void spi_sw_writeInt16(uint16_t dato_tx);
uint16_t spi_sw_readInt16();

void spi_sw_writeInt24(uint24_t dato_tx);
uint24_t spi_sw_readInt24();

void spi_sw_writeInt32(uint32_t dato_tx);
uint32_t spi_sw_readInt32();

void spi_sw_writeFloat(float dato_tx);
float spi_sw_readFloat();

//Variables internas
static uint8_t spi_sw_mode;
static bool flanco_muestreo, flanco_propagacion, estado_inactivo_reloj;

#endif	/* SPI_SW_H */




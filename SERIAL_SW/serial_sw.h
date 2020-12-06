/*
    Librería que implementa comunicación serial por software en su modalidad 8N1
	Autor: Ing. José Roberto Parra Trewartha
	Compilador: XC8
*/


#define SW_BAUDRATE	9600UL														//Definición de velocidad de comunicación en bits por segundo. Cambiar según necesidades del proyecto.	
#define CYCLES_DELAY_RX_BIT (((((2*_XTAL_FREQ)/(4*SW_BAUDRATE))+1)/2)-14)		//Retardo por bit en recepción
#define CYCLES_DELAY_RX_HALFBIT (((((2*_XTAL_FREQ)/(8*SW_BAUDRATE))+1)/2)-9)	//Retardo por medio bit en recepción
#define CYCLES_DELAY_TX_BIT (((((2*_XTAL_FREQ)/(4*SW_BAUDRATE))+1)/2)-12)		//Retardo por bit en transmisión

/*
	Definición de pines RX y TX para comunicación serial por software
*/
#define SW_RX		PORTBbits.RB7
#define SW_RX_TRIS	TRISBbits.TRISB7

#define SW_TX       LATBbits.LATB6
#define SW_TX_TRIS	TRISBbits.TRISB6

/*
	Prototipos de funciones
*/
void serial_sw_init(void);
uint8_t serial_sw_readByte(void);
void serial_sw_writeByte(uint8_t dato);
void serial_sw_puts(const char *cadena);
void serial_sw_writeLine(const char *cadena); 	
void serial_sw_gets(uint8_t *buffer, uint16_t len);
void serial_sw_writeInt16(uint16_t dato);
void serial_sw_writeInt24(uint24_t dato);
void serial_sw_writeInt32(uint32_t dato);
void serial_sw_writeFloat(float dato);
void serial_sw_write(void* datos, uint16_t len);
uint16_t serial_sw_readInt16(void);
uint24_t serial_sw_readInt24(void);
uint32_t serial_sw_readInt32(void);
float serial_sw_readFloat(void);
void serial_sw_read(void* datos, uint16_t len);




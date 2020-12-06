/*
    Librería de comunicación spi por software para microcontroladores PIC de 8 bits
    Autor: Ing. José Roberto Parra Trewartha
    Compilador: XC8
 */
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "pconfig.h"
#include "spi_sw.h"
#include "funciones_auxiliares.h"

//Inicializacion del bus SPI por software en el modo adecuado
void spi_sw_init(uint8_t mode)
{
    //Configuración de pines SPI como E/S según corresponda (Salidas: MOSI y SCK, Entradas: MISO). 
    SPI_SW_MOSI_TRIS = 0;
    SPI_SW_MISO_TRIS = 1;
    SPI_SW_SCK_TRIS = 0; 
    //El usuario debe implementar la cantidad necesaria de salidas digitales de CS (Chip select), según la cantidad
    //de dispositivos en el bus SPI.
    SPI_SW_MOSI = 0;
    spi_sw_setMode(mode);
}

//Función para cambiar de modo SPI por software. Se uitiliza cuando en el bus se tienen dispositivos con modos de funcionamiento diferentes
void spi_sw_setMode(uint8_t newMode)
{
    //Según el modo SPI, se configuran flanco de propagación y muestreo y estado
    //inactivo del reloj.
    switch(newMode) //Inicialización de la terminal de reloj según el modo SPI a configurar
    {
        case SPI_SW_MODE_01:
            spi_sw_mode = SPI_SW_MODE_01;
            estado_inactivo_reloj = 0; //Estado inactivo en bajo (idle low)
            flanco_muestreo = 0;
            flanco_propagacion = 1;
            break;
        case SPI_SW_MODE_10:
            spi_sw_mode = SPI_SW_MODE_10;
            estado_inactivo_reloj = 1; //Estado inactivo en alto (idle high)
            flanco_muestreo = 0;
            flanco_propagacion = 1;
            break;
        case SPI_SW_MODE_11:
            spi_sw_mode = SPI_SW_MODE_11;
            estado_inactivo_reloj = 1; //Estado inactivo en alto (idle high)
            flanco_muestreo = 1; 
            flanco_propagacion = 0; 
            break;
        default: //Modo SPI 0 es el predeterminado
            spi_sw_mode = SPI_SW_MODE_00;
            estado_inactivo_reloj = 0; //Estado inactivo en bajo (idle low)
            flanco_muestreo = 1; 
            flanco_propagacion = 0; 
            break;
    }
    SPI_SW_SCK = estado_inactivo_reloj; //Inicializa reloj en estado inactivo
}

/*
    Función que obtiene el modo de funcionamiento actual del bus SPI por software
*/
uint8_t spi_sw_getMode()
{
    return spi_sw_mode;
}

void spi_sw_write(tipo_dato_tx_spi_sw dato_tx) //OK!!!!
{
    #if SPI_SW_NUM_BITS_TX>8
        uint16_t mask=(0x0001<<(SPI_SW_NUM_BITS_TX-1));
    #else
        uint8_t mask=(0x01<<(SPI_SW_NUM_BITS_TX-1));
    #endif
    //Todos los modos SPI comienzan en estado de propagación, por tanto:
    SPI_SW_SCK = flanco_propagacion;
    __delay_us(1);
    //A continuacion el corrimiento de bits
    for(uint8_t contador=SPI_SW_NUM_BITS_TX;contador;contador--)
    {
        //Establece MOSI con el estado del bit más significativo del dato a transmitir
        SPI_SW_MOSI =(dato_tx & mask)? 1:0; 
        dato_tx<<=1; //Corrimiento a la izquierda del dato
        SPI_SW_SCK = flanco_muestreo;
        __delay_us(1);
        if(contador!=1) //En el último pulso de reloj, se deja sin flanco de propagacion
            SPI_SW_SCK = flanco_propagacion; 
    }  
    //Al final regresa al reloj a su estado inactivo
    SPI_SW_SCK = estado_inactivo_reloj;
    
}

void spi_sw_writeByte(uint8_t dato_tx)
{
    uint8_t mask=0x80, i;
    //Todos los modos SPI comienzan en estado de propagacion, por tanto:
    SPI_SW_SCK = flanco_propagacion;
    __delay_us(1);
    //A continuacion el corrimiento de bits
    for( i = 8; i ; i-- )
    {
        SPI_SW_MOSI =(dato_tx & mask)? 1:0; //Establece MOSI con el estado del bit m�s significativo del dato a transmitir
        //En cualquier modo SPI, los datos est�n listos antes de cualquier pulso de reloj
        dato_tx<<=1; //Corrimiento a la izquierda del dato
        SPI_SW_SCK = flanco_muestreo;
        __delay_us(1);
        if(i!=1) //En el ultimo pulso de reloj, se deja sin flanco de propagacion
            SPI_SW_SCK = flanco_propagacion;    
    }    
    //Al final regresa al reloj a su estado inactivo
    SPI_SW_SCK = estado_inactivo_reloj;
}

uint8_t spi_sw_readByte()
{
    uint8_t dato=0,i;
    //Todos los modos SPI comienzan en estado de propagacion, por tanto:
    SPI_SW_SCK = flanco_propagacion;
    __delay_us(1);
    SPI_SW_MOSI = 0; //Por default, manda 0x0000
    for (i = 8; i ; i--)
    {
        dato <<= 1;
        SPI_SW_SCK = flanco_muestreo;
        if(SPI_SW_MISO)
            dato|=0x01;
        if(i!=1) //En el ultimo pulso de reloj, se deja sin flanco de propagacion
            SPI_SW_SCK = flanco_propagacion; 
    }
    //Al final regresa al reloj a su estado inactivo
    SPI_SW_SCK = estado_inactivo_reloj;
    return dato;
}

tipo_dato_rx_spi_sw spi_sw_read()
{
    #if SPI_SW_NUM_BITS_TX>8
        uint16_t dato = 0;
    #else
        uint8_t dato = 0;
    #endif
    //Todos los modos SPI comienzan en estado de propagacion, por tanto:
    SPI_SW_SCK = flanco_propagacion;
    __delay_us(1);
    SPI_SW_MOSI = 0; //Por default, manda 0x0000
    for (uint8_t i = SPI_SW_NUM_BITS_RX; i ;  i--)
    {
        dato <<= 1;
        SPI_SW_SCK = flanco_muestreo;
        if(SPI_SW_MISO)
            dato|=0x01;
        if(i!=1) //En el ultimo pulso de reloj, se deja sin flanco de propagacion
            SPI_SW_SCK = flanco_propagacion; 
        
    }
    //Al final regresa al reloj a su estado inactivo
    SPI_SW_SCK = estado_inactivo_reloj;
    return dato;
}

//Por verificar funcionamiento, por el momento solo con 1 byte a transmitir y uno a recibir
uint8_t spi_sw_xmit(uint8_t dato_tx)
{
    //Todos los modos SPI comienzan en estado de propagacion, por tanto:
    SPI_SW_SCK = flanco_propagacion;
    __delay_us(1);
    uint8_t i, mask=0x80;//(0x01<<(SPI_NUM_BITS_TX-1));
    for( i = 8 ; i ; i--)
    {
        SPI_SW_MOSI = ( dato_tx & mask )? 1:0; //Establece MOSI con el estado del bit más significativo del dato a transmitir
        //En cualquier modo SPI, los datos están listos antes de cualquier pulso de reloj
        dato_tx <<= 1; //Corrimiento a la izquierda del dato

        SPI_SW_SCK = flanco_muestreo;
        if(SPI_SW_MISO)
            dato_tx|=0x01;
        if(i!=1) //En el ultimo pulso de reloj, se deja sin flanco de propagacion
            SPI_SW_SCK = flanco_propagacion;    
    }    
    //Al final regresa al reloj a su estado inactivo
    SPI_SW_SCK = estado_inactivo_reloj;
    return dato_tx;  
}


void spi_sw_writeInt16(uint16_t dato_tx)
{
    uint8_t i;
    for(i=0;i!=sizeof(uint16_t);i++)
    {
        spi_sw_writeByte(*((uint8_t *)&dato_tx+i));
    }
}
uint16_t spi_sw_readInt16()
{
    uint16_t dato_leido=0;
    uint8_t * p_lectura;                    //Apuntador a variable entera de 16 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 16 bits
    for(uint8_t i=0;i!=sizeof(uint16_t);i++)
    {
        *(p_lectura++) = spi_sw_readByte();      //Recepción de datos
    }
    return dato_leido;
}

void spi_sw_writeInt24(uint24_t dato_tx)
{
    uint8_t i;
    for(i=0;i!=sizeof(uint24_t);i++)
    {
        spi_sw_writeByte(*((uint8_t *)&dato_tx+i));
    }
}
uint24_t spi_sw_readInt24()
{
    uint24_t dato_leido=0;
    uint8_t * p_lectura;                    //Apuntador a variable entera de 24 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 24 bits
    for(uint8_t i=0;i!=sizeof(uint24_t);i++)
    {
        *(p_lectura++) = spi_sw_readByte();      //Recepción de datos
    }
    return dato_leido;
}

void spi_sw_writeInt32(uint32_t dato_tx)
{
    uint8_t i;
    for(i=0;i!=sizeof(uint32_t);i++)
    {
        spi_sw_writeByte(*((uint8_t *)&dato_tx+i));
    }
}

uint32_t spi_sw_readInt32()
{
    uint32_t dato_leido=0;
    uint8_t * p_lectura;                    //Apuntador a variable entera de 32 bits
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo entero de 32 bits
    for(uint8_t i=0;i!=sizeof(uint32_t);i++)
    {
        *(p_lectura++) = spi_sw_readByte();      //Recepción de datos
    }
    return dato_leido;
}

void spi_sw_writeFloat(float dato_tx)
{
    uint8_t i;
    for(i=0;i!=sizeof(float);i++)
    {
        spi_sw_writeByte(*((uint8_t *)&dato_tx+i));
    } 
}

float spi_sw_readFloat()
{
    float dato_leido;
    uint8_t * p_lectura;                    //Apuntador a variable flotante
    p_lectura = (uint8_t *)&dato_leido;     //Asigna dirección de valor de retorno de tipo flotante
    for(uint8_t i=0;i!=sizeof(float);i++)
    {
        *(p_lectura++) = spi_sw_readByte();      //Recepción de datos
    }
    return dato_leido;
}


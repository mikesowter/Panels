#ifndef OneWire_h
#define OneWire_h

#include <inttypes.h>

#if ARDUINO >= 100
#include "Arduino.h"       // for delayMicroseconds, digitalPinToBitMask, etc
#else
#include "WProgram.h"      // for delayMicroseconds
#include "pins_arduino.h"  // for digitalPinToBitMask, etc
#endif

// You can exclude certain features from OneWire.  In theory, this
// might save some space.  In practice, the compiler automatically
// removes unused code (technically, the linker, using -fdata-sections
// and -ffunction-sections when compiling, and Wl,--gc-sections
// when linking), so most of these will not result in any code size
// reduction.  Well, unless you try to use the missing features
// and redesign your program to not need them!  ONEWIRE_CRC8_TABLE
// is the exception, because it selects a fast but large algorithm
// or a small but slow algorithm.

// you can exclude onewire_search by defining that to 0
#ifndef ONEWIRE_SEARCH
#define ONEWIRE_SEARCH 1
#endif

// You can exclude CRC checks altogether by defining this to 0
#ifndef ONEWIRE_CRC
#define ONEWIRE_CRC 1
#endif

// Select the table-lookup method of computing the 8-bit CRC
// by setting this to 1.  The lookup table enlarges code size by
// about 250 bytes.  It does NOT consume RAM (but did in very
// old versions of OneWire).  If you disable this, a slower
// but very compact algorithm is used.
#ifndef ONEWIRE_CRC8_TABLE
#define ONEWIRE_CRC8_TABLE 1
#endif

// You can allow 16-bit CRC checks by defining this to 1
// (Note that ONEWIRE_CRC must also be 1.)
#ifndef ONEWIRE_CRC16
#define ONEWIRE_CRC16 1
#endif

#define FALSE 0
#define TRUE  1

// Platform specific I/O definition

/*  all these defined in esp8266_peri.h in folder C:\.platformio\packages\framework-arduinoespressif8266\cores\esp8266

#define ESP8266_REG(addr) *((volatile uint32_t *)(0x60000000+(addr)))

//GPIO (0-15) Control Registers
#define GPO    ESP8266_REG(0x300) //GPIO_OUT R/W (Output Level)
#define GPOS   ESP8266_REG(0x304) //GPIO_OUT_SET WO
#define GPOC   ESP8266_REG(0x308) //GPIO_OUT_CLR WO
#define GPE    ESP8266_REG(0x30C) //GPIO_ENABLE R/W (Enable)
#define GPES   ESP8266_REG(0x310) //GPIO_ENABLE_SET WO
#define GPEC   ESP8266_REG(0x314) //GPIO_ENABLE_CLR WO
#define GPI    ESP8266_REG(0x318) //GPIO_IN RO (Read Input Level)
#define GPIE   ESP8266_REG(0x31C) //GPIO_STATUS R/W (Interrupt Enable)
#define GPIES  ESP8266_REG(0x320) //GPIO_STATUS_SET WO
#define GPIEC  ESP8266_REG(0x324) //GPIO_STATUS_CLR WO


#define GPOP(p) ((GPO & (1 << ((p) & 0xF))) != 0)
#define GPEP(p) ((GPE & (1 << ((p) & 0xF))) != 0)
#define GPIP(p) ((GPI & (1 << ((p) & 0xF))) != 0)
#define GPIEP(p) ((GPIE & (1 << ((p) & 0xF))) != 0)

//GPIO (0-15) PIN Control Registers
#define GPC(p) ESP8266_REG(0x328 + ((p & 0xF) * 4))
#define GPC0   ESP8266_REG(0x328) //GPIO_PIN0
#define GPC1   ESP8266_REG(0x32C) //GPIO_PIN1
#define GPC2   ESP8266_REG(0x330) //GPIO_PIN2
#define GPC3   ESP8266_REG(0x334) //GPIO_PIN3
#define GPC4   ESP8266_REG(0x338) //GPIO_PIN4
#define GPC5   ESP8266_REG(0x33C) //GPIO_PIN5
#define GPC6   ESP8266_REG(0x340) //GPIO_PIN6
#define GPC7   ESP8266_REG(0x344) //GPIO_PIN7
#define GPC8   ESP8266_REG(0x348) //GPIO_PIN8
#define GPC9   ESP8266_REG(0x34C) //GPIO_PIN9
#define GPC10  ESP8266_REG(0x350) //GPIO_PIN10
#define GPC11  ESP8266_REG(0x354) //GPIO_PIN11
#define GPC12  ESP8266_REG(0x358) //GPIO_PIN12
#define GPC13  ESP8266_REG(0x35C) //GPIO_PIN13
#define GPC14  ESP8266_REG(0x360) //GPIO_PIN14
#define GPC15  ESP8266_REG(0x364) //GPIO_PIN15

//GPIO (0-15) PIN Control Bits
#define GPCWE  10 //WAKEUP_ENABLE (can be 1 only when INT_TYPE is high or low)
#define GPCI   7  //INT_TYPE (3bits) 0:disable,1:rising,2:falling,3:change,4:low,5:high
#define GPCD   2  //DRIVER 0:normal,1:open drain
#define GPCS   0  //SOURCE 0:GPIO_DATA,1:SigmaDelta

#define GPMUX  ESP8266_REG(0x800)
//GPIO (0-15) PIN Function Registers
#define GPF0   ESP8266_REG(0x834)
#define GPF1   ESP8266_REG(0x818)
#define GPF2   ESP8266_REG(0x838)
#define GPF3   ESP8266_REG(0x814)
#define GPF4   ESP8266_REG(0x83C)
#define GPF5   ESP8266_REG(0x840)
#define GPF6   ESP8266_REG(0x81C)
#define GPF7   ESP8266_REG(0x820)
#define GPF8   ESP8266_REG(0x824)
#define GPF9   ESP8266_REG(0x828)
#define GPF10  ESP8266_REG(0x82C)
#define GPF11  ESP8266_REG(0x830)
#define GPF12  ESP8266_REG(0x804)
#define GPF13  ESP8266_REG(0x808)
#define GPF14  ESP8266_REG(0x80C)
#define GPF15  ESP8266_REG(0x810)

extern uint8_t esp8266_gpioToFn[16];
#define GPF(p) ESP8266_REG(0x800 + esp8266_gpioToFn[(p & 0xF)])

//GPIO (0-15) PIN Function Bits
#define GPFSOE 0 //Sleep OE
#define GPFSS  1 //Sleep Sel
#define GPFSPD 2 //Sleep Pulldown
#define GPFSPU 3 //Sleep Pullup
#define GPFFS0 4 //Function Select bit 0
#define GPFFS1 5 //Function Select bit 1
#define GPFPD  6 //Pulldown
#define GPFPU  7 //Pullup
#define GPFFS2 8 //Function Select bit 2
#define GPFFS(f) (((((f) & 4) != 0) << GPFFS2) | ((((f) & 2) != 0) << GPFFS1) | ((((f) & 1) != 0) << GPFFS0))
#define GPFFS_GPIO(p) (((p)==0||(p)==2||(p)==4||(p)==5)?0:((p)==16)?1:3)
#define GPFFS_BUS(p) (((p)==1||(p)==3)?0:((p)==2||(p)==12||(p)==13||(p)==14||(p)==15)?2:((p)==0)?4:1)
*/

#if defined(ARDUINO_ARCH_ESP8266)
#define PIN_TO_BASEREG(pin)             ((volatile uint32_t*) GPO)
#define PIN_TO_BITMASK(pin)             (1 << pin)
#define IO_REG_TYPE uint32_t
#define IO_REG_ASM
#define DIRECT_READ(base, mask)         ((GPI & (mask)) ? 1 : 0)    //GPIO_IN_ADDRESS
#define DIRECT_MODE_INPUT(base, mask)   (GPE &= ~(mask))            //GPIO_ENABLE_W1TC_ADDRESS
#define DIRECT_MODE_OUTPUT(base, mask)  (GPE |= (mask))             //GPIO_ENABLE_W1TS_ADDRESS
#define DIRECT_WRITE_LOW(base, mask)    (GPOC = (mask))             //GPIO_OUT_W1TC_ADDRESS
#define DIRECT_WRITE_HIGH(base, mask)   (GPOS = (mask))             //GPIO_OUT_W1TS_ADDRESS

#else
#error "Please define I/O register types here"
#endif


class OneWire
{
  private:
    IO_REG_TYPE bitmask;
    volatile IO_REG_TYPE *baseReg;

#if ONEWIRE_SEARCH
    // global search state
    unsigned char ROM_NO[8];
    uint8_t LastDiscrepancy;
    uint8_t LastFamilyDiscrepancy;
    uint8_t LastDeviceFlag;
#endif

  public:
    OneWire( uint8_t pin);

    // Perform a 1-Wire reset cycle. Returns 1 if a device responds
    // with a presence pulse.  Returns 0 if there is no device or the
    // bus is shorted or otherwise held low for more than 250uS
    uint8_t reset(void);

    // Issue a 1-Wire rom select command, you do the reset first.
    void select(const uint8_t rom[8]);

    // Issue a 1-Wire rom skip command, to address all on bus.
    void skip(void);

    // Write a byte. If 'power' is one then the wire is held high at
    // the end for parasitically powered devices. You are responsible
    // for eventually depowering it by calling depower() or doing
    // another read or write.
    void write(uint8_t v, uint8_t power = 0);

    void write_bytes(const uint8_t *buf, uint16_t count, bool power = 0);

    // Read a byte.
    uint8_t read(void);

    void read_bytes(uint8_t *buf, uint16_t count);

    // Write a bit. The bus is always left powered at the end, see
    // note in write() about that.
    void write_bit(uint8_t v);

    // Read a bit.
    uint8_t read_bit(void);

    // Stop forcing power onto the bus. You only need to do this if
    // you used the 'power' flag to write() or used a write_bit() call
    // and aren't about to do another read or write. You would rather
    // not leave this powered if you don't have to, just in case
    // someone shorts your bus.
    void depower(void);

#if ONEWIRE_SEARCH
    // Clear the search state so that if will start from the beginning again.
    void reset_search();

    // Setup the search to find the device type 'family_code' on the next call
    // to search(*newAddr) if it is present.
    void target_search(uint8_t family_code);

    // Look for the next device. Returns 1 if a new address has been
    // returned. A zero might mean that the bus is shorted, there are
    // no devices, or you have already retrieved all of them.  It
    // might be a good idea to check the CRC to make sure you didn't
    // get garbage.  The order is deterministic. You will always get
    // the same devices in the same order.
    uint8_t search(uint8_t *newAddr);
#endif

#if ONEWIRE_CRC
    // Compute a Dallas Semiconductor 8 bit CRC, these are used in the
    // ROM and scratchpad registers.
    static uint8_t crc8(const uint8_t *addr, uint8_t len);

#if ONEWIRE_CRC16
    // Compute the 1-Wire CRC16 and compare it against the received CRC.
    // Example usage (reading a DS2408):
    //    // Put everything in a buffer so we can compute the CRC easily.
    //    uint8_t buf[13];
    //    buf[0] = 0xF0;    // Read PIO Registers
    //    buf[1] = 0x88;    // LSB address
    //    buf[2] = 0x00;    // MSB address
    //    WriteBytes(net, buf, 3);    // Write 3 cmd bytes
    //    ReadBytes(net, buf+3, 10);  // Read 6 data bytes, 2 0xFF, 2 CRC16
    //    if (!CheckCRC16(buf, 11, &buf[11])) {
    //        // Handle error.
    //    }
    //
    // @param input - Array of bytes to checksum.
    // @param len - How many bytes to use.
    // @param inverted_crc - The two CRC16 bytes in the received data.
    //                       This should just point into the received data,
    //                       *not* at a 16-bit integer.
    // @param crc - The crc starting value (optional)
    // @return True, iff the CRC matches.
    static bool check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc, uint16_t crc = 0);

    // Compute a Dallas Semiconductor 16 bit CRC.  This is required to check
    // the integrity of data received from many 1-Wire devices.  Note that the
    // CRC computed here is *not* what you'll get from the 1-Wire network,
    // for two reasons:
    //   1) The CRC is transmitted bitwise inverted.
    //   2) Depending on the endian-ness of your processor, the binary
    //      representation of the two-byte return value may have a different
    //      byte order than the two bytes you get from 1-Wire.
    // @param input - Array of bytes to checksum.
    // @param len - How many bytes to use.
    // @param crc - The crc starting value (optional)
    // @return The CRC16, as defined by Dallas Semiconductor.
    static uint16_t crc16(const uint8_t* input, uint16_t len, uint16_t crc = 0);
#endif
#endif
};

#endif

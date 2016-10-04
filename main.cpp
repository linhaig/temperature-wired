#include "mbed.h"
#include "MAX14720.h"

// I2C Master 2
I2C i2c2(I2C2_SDA, I2C2_SCL);

#define  I2C_ADDR_PMIC   (0x54)
MAX14720 max14720(&i2c2,I2C_ADDR_PMIC);

DigitalOut led(LED1);
InterruptIn button(SW1);

void turnOff()
{
    max14720.shutdown();
}

int main()
{
    int result;
    button.fall(&turnOff);
    led = 0;
    max14720.boostEn = MAX14720::BOOST_ENABLED;
    result = max14720.init();
    if (result == MAX14720_ERROR) printf("Error initializing MAX14720");
    wait(1);
    while(1) {
        max14720.boostSetMode(MAX14720::BOOST_DISABLED);
        max14720.boostEn = MAX14720::BOOST_ENABLED;
        wait(0.5);
        max14720.boostSetVoltage(2500);
        wait(0.5);
        max14720.boostSetVoltage(5000);
        wait(0.5);
    }
}

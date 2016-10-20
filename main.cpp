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
    // Write the power down command to the PMIC
    max14720.shutdown();
}

int main()
{
    // Assign turnOff function to falling edge of button
    button.fall(&turnOff);

    // Turn LED signal on to make buck-boost output visible
    led = 0;

    // Override the default value of boostEn to BOOST_ENABLED
    max14720.boostEn = MAX14720::BOOST_ENABLED;
    // Note that writing the local value does directly affect the part
    // The buck-boost regulator will remain off until init is called

    // Call init to apply all settings to the PMIC
    if (max14720.init() == MAX14720_ERROR) {
        printf("Error initializing MAX14720");
    }

    // Wait 1 second to see the buck-boost regulator turn on
    wait(1);

    while(1) {
        // Turn off the buck-boost regulators
        max14720.boostSetMode(MAX14720::BOOST_DISABLED);
        wait(0.5);

        // Change the voltage of the buck-boost regulator and enable it
        max14720.boostSetVoltage(2500);
        max14720.boostSetMode(MAX14720::BOOST_ENABLED);
        wait(0.5);

        // Change the voltage of the buck-boost regulator
        // Note that the MAX14720 cannot change the buck-boost voltage while
        // it is enabled so if boostEn is set to BOOST_ENABLED, this
        // function will disable it, change the voltage, and re-enable it.
        max14720.boostSetVoltage(5000);
        wait(0.5);
    }
}

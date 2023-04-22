#include "mbed.h"
#include "C12832.h"
// Include the libraries necessary for the program, including the Mbed and LCD libraries

C12832 lcd(p5, p7, p6, p8, p11);
// Initialize the LCD display object with the appropriate pins

InterruptIn joystickcenter(p14);
InterruptIn button(p9);
// Initialize the InterruptIn objects for the joystick button and the external button

DigitalOut led(LED1);
DigitalOut flash(LED4);
// Initialize the DigitalOut objects for the LEDs

volatile int var = 0;
// Declare a volatile integer variable called "var" to be set by the interrupt service routine

Timer debounce;
// Declare a Timer object called "debounce" to debounce button presses

void flip() {
    if(debounce.read_ms() > 50) {
        led = !led;
        var = 1;
        debounce.reset();
        // Debounce the button presses using the Timer class
    }
    debounce.start();
}

int main() {
    debounce.start();
    // Start the debounce timer at the beginning of the program

    joystickcenter.rise(&flip);
    button.mode(PullUp);
    button.rise(&flip);
    // Attach the "flip" function to the rising edges of the joystick button and external button, and set the mode of the external button to PullUp

    while(1) {
        flash = !flash;
        wait(0.25);
        // Toggle the state of the flashing LED every quarter of a second

        if(var) {
            lcd.cls();
            lcd.printf("Button pressed!");
            var = 0;
            // If the "var" variable has been set by the interrupt service routine, clear the LCD screen, print a message saying "Button pressed!", and reset the variable to 0
        }
    }
}

#include <stdio.h> 
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22    
#define LED_PIN 12     
#define DUTY_180 2400   
#define DUTY_90 1470  
#define DUTY_0 500      
#define INCREMENTO 5  
#define ATRASO_MS 10    
#define PWM_FREQ 50    
#define PWM_WRAP 20000  



void setup_led(uint pin, float brightness) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint wrap = 255; 
    pwm_set_wrap(slice_num, wrap);  
    pwm_set_clkdiv(slice_num, 125.0);

    uint duty = (uint)(brightness * wrap); 
    pwm_set_gpio_level(pin, duty);
    pwm_set_enabled(slice_num, true);
}

void setup_pwm(uint pin, uint wrap, uint duty) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin); 

    pwm_set_wrap(slice_num, wrap);   
    pwm_set_clkdiv(slice_num, 125.0);
    pwm_set_gpio_level(pin, duty);   

    pwm_set_enabled(slice_num, true); 
}

void mover_servemotor() {
    int duty = DUTY_0;  

    // Movimenta do 0 para 180 graus
    while (duty <= DUTY_180) {
        setup_pwm(SERVO_PIN, PWM_WRAP, duty); 
        float brightness = (duty - DUTY_0) / (float)(DUTY_180 - DUTY_0); 
        setup_led(LED_PIN, brightness);  
        printf("Posicionar servo em %dµs, brilho LED %.2f\n", duty, brightness);
        sleep_ms(ATRASO_MS);
        duty += INCREMENTO;  
}

    // Movimenta de volta do 180 para 0 graus
    while (duty >= DUTY_0) {
        setup_pwm(SERVO_PIN, PWM_WRAP, duty); 
        float brightness = (duty - DUTY_0) / (float)(DUTY_180 - DUTY_0); 
        setup_led(LED_PIN, brightness);  
        printf("Posicionar serve em %dµs, brilho LED %.2f\n", duty, brightness);
        sleep_ms(ATRASO_MS); 
        duty -= INCREMENTO; 
    }
}

int main() {
    stdio_init_all(); 
    
    // Posiciona o servo em 0 graus (ciclo de 500µs)
    setup_pwm(SERVO_PIN, PWM_WRAP, DUTY_0); 
    setup_led(LED_PIN, 0.0);  
    printf("Posicionado - 0 graus, LED desligado.\n");
    sleep_ms(5000); 

    // Posiciona o servo em 180 graus (ciclo de 2.400µs)
    setup_pwm(SERVO_PIN, PWM_WRAP, DUTY_180); 
    setup_led(LED_PIN, 1.0);  
    printf("Posicionado - 180 graus, LED aceso.\n");
    sleep_ms(5000);

    // Posiciona o servo em 90 graus (ciclo de 1.470µs)
    setup_pwm(SERVO_PIN, PWM_WRAP, DUTY_90); 
    setup_led(LED_PIN, 0.5); 
    printf("Posicionado - 90 graus, LED com brilho médio.\n");
    sleep_ms(5000);

    printf("Movimenta suave.\n");
    while (1) {
        mover_servemotor();  
    }

    printf("Fim.\n");
    return 0;
}

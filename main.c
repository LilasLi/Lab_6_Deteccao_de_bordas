#define F_CPU   16000000UL
#include <util/delay.h>
#include <avr/io.h>

char digitos7seg[10] = {
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F
};

void disp7seg_init(void)
{
    DDRD  = 0xFF;     // PORTD como saída (segmentos)
    PORTD = 0x00;

    DDRB |= (1 << PB0) | (1 << PB1);  // PB0 = D8 (uni), PB1 = D9 (dez)
    PORTB &= ~((1 << PB0) | (1 << PB1));

    DDRB &= ~((1 << PB4) | (1 << PB5));  // D12 (PB4), D13 (PB5) como entrada
    PORTB |= (1 << PB4) | (1 << PB5);    // pull-up ativado nos botões
}

void exibir_duplo(unsigned char dez, unsigned char uni)
{
    // Unidade
    PORTD = digitos7seg[uni];
    PORTB |= (1 << PB0);   // Ativa transistor da unidade (D8)
    PORTB &= ~(1 << PB1);  // Desativa transistor da dezena
    _delay_ms(5);

    // Dezena
    PORTD = digitos7seg[dez];
    PORTB |= (1 << PB1);   // Ativa transistor da dezena (D9)
    PORTB &= ~(1 << PB0);  // Desativa transistor da unidade
    _delay_ms(5);
}

int main(void)
{
    disp7seg_init();
    unsigned char contador = 0;
    unsigned char botao_0_ant = 1;
    unsigned char botao_1_ant = 1;

    while (1)
    {
        // Atualiza display
        for (int i = 0; i < 20; i++)
            exibir_duplo(contador / 10, contador % 10);

        // Lê estado dos botões
        unsigned char botao_0 = PINB & (1 << PB4); // D12
        unsigned char botao_1 = PINB & (1 << PB5); // D13

        // Borda de descida no botão 0 (decrementa)
        if (botao_0_ant && !botao_0)
        {
            if (contador > 0)
                contador--;
        }

        // Borda de descida no botão 1 (incrementa)
        if (botao_1_ant && !botao_1)
        {
            if (contador < 99)
                contador++;
        }

        // Atualiza estados antigos
        botao_0_ant = botao_0;
        botao_1_ant = botao_1;

        _delay_ms(50); // Debounce simples
    }
}

/************************************************************************
 * 5 semestre - Eng. da Computao - Insper - APS 1
 * Rafael Malcervelli - rafaelsm9@al.insper.edu.br
 * Gabriel Mitelman Tkacz - gabrielmt2@al.insper.edu.br
 *
 * Material:
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 *
 * Objetivo:
 *  - Nesta APS voc?s ir?o desenvolver um sistema embarcado que reproduz uma m?sica monofonia, 
 *	  para isso, ir?o utilizar um buzzer conectado a um pino do microcontrolador.
 *
 * Perif?ricos:
 *  - TC
 *	- RTT
 *	- RTC
 *
 * Log:
 *  - 09/2021: Criacao
 ************************************************************************/

#include "asf.h"
#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"
#include "musics.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/
// LED1
#define LED1_PIO      PIOA
#define LED1_PIO_ID   ID_PIOA
#define LED1_IDX      0
#define LED1_IDX_MASK (1 << LED1_IDX)

// LED2
#define LED2_PIO      PIOC
#define LED2_PIO_ID   ID_PIOC
#define LED2_IDX      30
#define LED2_IDX_MASK (1 << LED2_IDX)

// LED3
#define LED3_PIO      PIOB
#define LED3_PIO_ID   ID_PIOB
#define LED3_IDX      2
#define LED3_IDX_MASK (1 << LED3_IDX)

// Botao 1 - PLAY/PAUSE
#define BUT1_PIO      PIOD
#define BUT1_PIO_ID   ID_PIOD
#define BUT1_IDX      28
#define BUT1_IDX_MASK (1 << BUT1_IDX)

// Botao 2 - SELECAO MUSICAL
#define BUT2_PIO      PIOC
#define BUT2_PIO_ID   ID_PIOC
#define BUT2_IDX      31
#define BUT2_IDX_MASK (1 << BUT2_IDX)

// Buzzer
#define BUZ_PIO      PIOD
#define BUZ_PIO_ID   ID_PIOD
#define BUZ_IDX      30
#define BUZ_IDX_MASK (1 << BUZ_IDX)

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/
/* flags */
 volatile Bool play;
 volatile int selecionado;
 volatile Bool trocou;
 
 /************************************************************************/
 /* prototype                                                            */
 /************************************************************************/
 void io_init(void);
 void LED1_init(int estado);
 void LED2_init(int estado);
 void LED3_init(int estado);
 void pisca_led1(int n, int t);
 void pisca_led2(int n, int t);
 void pisca_led3(int n, int t);
 void but1_callback(void);
 void but2_callback(void);
 void choice (void);
 void tone(int freq, int time);
 void toca(long melodia[]);
 
 /************************************************************************/
 /* funcoes                                                              */
 /************************************************************************/
void but1_callback(void){
	if (!pio_get(BUT1_PIO, PIO_INPUT, BUT1_IDX_MASK)) {
		/*Configura pause e play invertendo o valor de play */
		play = !play;
	}  
}

void but2_callback(void) {
	if (!pio_get(BUT2_PIO, PIO_INPUT, BUT2_IDX_MASK)) {
		trocou = true;
		if (selecionado == 1){
			selecionado = 2;
		} else if (selecionado == 2){
			selecionado = 1;
		}
	}
}

/*
 * @Brief Pisca LED1 placa
 */
void pisca_led1(int n, int t){
  for (int i=0;i<n;i++){
    pio_clear(LED1_PIO, LED1_IDX_MASK);
    delay_ms(t);
    pio_set(LED1_PIO, LED1_IDX_MASK);
    delay_ms(t);
  }
}

/*
 * @Brief Pisca LED2 placa
 */
void pisca_led2(int n, int t){
  for (int i=0;i<n;i++){
    pio_clear(LED2_PIO, LED2_IDX_MASK);
    delay_ms(t);
    pio_set(LED2_PIO, LED2_IDX_MASK);
    delay_ms(t);
  }
}

/*
 * @Brief Pisca LED3 placa
 */
void pisca_led3(int n, int t){
  for (int i=0;i<n;i++){
    pio_clear(LED3_PIO, LED3_IDX_MASK);
    delay_ms(t);
    pio_set(LED3_PIO, LED3_IDX_MASK);
    delay_ms(t);
  }
}

/**
* @Brief Inicializa o pino do LED1
*/
void LED1_init(int estado){
	pmc_enable_periph_clk(LED1_PIO_ID);
	pio_set_output(LED1_PIO, LED1_IDX_MASK, estado, 0, 0);
};

/**
* @Brief Inicializa o pino do LED2
*/
void LED2_init(int estado){
	pmc_enable_periph_clk(LED2_PIO_ID);
	pio_set_output(LED2_PIO, LED2_IDX_MASK, estado, 0, 0);
};

/**
* @Brief Inicializa o pino do LED3
*/
void LED3_init(int estado){
	pmc_enable_periph_clk(LED3_PIO_ID);
	pio_set_output(LED3_PIO, LED3_IDX_MASK, estado, 0, 0);
};

void choice(void){
	if(selecionado == 1){
		pio_set(LED1_PIO,LED1_IDX_MASK);
		pio_clear(LED2_PIO,LED2_IDX_MASK);
		pio_clear(LED3_PIO,LED3_IDX_MASK);
	}
	 
	else if(selecionado == 2){
		pio_set(LED2_PIO,LED2_IDX_MASK);
		pio_clear(LED1_PIO,LED1_IDX_MASK);
		pio_clear(LED3_PIO,LED3_IDX_MASK);
	}
}

/**
 * freq: Frequecia em Hz
 * time: Tempo em ms que o tom deve ser gerado
 */
void tone(int freq, int time){
	int n = (double) freq * ((double) time/1000);
	float T = (1.0/freq) * 1E6; //ms
	
	for (int i = 0; i <= n; i++){
		pio_set(BUZ_PIO, BUZ_IDX_MASK);
		delay_us(T/2);
		pio_clear(BUZ_PIO, BUZ_IDX_MASK);
		delay_us(T/2);
	}
}

void toca(long melodia[]){
	// change this to make the song slower or faster
	int tempo_starwars = 108;
	int tempo_harrypotter = 144;
	
	// this calculates the duration of a whole note in ms
	int wholenote_starwars = (60000 * 4) / (tempo_starwars);
	int wholenote_harrypotter = (60000 * 4) / (tempo_harrypotter);
	
	int notes_starwars = 172;
	int notes_harrypotter = 124;
	// iterate over the notes of the melody.
	// Remember, the array is twice the number of notes (notes + durations)
	
	if (selecionado == 1){
		for (int thisNote = 0; thisNote < abs(notes_starwars); thisNote = thisNote + 2) {
			if (play){
				// calculates the duration of each note
				int divider = melodia[thisNote + 1];
				int noteDuration = (wholenote_starwars) / abs(divider);
				if (divider < 0) {
					noteDuration *= 1.5; // increases the duration in half for dotted notes
				}
				
				if (melodia[thisNote] == 0){
					// Wait for the specief duration before playing the next note.
					delay_us(noteDuration);
				}
				else{
					// we only play the note for 90% of the duration, leaving 10% as a pause
					tone(melodia[thisNote], noteDuration * 0.9);
					

					// Wait for the specief duration before playing the next note.
					delay_us(noteDuration);
				}
			}
			else {
				pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
			}
		}
	}
	if (selecionado == 2){
		for (int thisNote = 0; thisNote < abs(notes_harrypotter); thisNote = thisNote + 2) {
			if (play){
				// calculates the duration of each note
				int divider = melodia[thisNote + 1];
				int noteDuration = (wholenote_harrypotter) / abs(divider);
				if (divider < 0) {
					noteDuration *= 1.5; // increases the duration in half for dotted notes
				}
				
				if (melodia[thisNote] == 0){
					// Wait for the specief duration before playing the next note.
					delay_us(noteDuration);
				}
				else{
					// we only play the note for 90% of the duration, leaving 10% as a pause
					tone(melodia[thisNote], noteDuration * 0.9);
					

					// Wait for the specief duration before playing the next note.
					delay_us(noteDuration);
				}
			}
			else{
				pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
			}
		}
	}
}


void io_init(void){
	// Configura led1
	pmc_enable_periph_clk(LED1_PIO_ID);
	pio_configure(LED1_PIO, PIO_OUTPUT_0, LED1_IDX_MASK, PIO_DEFAULT);
	
	// Configura led2
	pmc_enable_periph_clk(LED2_PIO_ID);
	pio_configure(LED2_PIO, PIO_OUTPUT_0, LED2_IDX_MASK, PIO_DEFAULT);
	
	// Configura led3
	pmc_enable_periph_clk(LED3_PIO_ID);
	pio_configure(LED3_PIO, PIO_OUTPUT_0, LED3_IDX_MASK, PIO_DEFAULT);
	
	// Configura o Buzzer
	pmc_enable_periph_clk(BUZ_PIO_ID);
	pio_configure(BUZ_PIO, PIO_OUTPUT_0, BUZ_IDX_MASK, PIO_DEFAULT);

	// ------ BOTAO 1------
	// Inicializa clock do perif?rico PIO responsavel pelo botao1
	pmc_enable_periph_clk(BUT1_PIO_ID);
		
	pio_configure(BUT1_PIO, PIO_INPUT, BUT1_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT1_PIO, BUT1_IDX_MASK, 60);

	// Configura interrup??o no pino referente ao botao e associa
	// fun??o de callback caso uma interrup??o for gerada
	// a fun??o de callback ? a: but_callback()
	pio_handler_set(BUT1_PIO,
	BUT1_PIO_ID,
	BUT1_IDX_MASK,
	PIO_IT_EDGE,
	but1_callback);

	// Ativa interrup??o
	pio_enable_interrupt(BUT1_PIO, BUT1_IDX_MASK);

	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais pr?ximo de 0 maior)
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_SetPriority(BUT1_PIO_ID, 4); // Prioridade 4
	
	// ------ BOTAO 2------
	// Inicializa clock do perif?rico PIO responsavel pelo botao2
	pmc_enable_periph_clk(BUT2_PIO_ID);
	
	pio_configure(BUT2_PIO, PIO_INPUT, BUT2_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT2_PIO, BUT2_IDX_MASK, 60);

	// Configura interrup??o no pino referente ao botao e associa
	// fun??o de callback caso uma interrup??o for gerada
	// a fun??o de callback ? a: but_callback()
	pio_handler_set(BUT2_PIO,
	BUT2_PIO_ID,
	BUT2_IDX_MASK,
	PIO_IT_EDGE,
	but2_callback);

	// Ativa interrup??o
	pio_enable_interrupt(BUT2_PIO, BUT2_IDX_MASK);

	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais pr?ximo de 0 maior)
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_SetPriority(BUT2_PIO_ID, 4); // Prioridade 4
}

int main (void)
{
	sysclk_init();
	io_init();

	// Init OLED
	gfx_mono_ssd1306_init();
	
	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;

	/* Configura Leds */
	LED1_init(1);
	LED2_init(1);
	LED3_init(1);
	
	/* Settando variaveis iniciais */
	play = false;
	selecionado = 2;

  /* Insert application code here, after the board has been initialized. */
	while(1) {
		choice();
		if (trocou){
			if (selecionado == 1){
				gfx_mono_draw_string("             ", 0,10, &sysfont);
				gfx_mono_draw_string("Star Wars", 0,10, &sysfont);
				play = false;
			} else if (selecionado == 2){
				gfx_mono_draw_string("             ", 0,10, &sysfont);
				gfx_mono_draw_string("Harry Potter", 0,10, &sysfont);
				play = false;
			}
			trocou = false;
		}
		
		if (selecionado == 1 && play){
			toca(melody_starwars);
			
			play = false;
		} else if (selecionado == 2 && play){
			toca(melody_harrypotter);
			play = false;
		}
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
	}
}

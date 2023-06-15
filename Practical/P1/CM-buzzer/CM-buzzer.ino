/* Define the total number of tasks */
#define NT 20

/* Define the analogue pin used to read the potentiometer */
#define POT_DIO 0

/* Define the digital pin used to control the buzzer */
#define BUZZER_DIO 3

#define OFF HIGH
#define ON LOW

typedef struct {
  /* period in ticks */
  int period;

  /* ticks until next activation */
  int delay;

  /* function pointer */
  void (*func)(void);

  /* activation counter */
  int exec;
} Sched_Task_t;

Sched_Task_t Tasks[NT];

int Sched_Init(void) {
  for(int x=0; x<NT; x++) {
    Tasks[x].func = 0;
  }
}

int Sched_AddT(void (*f)(void), int d, int p) {
  for(int x=0; x<NT; x++) {
    if (!Tasks[x].func) {
      Tasks[x].period = p;
      Tasks[x].delay = d;
      Tasks[x].exec = 0;
      Tasks[x].func = f;
      return x;
    }
  }
  return -1;
}

void Sched_Schedule(void) {
  for(int x=0; x<NT; x++) {
    if(Tasks[x].func) {
      if(Tasks[x].delay) {
        Tasks[x].delay--;
      } else {
        /* Schedule Task */
        Tasks[x].exec = 1;
        Tasks[x].delay = Tasks[x].period-1;
      }
    }
  }
}

void Sched_Dispatch(void) {
  for(int x=0; x<NT; x++) {
    if((Tasks[x].func) && (Tasks[x].exec)) {
      Tasks[x].exec=0;
      Tasks[x].func();

      /* Delete task if one-shot */
      if(!Tasks[x].period) {
        Tasks[x].func = 0;
      }
    }
  }
}

void Setup_Interrupts() {
  // setup timer interrupts
  noInterrupts();
  TCCR1A=0;
  TCCR1B=0;
  TCNT1 = 0;

  OCR1A = 62500; // compare match register 16MHz/256/2Hz
  //OCR1A = 31;  // compare match register 16MHz/256/2kHz
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts
}

void Setup_Buzzer() {
  pinMode(BUZZER_DIO, OUTPUT);
  digitalWrite(BUZZER_DIO, OFF);  
}


void Beep() {
  digitalWrite(BUZZER_DIO, ON);
  delay(20);
  digitalWrite(BUZZER_DIO, OFF);
}

void Beep2() {
  digitalWrite(BUZZER_DIO, ON);
  delay(200);
  digitalWrite(BUZZER_DIO, OFF);
}

void setup() {
  Setup_Interrupts();
  Setup_Buzzer();

  Sched_Init();
  Sched_AddT(Beep, 1, 0);
  // Sched_AddT(Beep2, 0, 4);
  // Sched_AddT(Beep, 2, 4);
  // Sched_AddT(Beep, 3, 4);
}

ISR(TIMER1_COMPA_vect){ //timer1 interrupt 
  Sched_Schedule();
}

void loop() {
  // put your main code here, to run repeatedly:
  Sched_Dispatch();
}
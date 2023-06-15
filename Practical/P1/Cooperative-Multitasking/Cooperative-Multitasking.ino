#define NT 20

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


void Hello_World() {
  Serial.println("Hello World");
}

void setup() {
  Setup_Interrupts();
  
  Sched_Init();
  Serial.begin(9600);
  Sched_AddT(Hello_World, 0, 2);
}

ISR(TIMER1_COMPA_vect){ //timer1 interrupt 
  Sched_Schedule();
}

void loop() {
  // put your main code here, to run repeatedly:
  Sched_Dispatch();
}
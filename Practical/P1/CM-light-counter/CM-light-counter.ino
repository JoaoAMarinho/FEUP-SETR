#define NT 20
#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

/* Segment byte maps for numbers 0 to 9 */
const byte SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
/* Byte maps to select digit 1 to 4 */
const byte SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

int Count; // Stores the value that will be displayed

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

void Setup_Interrupts(void) {
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

void Setup_Leds(void) {
  pinMode(LATCH_DIO,OUTPUT);
  pinMode(CLK_DIO,OUTPUT);
  pinMode(DATA_DIO,OUTPUT);
}

void WriteNumberToSegment(byte Segment, byte Value)
{
  digitalWrite(LATCH_DIO,LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
  digitalWrite(LATCH_DIO,HIGH);    
}

void Func1(void) {
  WriteNumberToSegment(0, Count);
}
void Func2(void) {
  WriteNumberToSegment(1, Count);
}
void Func3(void) {
  WriteNumberToSegment(2, Count);
}
void Func4(void) {
  WriteNumberToSegment(3, Count);
}

void setup() {
  Setup_Interrupts();
  Setup_Leds();

  Count = 0;
  
  Sched_Init();
  Sched_AddT(Func1, 0, 4);
  Sched_AddT(Func2, 1, 4);
  Sched_AddT(Func3, 2, 4);
  Sched_AddT(Func4, 3, 4);

}

ISR(TIMER1_COMPA_vect){ //timer1 interrupt 
  Count++;
  Sched_Schedule();
} 


void loop() {
  // put your main code here, to run repeatedly:
  Sched_Dispatch();
}
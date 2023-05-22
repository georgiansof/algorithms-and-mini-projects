#define DOT 1000
#define DASH 3000
#define END 0
#define LETTER_END 3000
#define SYMBOL_END 1000
#define WORD_END 7000

int morse[][7] = {
  {DOT,DASH,END}, /// A
  {DASH,DOT,DOT,DOT,END}, /// B
  {DASH,DOT,DASH,DOT,END}, /// C
  {DASH,DOT,DOT,END}, /// D
  {DOT,END}, /// E
  {DOT,DOT,DASH,DOT,END}, /// F
  {DASH,DASH,DOT,END}, /// G
  {DOT,DOT,DOT,DOT,END}, /// H
  {DASH,DOT,DASH,END}, /// K
  {DOT,DASH,DOT,DOT,END}, /// L
  {DASH,DASH,END}, /// M
  {DASH,DOT,END}, /// N
  {DASH,DASH,DASH,END}, /// O
  {DOT,DASH,DASH,DOT,END}, /// P
  {DASH,DASH,DOT,DASH,END}, /// Q
  {DOT,DASH,DOT,END}, /// R
  {DOT,DOT,DOT,END}, /// S
  {DASH,END}, /// T
  {DOT,DOT,DASH,END}, /// U
  {DOT,DOT,DOT,DASH,END}, /// V
  {DOT,DASH,DASH,END}, /// W
  {DASH,DOT,DOT,DASH,END}, /// X
  {DASH,DOT,DASH,DASH,END}, /// Y
  {DASH,DASH,DOT,DOT,END}, /// Z
  {DASH,DASH,DASH,DASH,DASH}, /// 0
  {DOT,DASH,DASH,DASH,DASH,END}, /// 1
  {DOT,DOT,DASH,DASH,DASH,END}, /// 2
  {DOT,DOT,DOT,DASH,DASH,END}, /// 3
  {DOT,DOT,DOT,DOT,DASH,END}, /// 4
  {DOT,DOT,DOT,DOT,DOT,END}, /// 5
  {DASH,DOT,DOT,DOT,DOT,END}, /// 6
  {DASH,DASH,DOT,DOT,DOT,END}, /// 7
  {DASH,DASH,DASH,DOT,DOT,END}, /// 8
  {DASH,DASH,DASH,DASH,DOT,END}, /// 9
};

int led = 13;
char *phrase = "50ana are mere";

void MorseLetter(char lett) {
  int i;
  Serial.print("LETTER: ");
  Serial.print(lett);
  Serial.print("\n");
  for(i=0; morse[lett-'a'][i]; ++i) {
    digitalWrite(led, HIGH);
    Serial.println(morse[lett-'a'][i]);
    delay(morse[lett-'a'][i]);
    digitalWrite(led, LOW);
    delay(SYMBOL_END);
  }
}

void MorseDigit(char digit) {
  int i;
  Serial.print("Digit: ");
  Serial.print(digit);
  Serial.print("\n");
  for(i=0; morse[26+digit-'0'][i]; ++i) {
    digitalWrite(led, HIGH);
    Serial.println(morse[26+digit-'0'][i]);
    delay(morse[26+digit-'0'][i]);
    digitalWrite(led, LOW);
    delay(SYMBOL_END);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  int i;
  for(i=0; phrase[i]; ++i) {
    if(phrase[i] == ' ') {
      delay(WORD_END-(LETTER_END-SYMBOL_END));
      continue;
    }
    if(phrase[i]>='0' && phrase[i]<='9')
      MorseDigit(phrase[i]);
    else
      MorseLetter(phrase[i]);
    delay(LETTER_END-SYMBOL_END);
  }
}

void loop () {
  
}
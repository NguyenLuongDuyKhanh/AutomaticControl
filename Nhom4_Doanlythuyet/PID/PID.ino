#include <DHT.h> // Gọi thư viện DHT22
#define PWM_Pin_OUTPUT 3

  const int DHTPIN = A0; //Đọc dữ liệu từ DHT22 ở chân A0 trên mạch Arduino
  const int DHTTYPE = DHT22; //Khai báo loại cảm biến đang dùng là DHT22

  unsigned long time_now = 0; // Set thời gian bây giờ, phục vụ cho việc timer

  double P_part, I_part,D_part, Out; 
  double Val_Out, Error, pre_Error = 0; 
  //double T = 0.75;        // Set thời gian lấy mẫu
  
  double Setpoint = 40;
  double Kp = 0.5 ; //0.5; 40        is ok +1 , +0.4 -0.3 ,10cm 
  double Ki = 0.0005;  //0.001; 85;
  double Kd = 0.0001; // 0.0025; 160
  double TEMP = 0;

DHT dht(DHTPIN, DHTTYPE);

void PID_FUNTION(double _Temp)
{
  //F=Kp*e + Kd*(de/dt)+Ki*§edt                
  Error = Setpoint - _Temp;
  
  P_part = Kp*Error;
  
  I_part += (Ki*Error); // Ghép deltaT(hằng số)vào thẳng KI để hệ thống đỡ tính toán
  //if(I_part > 190) I_part = 190;  // Max của PWM
  //else if(I_part < -2) I_part = -1; // 
  
  D_part = Kd*( Error - pre_Error ); // Ghép deltaT(hằng số)vào thẳng Kd để hệ thống đỡ tính toán
  
  Val_Out += (P_part + I_part + D_part);  // OUTPUT
  pre_Error = Error;

  
  if(Val_Out > 190) // Giới hạn Val_Out vì dù gì tới 190 là đèn tắt r , có tăng nữa cũng v 
  Val_Out = 190;
  else if(Val_Out < 0)  //Giới hạn Val_Out vì = 1 là đèn sáng max , nhưng 0 đèn sẽ tắt nên sẽ để 1
  Val_Out = 1;
  analogWrite(3,191-Val_Out); // Xuất xung PWM khi max = 1 , và khi min = 190
}


void setup()
{
  Serial.begin(2000000);
  dht.begin();
  //PID_FUNTION(dht.readTemperature());

    TCCR1A = 0;
    TCCR1B = 0;    
    TIMSK1 = 0;
  
    TCCR1B |= (1 << CS12); // Normal Mode ,prescaler = 256
    TIMSK1 |= (1 << TOIE1); //Set interrupt on overflow
    TCNT1 = 65099; // 2ms , 65536 - (2ms * 16000000)/256 Thời gian lấy mẫu
    sei();

}

void loop()
{
     
}


ISR(TIMER1_OVF_vect)
{
   cli();
   TEMP = dht.readTemperature();    
   Serial.print(Setpoint);
   Serial.print(",");
   Serial.println(TEMP);
   PID_FUNTION(TEMP);
   TCNT1 = 65099;//2ms , // 2ms , 65536 - (2ms * 16000000)/256 Thời gian lấy mẫu
   sei();
}

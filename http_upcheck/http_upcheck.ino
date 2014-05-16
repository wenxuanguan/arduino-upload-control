#include "Scheduler.h";

boolean flag=false;

void setup()
{
 Serial.begin(115200);
 Sch.init(); 
 Sch.addTask(upload_data,2700,500,1);  
 Sch.addTask(check_cmd,3300,2000,1); 
 Sch.start();
}
void loop()
{  
   Sch.dispatchTasks();  
}

String get_command() 
{ 
     char buf[400];
     memset(buf,0,400);
     int ret;
     ret=sprintf(buf,"GET http://api.iot.fm/get.php?uid=demo&key=c514c91e4ed341f263e458d44b3bb0a7&sensor_name=temperature");
     Serial.write(buf);      
     Serial.write(" HTTP/1.1\r\n"); 
     Serial.flush(); 
     Serial.write("Host: api.iot.fm\r\n\r\n");
     Serial.flush();     
     delay(800);
     String header=get_http_header();
     int length=parse_content_length(header);
     String body=get_http_body(length);
     return body;     
}
void upload_data()
{
  serial_clear();
  char buf[500];
  memset(buf,0,500);
  int t=20+random(-2,2);
  if(!flag){
   return ;     
  }
  sprintf(buf,"GET /upload.php?uid=demo&key=c514c91e4ed341f263e458d44b3bb0a7&sensor_name=temperature&data=%d",t);
  Serial.print(buf);
  Serial.println(" HTTP/1.1");
  Serial.println("host:api.iot.fm");
  Serial.println("connection:close");
  Serial.println();
  Serial.flush();
  delay(800);
  String header=get_http_header();
  int length=parse_content_length(header);
  String body=get_http_body(length);
}

void check_cmd()
{
  serial_clear();
  String command=get_command();
  if(command!="")
     if(command.indexOf("end")>=0){
            flag=false;
     }     
       if(command.indexOf("begin")>=0){
            flag=true;
     }
     
}









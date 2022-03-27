/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

#define FLAG 0x7E
#define A 0x03
#define C 0x03
#define BCC A^C

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;
    unsigned char buf[255];
    unsigned char bufUA[255];

    int i, sum = 0, speed = 0;
    
    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS10", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS11", argv[1])!=0) )) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS10\n");
      exit(1);
    }


  /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */


    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd <0) {perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 1;   /* blocking read until 5 chars received */



  /* 
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
    leitura do(s) pr�ximo(s) caracter(es)
  */



    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    printf("New termios structure set\n");




/**********************CODIGO DA PRIMEIRA AULA****************************

  char str[255];
  int len;

  fgets(str, sizeof(str), stdin);
  len = strlen(str);

    for (i = 0; i < len; i++) {
      buf[i] = str[i];
    }
    printf("len = %d\n", len);
    buf[len-1] = '\0';
    printf("Mensagem a enviar: %s\n", buf);
    
    res = write(fd,buf,len);   
    printf("%d bytes written\n", res);
 
    read(fd, buf, len);
     printf("Mensagem recebida: %s\n", buf);
***************************************************************************/




//**********************CODIGO DA SEGUNDA AULA****************************
buf[0]=0x49;
buf[1]=FLAG;
buf[2]=A;
buf[3]=C;
buf[4]=FLAG;
buf[5]=C;
buf[6]=FLAG;
buf[7]=A;
buf[8]=C;
buf[9]=BCC;
buf[10]=FLAG;

res = write(fd, buf, 255);

int state=0;
i=0;

while (STOP==FALSE)          /* loop for input */
{                           
  res = read(fd,buf,1);   

  switch(state)
  {
    case 0: 
      printf("im on the %d, wachout\n", state);
      if(buf[i] == FLAG)          
      {
        bufUA[state]=buf[i];   
        state = 1;
      }
      else state = 0;
      break;
    
    case 1:      
      printf("im on the %d, wachout\n", state);
      if(buf[i] == A)             
      {
        bufUA[state]=buf[i];   
        state = 2;
      }
      else if(buf[i] == FLAG)     
      {
        bufUA[0]=buf[i];       
        state = 1;
      }
      else state = 0;
      break;

    case 2:
      printf("im on the %d, wachout\n", state);
      if(buf[i] == C)             
      {
        bufUA[state]=buf[i];   
        state = 3;
      }
      else if(buf[i] == FLAG)     
      {
        bufUA[0]=buf[i];       
        state = 1;
      }
      else state = 0;    
      break;   
  
    case 3:
      printf("im on the %d, wachout\n", state);
      if(buf[i] == BCC)           
      {
        bufUA[state]=buf[i];   
        state = 4;
      }
      else if(buf[i] == FLAG)     
      {
        bufUA[0]=buf[i];      
        state = 1;
      }
      else state = 0;
      break;

    case 4:
      printf("im on the %d, wachout\n", state);
      if(buf[i] == FLAG)          
      {
        bufUA[state]=buf[i];   
        state = 5;
      }
      else state = 0;
      break;
    
    case 5:
      printf("im on the %d, wachout\n", state);
      STOP = TRUE;
      printf("%d, Success\n", i);
      break;
  }
  i++;
}


//***************************************************************************


  if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
    perror("tcsetattr");
    exit(-1);
  }

  close(fd);
  return 0;
}
/* This file was generated with WriteTarget  0.96
 * from  par_op.wt
 * See  http://exoprog.110mb.com/wt/index.html for details
 **********/
#include "sysdefs.h"

int op_b=0;
int op_e=9999999;
int op_l=8;
int op_n=0;
int op_s=0;
int op_t=0;
int op_w=26;
int op_x=0;

extern const char *prog_name, *prog_version;    
    
void help( void )
{
    printf( "%s %s\n", prog_name, prog_version );
    printf( "Usage:  %s [options] file\n"
            "Options:\n"
    "     -b N    - begin of dump after N steps\n"
"     -e N    - end dump after N steps\n"
"     -h      - show this help\n"
"     -l N    - dump lines (height/2)\n"
"     -n N    - dump initial data as numerical table\n"
"     -s      - step program\n"
"     -t N    - expand TAB to N spaces\n"
"     -w N    - dump margin (width/2)\n"
"     -x N    - exit program after N steps\n"
            "\n", prog_name );     
}
    
void parse_op( int argc, char *argv[] )
{
    int op;

    op = 0;
    while ( op >= 0 ) {
        op = getopt( argc, argv, "b:e:hl:n:st:w:x:" );
        switch ( op ) {
            case -1:
                break;
            case 'h':
                help();
                exit( 0 );
                
  case 'b':
       op_b = atoi( optarg );
       break;
  case 'e':
       op_e = atoi( optarg );
       break;
  case 'l':
       op_l = atoi( optarg );
       break;
  case 'n':
       op_n = atoi( optarg );
       break;
  case 's':
       op_s = 1;
       break;
  case 't':
       op_t = atoi( optarg );
       break;
  case 'w':
       op_w = atoi( optarg );
       break;
  case 'x':
       op_x = atoi( optarg );
       break;  
    
            default:
                exit( 1 );
        }
    }
}


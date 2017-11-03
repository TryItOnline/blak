#include "blak.h"
#include "sysdefs.h"

const char *prog_name ="blak";
const char *prog_version ="1.0";

extern void parse_op( int argc, char *argv[] );

struct cell {
    int x, y;
    char ch;
};

struct cell *data = NULL;
int  n_cells = 0;

int  cur_x = 3;
int  cur_y = 3;

int  cur_dx = 1;
int  cur_dy = 0;

void add_cell( char ch, int x, int y )
{
    struct cell *c;
    
    if ( n_cells % 512 == 0 ) {
        data = (struct cell*)realloc( data, 
                (n_cells + 512)*sizeof( data[0] ));

        if ( !data ) {
            perror( "Error: cannot add cell" );
            exit( 1 );
        }
    }

    c = data + n_cells++;
    c->x = x;
    c->y = y;
    c->ch = ch;
}

void set_pointer( char dir, int x, int y )
{
    cur_dx = cur_dy = 0;
    
    switch ( dir ) {
        case 'v':
            cur_dy = 1;
            break;
        case '^':
            cur_dy = -1;
            break;
        case '>':
            cur_dx = 1;
            break;
        case '<':
            cur_dx = -1;
            break;
    }

    cur_x = x;
    cur_y = y;
}

void load_file( const char *fname )
{
    FILE *f;
    int ch, x, y;

    f = fopen( fname, "r" );
    if ( !f ) {
        perror( fname );
        exit( 1 );
    }

    y = x = 0;
    while ( (ch = fgetc( f )) != EOF ) {
        switch ( ch ) {
            case '\r':
                break;
            case '\n':
                y++;
                x=0;
                break;
            case '\t':
                if ( op_t ) 
                    x += op_t;
                else {
                    fprintf( stderr, "error: TAB characters in input\n" );
                    exit( 1 );
                }

                break;
            case '>':
            case '<':
            case 'v':
            case '^':
                set_pointer( ch, x, y );
            case ' ':
                x++;
                break;
            default:
                add_cell( ch, x, y );
                x++;
                break;
        }
    }

    fclose( f );
}

void plot_dir( void )
{
    if ( cur_dx > 0 )
        fputc( '>', stdout );
    else if ( cur_dx < 0 ) 
        fputc( '<', stdout );
    else if ( cur_dy < 0 ) 
        fputc( '^', stdout );
    else if ( cur_dy > 0 ) 
        fputc( 'v', stdout );
    else
        fputc( '?', stdout );
}

struct cell *find_cell( int x, int y )
{
    int i;

    for ( i=0; i<n_cells; i++ )
        if ( data[i].x == x && data[i].y == y )
            return data + i;

    return NULL;
}

void do_dump( void )
{
    struct cell *c;
    int x, y, min_x, min_y, max_x, max_y;

    min_x = cur_x - op_w;
    max_x = cur_x + op_w;
    min_y = cur_y - op_l;
    max_y = cur_y + op_l;

    for ( y=min_y; y<=max_y; y++ ) {
        for ( x=min_x; x<=max_x; x++ ) {
            if ( x==cur_x && y==cur_y )
                plot_dir();
            else {
                c = find_cell( x, y );
                if ( c ) 
                    fputc( c->ch, stdout );
                else
                    fputc( ' ', stdout );
            }
        }

        fputc( '\n', stdout );
    }
}

int get_val( int x, int y )
{
    struct cell *c;

    c = find_cell( x, y );
    if ( !c ) return 0;

    return (c->ch == ' ' || c->ch == '\0') ? 0: 1;
}

void move_data( int x1, int y1, int x2, int y2 )
{
    struct cell *c;

    c = find_cell( x1, y1 );
    if ( c ) { 
        c->x = x2;
        c->y = y2;

        switch ( c->ch ) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                printf( "%c", c->ch );
                break;
            case 'N':
                printf( "\n" );
                break;
        }
    }
    else {
        fprintf( stderr, "Internal error 002" );
        abort();
    }       
}

void rotate( int d )
{
    if ( cur_dx ) {
        cur_dy = d * cur_dx;
        cur_dx = 0;
    }
    else {
        cur_dx = -d * cur_dy;
        cur_dy = 0;
    }
}

void do_step( void )
{
    int  here, ahead;
    int  behl, behr;
    
    /* coordinates from "pointer point of view" */
    int  *pp_x, *pp_y;
    int  pp_dx, pp_dy;
    int  x1, y1;
    
    if ( cur_dx > 0 ) {
        pp_x = &cur_y;
        pp_y = &cur_x;
        pp_dx = 1;
        pp_dy = 1;
    }
    else if ( cur_dy > 0 ) {
        pp_x = &cur_x;
        pp_y = &cur_y;
        pp_dx = -1;
        pp_dy = 1;
    }
    else if ( cur_dx < 0 ) {
        pp_x = &cur_y;
        pp_y = &cur_x;
        pp_dx = -1;
        pp_dy = -1;
    }
    else if ( cur_dy < 0 ) {
        pp_x = &cur_x;
        pp_y = &cur_y;
        pp_dx = 1;
        pp_dy = -1;
    }
    else {
        fprintf( stderr, "Internal error 001" );
        abort();
    }
    
    here = get_val( cur_x, cur_y );
    (*pp_y) += pp_dy;
    ahead = get_val( cur_x, cur_y );
    (*pp_y) -= 2*pp_dy;
    (*pp_x) -= pp_dx;
    behl = get_val( cur_x, cur_y );
    (*pp_x) += 2*pp_dx;
    behr = get_val( cur_x, cur_y );
    (*pp_x) -= pp_dx;
    (*pp_y) += pp_dy;

    if ( here ) {
	if ( ahead ) exit( 0 );
	x1 = cur_x;  y1 = cur_y;
        (*pp_y) += pp_dy;
	move_data( x1, y1, cur_x, cur_y );
	(*pp_y) -= 2*pp_dy;
        cur_dx *= -1;
        cur_dy *= -1; 
    }
    else {
	if ( behl && !behr ) {
            rotate( 1 );
	    (*pp_x) += pp_dx;
	}
	else if ( behr && !behl ) {
            rotate( -1 );
	    (*pp_x) -= pp_dx;
	}
	else {
	    (*pp_y) += pp_dy;
	}
    }
}

void dump_all_data( void )
{
    int i;

    for ( i=0; i<n_cells; i++ )
        printf( "%d,%d: %c\n", data[i].y, data[i].x, data[i].ch );

    printf( "%d,%d: ", cur_y, cur_x );
    plot_dir();
    printf( "\n" );
}

int main( int argc, char *argv[] )
{
    int ndx;
    
    parse_op( argc, argv );
    
    if ( optind != argc-1 ) {
        fprintf( stderr, "Bad usage, see %s -h\n", prog_name );
        exit( 1 );
    }

    load_file( argv[ optind ] );
    if ( op_n ) dump_all_data();

    while ( 1 ) {
        ndx++;

        if ( (op_b || op_e) &&  (ndx > op_b) && (ndx<=op_e) ) {
            do_dump();
        }

        if ( op_x && ndx>op_x ) break;

        do_step();
        if ( op_s ) getchar();
    }
    
    return 0;
}



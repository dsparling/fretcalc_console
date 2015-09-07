fretcalc
========

fretcalc is a simple console fret calculator for Linux.

LICENSE
=======

GNU/GPL copying rules apply, see COPYING

COMPILATION
===========

Go into fretcalc source directory and type 'make'.
It should compile without errors.

INSTALLATION
============
Currently running make will produce a single binary in the source directory. You may put it anywhere you want. 

RUNNING
=======
    $fretcalc -n <num_frets> -s <scale_length>
    
    OPTIONS: 
     Options:
        -a <fret1,fret2,...fretx) additional half frets for dulcimer - use with -d
           half frets allowed: 0,1,3,4,6 and multiples of 7 for each 
           (7,8,10,11,13,etc.) 
        -c <frets_per_octave>     custom number of frets per octave
        -d, --dulcimer            calculate for dulcimer
        -h, --help                print this help
        -n <num_frets>            number of frets
        -o, --output              write output to calc.out
        -p <num_decimal_places>   precision of calculation display (1-5, default 3)
        -s <scale_length>         scale length
        -V, --verbose             be verbose
        -v, --version             print version number
        -w, --warranty            show warranty and copyright

FILES
=====
    ./.fretcalc        Users fretcalc configuration.
        n=<num_frets> - required
        s=<scale_length> - required	
        p=<precision> - optional, default 3
        c=<frets_per_octave> - optional, default 12
        d=1 - optional (dulcimer)
        a=<fret1,fret2,...fretx> - optional (half frets)

// FretCalculator.cpp
#include "FretCalculator.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;

// constants
const string PROGRAM_NAME = "fretcalc";
const string VERSION_NUMBER = "0.17";

// vars
double dulc_frets [MAX_FRETS];
int dulc_half_frets_bool[MAX_FRETS]; 

FretCalculator::FretCalculator()
{
  // initialize object
  scale_length     = 25;
  num_frets        = 24;
  frets_per_octave = 12;
  precision        = 3;
}

FretCalculator::FretCalculator(double sl, int nf)
{
  // initialize object
  scale_length     = sl;
  num_frets        = nf;
  frets_per_octave = 12;
  precision        = 3;
}

FretCalculator::~FretCalculator()
{
}

void FretCalculator::calcFrets()
{
  int i;

  nut_to_fret[0] = 0.0;
  for (i = 1; i <= num_frets; i++)
  {
    double ii = i;
    double dist;
    dist = scale_length - (scale_length / pow(2,(ii/frets_per_octave)));
    nut_to_fret[i] = dist;
  }
}

void FretCalculator::calcDulcFrets()
{
  /* Half frets allowed: 0,1,3,4,6 and multiples of 7 (7,8,10,11,13,etc.)
     Half frets not allowed: 2,5 and multiples of 7 (9,12,etc.)

     Note Dulc_fret Guitar_fret
     --------------------------
      D     0  (7)    0  (12)
      D#    0+ (7+)   1  (13)
      E     1  (8)    2  (14)
      F     1+ (8+)   3  (15)
      F#    2  (9)    4  (16)
      G     3  (10)   5  (17)
      G#    3+ (10+)  6  (18)
      A     4  (11)   7  (19)
      A#    4+ (11+)  8  (20)
      B     5  (12)   9  (21)
      C     6  (13)   10 (22)
      C#    6+ (13+)  11 (23)
   */
  int i,j,k,dulc_half_fret_flag;
  int zero_counter;
  int one_counter;
  int three_counter;
  int four_counter;
  int six_counter;

  dulc_half_fret_flag = 0; /* calculate a "half" fret */
  zero_counter = 0;
  one_counter = 1;
  three_counter = 3;
  four_counter = 4;
  six_counter = 6;

  nut_to_fret[0] = 0.0;
  for (i = 1, j = 1; j <= num_frets; i++)
  {
    if( (i-1) % 12 == 0 )       /* Dulc(j): 0+,7+,14+ - Guitar(i): 1,13,25 */
    {
      if( dulc_half_frets_bool[zero_counter] )
      {
        k = zero_counter;
        zero_counter += 7;
        dulc_half_fret_flag = 1;
      }
      else
      {
        zero_counter += 7;
        continue;
      }
    }
    else if( (i-3) % 12 == 0 )  /* Dulc(j): 1+,8+,15+ - Guitar(i): 3,15,27 */
    {
      if( dulc_half_frets_bool[one_counter] )
      {
        k = one_counter;
        one_counter += 7;
        dulc_half_fret_flag = 1;
      }
      else
      {
        one_counter += 7;
        continue;
      }
    }
    else if( (i-4) % 12 == 0 )  /* Dulc(j): 2,9,16 - Guitar(i): 4,16,28 */
    {
      k = i - 2;
      if( k >= 0 ) // >= 2?
      {
        dulc_half_frets_bool[k] = 0;
        dulc_half_fret_flag = 0;
      }
    }
    else if( (i-6) % 12 == 0 )  /* Dulc(j): 3+,10+,17+ - Guitar(i): 6,18,30 */
    {
      if( dulc_half_frets_bool[three_counter] )
      {
        k = three_counter;
        three_counter += 7;
        dulc_half_fret_flag = 1;
      }
      else
      {
        three_counter += 7;
        continue;
      }
    } else if( (i-8) % 12 == 0 )  /* Dulc(j): 4+,11+,18+ - Guitar(i): 8,20,32 */    {
      if( dulc_half_frets_bool[four_counter] )
      {
        k = four_counter;
        four_counter += 7;
        dulc_half_fret_flag = 1;
      }
      else
      {
        four_counter += 7;
        continue;
      }
    }
    else if( (i-9) % 12 == 0 )  /* Dulc(j): 5,12,19 - Guitar(i): 9,21,33 */
    {
      k = i - 4;
      if( k >= 0 ) // >= 4?
      {
        dulc_half_frets_bool[k] = 0;
        dulc_half_fret_flag = 0;
      }
    }
    else if( (i-11) % 12 == 0 ) /* Dulc(j): 6+,13+,25+ - Guitar(i): 11,23,35 */
    {
      if( dulc_half_frets_bool[six_counter] )
      {
        k = six_counter;
        six_counter += 7;
        dulc_half_fret_flag = 1;
      }
      else
      {
        six_counter += 7;
        continue;
      }
    }

    double ii = i;
    double dist;
    dist = scale_length - (scale_length / pow(2,(ii/12)));
    if( dulc_half_fret_flag )
      dulc_frets[k] = dist;
    else
      nut_to_fret[j] = dist;

    if( dulc_half_fret_flag )
      dulc_half_fret_flag = 0;
    else
      j++;
  }
}

void FretCalculator::printFrets()
{
  int i;

  cout << "Number of Frets: "  << num_frets << endl;
  cout << "Scale Length: "     << scale_length << endl;
  cout << "Frets Per Octave: " << frets_per_octave << endl;

  // start from 0 since there may be a ducl_half_frets[0]
  for (i = 0; i <= num_frets; i++)
  {
    double dist;
    dist = nut_to_fret[i];


    // now setprecision() will set number of decimal places
    cout.setf(ios::fixed);

    /* print regular frets first */
    if( i > 0 )
    {
      if( precision == 1 )
        cout << "Fret " << i << ": " << setprecision(1) << dist << endl;
      else if (precision == 2 )
        cout << "Fret " << i << ": " << setprecision(2) << dist << endl;
      else if (precision == 3 )
        cout << "Fret " << i << ": " << setprecision(3) << dist << endl;
      else if (precision == 4 )
        cout << "Fret " << i << ": " << setprecision(4) << dist << endl;
      else if (precision == 5 )
        cout << "Fret " << i << ": " << setprecision(5) << dist << endl;
      else
        cout << "Fret " << i << ": " << setprecision(3) << dist << endl;
    }

    // print dulcimer frets second
    if( dulc_half_frets_bool[i] )
    {
      dist = dulc_frets[i];
    
      if( precision == 1 )
        cout << "Fret " << i << "+: " << setprecision(1) << dist << endl;
      else if (precision == 2 )
        cout << "Fret " << i << "+: " << setprecision(2) << dist << endl;
      else if (precision == 3 )
        cout << "Fret " << i << "+: " << setprecision(3) << dist << endl;
      else if (precision == 4 )
        cout << "Fret " << i << "+: " << setprecision(4) << dist << endl;
      else if (precision == 5 )
        cout << "Fret " << i << "+: " << setprecision(5) << dist << endl;
      else
        cout << "Fret " << i << "+: " << setprecision(3) << dist << endl;
      }
  }
}

void FretCalculator::writeFrets()
{
  int i;

  ofstream outfile("calc.out", ios::out);
  if(! outfile)
  {
    cout << "Error opening output file" << endl;
    return;
  }

  // file opened successfully
  cout << "File calc.out created" << endl;

  outfile << "Number of Frets: "  << num_frets << endl;
  outfile << "Scale Length: "     << scale_length << endl;
  outfile << "Frets Per Octave: " << frets_per_octave << endl;

  // now setprecision() will set number of decimal places
  outfile.setf(ios::fixed);


  // start from 0 since there may be a ducl_half_frets[0]
  for (i = 0; i <= num_frets; i++)
  {
    double dist;
    dist = nut_to_fret[i];

    // print regular frets first
    if( i > 0 )
    {
      if( precision == 1 )
        outfile << "Fret " << i << ": " << setprecision(1) << dist << endl;
      else if (precision == 2 )
        outfile << "Fret " << i << ": " << setprecision(2) << dist << endl;
      else if (precision == 3 )
        outfile << "Fret " << i << ": " << setprecision(3) << dist << endl;
      else if (precision == 4 )
        outfile << "Fret " << i << ": " << setprecision(4) << dist << endl;
      else if (precision == 5 )
        outfile << "Fret " << i << ": " << setprecision(5) << dist << endl;
      else
        outfile << "Fret " << i << ": " << setprecision(3) << dist << endl;
    }

    // print dulcimer frets second
    if( dulc_half_frets_bool[i] )
    {
      dist = dulc_frets[i];

      if( precision == 1 )
        outfile << "Fret: " << i << "+: " << setprecision(1) << dist << endl;
      else if (precision == 2 )
        outfile << "Fret: " << i << "+: " << setprecision(2) << dist << endl;
      else if (precision == 3 )
        outfile << "Fret: " << i << "+: " << setprecision(3) << dist << endl;
      else if (precision == 4 )
        outfile << "Fret: " << i << "+: " << setprecision(4) << dist << endl;
      else if (precision == 5 )
        outfile << "Fret: " << i << "+: " << setprecision(5) << dist << endl;
      else
        outfile << "Fret: " << i << "+: " << setprecision(3) << dist << endl;
      }
  }

  outfile.close();

}

void FretCalculator::setScaleLength(double sl)
{
  scale_length = sl;
}

void FretCalculator::setNumFrets(int nf)
{
  num_frets = nf;
}

void FretCalculator::setFretsPerOctave(int fpo)
{
  if( fpo <= 0 )
    fpo = 12;

  frets_per_octave = fpo;
}

void FretCalculator::setPrecision(int prec)
{
  if( prec <= 0 || prec > 5 )
    prec = 3;

  precision = prec;
}

void FretCalculator::setDulcHalfFrets(const vector<int> &vec)
{
  std::vector<int>::const_iterator p1;
  for( p1 = vec.begin(); p1 != vec.end(); p1++ )
  {
    //cout << *p1 << ' ';
    dulc_half_frets_bool[*p1] = 1;
  }
}

double FretCalculator::getScaleLength()
{
  return scale_length;
}

int FretCalculator::getNumFrets()
{
  return num_frets;
}

int FretCalculator::getFretsPerOctave()
{
  return frets_per_octave;
}

int FretCalculator::getPrecision()
{
  return precision;
}

void FretCalculator::printHelp()
{
  cout << "Usage: fretcalc -n <num_frets> -s <scale_length> [OPTIONS]" << endl;
  cout << "e.g: fretcalc -n 24 -s 25.5" << endl;
  cout << "e.g: fretcalc -d -a 6 -n 24 -s 25.5" << endl;
  cout << endl;
  cout << "Options:" << endl;
  cout << "  -a <fret1,fret2,...fretx) additional half frets for dulcimer - use with -d" << endl;
  cout << "     half frets allowed: 0,1,3,4,6 and multiples of 7 for each (7,8,10,11,13,etc.)" << endl;
  cout << "  -c <frets_per_octave>     custom number of frets per octave" << endl;
  cout << "  -d, --dulcimer            calculate for dulcimer" << endl;
  cout << "  -h, --help                print this help" << endl;
  cout << "  -n <num_frets>            number of frets" << endl;
  cout << "  -o, --output              write output to calc.out" << endl;
  cout << "  -p <num_decimal_places>   precision of calculation display (1-5, default 3)" << endl;
  cout << "  -s <scale_length>         scale length" << endl;
  cout << "  -V, --verbose             be verbose" << endl;
  cout << "  -v, --version             print version number" << endl;
  cout << "  -w, --warranty            show warranty and copyright" << endl;
}

void FretCalculator::printVersion()
{
  cout << PROGRAM_NAME << " " << VERSION_NUMBER << endl;
}

void FretCalculator::printWarranty()
{
  cout << "  Copyright (c) 1996--2015 by" << endl;
  cout << "  Douglas S Sparling <doug@dougsparling.com>" << endl;
  cout << endl;
  cout << "    This program is free software; you can redistribute it and/or" << endl;
  cout << "modify it under the terms of the GNU General Public License version 2" << endl;
  cout << "as published by the Free Software Foundation." << endl;
  cout << endl;
  cout << "    This program is distributed in the hope that it will be useful," << endl;
  cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl;
  cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU" << endl;
  cout << "General Public License for more details." << endl;
  cout << endl;
  cout << "    You should have received a copy (refer to the file COPYING) of the" <<  endl;
  cout << "GNU General Public License along with this program; if not, write to" << endl;
 cout << "the Free Software Foundation, Inc., 59 Temple Place - Suite 330," << endl;
  cout << "Boston, MA 02111-1307, USA." << endl;
  cout << endl << endl;
  cout << "Report bugs to doug.sparling@gmail.com." << endl;
}

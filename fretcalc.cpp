// fretcalc.cpp
#include "FretCalculator.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
using namespace std;

// vars
vector <int> dulc_half_frets(0);

// function delcarations
void param_init(int, char *[]);

// variable declarations
char * endptr;
double scale_length;
int num_frets;
int frets_per_octave;
int precision;
bool print_help     = false;
bool print_version  = false;
bool print_warranty = false;
bool verbose        = false;
bool is_dulcimer    = false;
bool out_file       = false;
bool s_found        = false;
bool n_found        = false;

int main (int argc, char **argv)
{

  // check for parameters
  param_init(argc,argv);

  // create object
  FretCalculator FC;

  // check for message flags
  if(print_help)
  {
    FC.printHelp();
    exit(0);
  } 
  else if(print_warranty)
  {
    FC.printVersion();
    FC.printWarranty();
    exit(0);
  }
  else if(print_version)
  {
    FC.printVersion();
    exit(0);
  }

  // Calculate if num_frets and scale_length
  // Remove this if statement to use
  // num_frets and scale_length defaults
  if(!n_found && !s_found)
  {
    // check for .fretcalc file 
    ifstream inFile(".fretcalc");
    if(inFile)
    {
      while(! inFile.eof())
      {
        string input_str;
        string flag_name;
        string flag_value;
        unsigned int pos;

        getline(inFile,input_str);
        pos = input_str.find("=");
        if(pos != string::npos)
        {
          flag_name.assign(input_str,pos-1,1);  // assumes 1 character flagname
          flag_value.assign(input_str,pos+1,input_str.size()-pos-1); 
          if( flag_name.compare("n") == 0 )
          {
            //cout << "n flag found (" << flag_value << ")" << endl;
            n_found = true;
            num_frets = atoi(flag_value.c_str());
          }
          if( flag_name.compare("s") == 0 )
          {
            //cout << "s flag found (" << flag_value << ")" << endl;
            s_found = true;
            scale_length = atof(flag_value.c_str());
          }
          if( flag_name.compare("p") == 0 )
          {
            //cout << "p flag found (" << flag_value << ")" << endl;
            precision = atoi(flag_value.c_str());
          }
          if( flag_name.compare("c") == 0 )
          {
            //cout << "c flag found (" << flag_value << ")" << endl;
            frets_per_octave = atoi(flag_value.c_str());
          }
          if( flag_name.compare("d") == 0 )
          {
            //cout << "d flag found (" << flag_value << ")" << endl;
            is_dulcimer = true;
          }
          if( flag_name.compare("a") == 0 )
          {
            // split on commas
            char *pch;       // pointer for tokens
            const char *ptr1 = 0;
            int len = flag_value.length();
            char *ptr2 = new char[len + 1]; // including null
            // copy characters out of sting into allocated memory
            flag_value.copy(ptr2,len,0);
            ptr2[len] = 0; // add null terminator
            pch = strtok(ptr2,",");
            while (pch != NULL)
            {
              int half_fret = atoi(pch);
              if( half_fret >= 0 )
              {
                dulc_half_frets.push_back(half_fret);
                pch = strtok(NULL,",");
              }
            }
          }
        }
      }
      inFile.close();
    } 
  }

  if(n_found && s_found )
  {

    // set all params
    FC.setScaleLength(scale_length);
    FC.setNumFrets(num_frets);
    FC.setFretsPerOctave(frets_per_octave);
    FC.setPrecision(precision);
    FC.setDulcHalfFrets(dulc_half_frets);

    // do fret calculation
    if( is_dulcimer )
      FC.calcDulcFrets();
    else
      FC.calcFrets();

    if(out_file)
    {
     FC.writeFrets();
      if(verbose)
        FC.printFrets();
    } 
    else
    {
      FC.printFrets();
    }
  
  } else {
    FC.printHelp();
  }

  return 0;
}

void param_init(int argc,char *argv[])
{
  // first, get the parameters
  char *str;             // string pointer
  const char *sub = "-"; // substring pointer
  char *ptr;             // pointer for substring address
  char *pch;             // pointer for tokens
  int pos;               // element position for found substring

  for (int i = 1; i < argc; i++)
  {
    // additional half frets for dulcimer
    if (strcmp(argv[i], "-a") == 0)
    {
      if (i + 1 < argc)
      {
        // make sure "i+1" isn't another argument flag
        str = argv[i+1];
        ptr = strstr(str,sub);
        pos = (ptr - str);
        if( pos == 0 ) {
          continue;
        }

        // split on commas
        pch = strtok(argv[++i],",");
        while (pch != NULL)
        {
          int half_fret = atoi(pch);
          if( half_fret >= 0 )
          {
            dulc_half_frets.push_back(half_fret);
            pch = strtok(NULL,",");
          }
        }
      } else {
        // default
      }
    }

    // custom (custom) non-12 tone 
    if (strcmp(argv[i], "-c") == 0)
    {
      if (i + 1 < argc)
      {
        // make sure "i+1" isn't another argument flag
        str = argv[i+1];
        ptr = strstr(str,sub);
        pos = (ptr - str);
        if( pos != 0 )
        {
          frets_per_octave = atoi(argv[++i]);
        }
      }
    }

    // dulcimer
    if ((strcmp(argv[i], "-d") == 0) ||
        (strcmp(argv[i], "--d") == 0) ||
        (strcmp(argv[i], "-dulcimer") == 0) ||
        (strcmp(argv[i], "--dulcimer") == 0)) {
      is_dulcimer = true;
    }

    // help
    if ((strcmp(argv[i], "-h") == 0) ||
        (strcmp(argv[i], "--h") == 0) ||
        (strcmp(argv[i], "-help") == 0) ||
        (strcmp(argv[i], "--help") == 0)) {
      print_help = true;
    }

    // write to output file 'calc.out' 
    if ((strcmp(argv[i], "-o") == 0) ||
        (strcmp(argv[i], "--o") == 0) ||
        (strcmp(argv[i], "-output") == 0) ||
        (strcmp(argv[i], "--output") == 0)) {
      out_file = true;
    }

    // precision (precision)
    if (strcmp(argv[i], "-p") == 0)
    {
      if (i + 1 < argc)
      {
        // make sure "i+1" isn't another argument flag
        str = argv[i+1];
        ptr = strstr(str,sub);
        pos = (ptr - str);
        if( pos != 0 ) {
          precision = atoi(argv[++i]);
        }
      }
    }

    // verbose
    if ((strcmp(argv[i], "-V") == 0) ||
        (strcmp(argv[i], "--V") == 0) ||
        (strcmp(argv[i], "-verbose") == 0) ||
        (strcmp(argv[i], "--verbose") == 0)) {
      verbose = true;
    }

    // version and credits
    if ((strcmp(argv[i], "-v") == 0) ||
        (strcmp(argv[i], "--v") == 0) ||
        (strcmp(argv[i], "-version") == 0) ||
        (strcmp(argv[i], "--version") == 0)) {
      print_version = true;
    }

    // warranty
    if ((strcmp(argv[i], "-w") == 0) ||
        (strcmp(argv[i], "--w") == 0) ||
        (strcmp(argv[i], "-warranty") == 0) ||
        (strcmp(argv[i], "--warranty") == 0)) {
      print_version = true;
      print_warranty = true;
    }

    // number of frets (num_frets_val)
    if (strcmp(argv[i], "-n") == 0)
    {
      if (i + 1 < argc)
      {
        // make sure "i+1" isn't another argument flag
        str = argv[i+1];
        ptr = strstr(str,sub);
        pos = (ptr - str);
        if( pos != 0 )
        {
          n_found = true;
          num_frets = atoi(argv[++i]);
        }
      }
    }

    // scale length (scale_length) 
    if (strcmp(argv[i], "-s") == 0)
    {
      if (i + 1 < argc)
      {
        // make sure "i+1" isn't another argument flag
        str = argv[i+1];
        ptr = strstr(str,sub);
        pos = (ptr - str);
        if( pos != 0 )
        {
          s_found = true;
          scale_length = strtod(argv[++i],&endptr);
        }
      }
    }

  }
}

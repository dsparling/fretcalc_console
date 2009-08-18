// FretCalculator.h
#ifndef FRETCALCULATOR_H
#define FRETCALCULATOR_H
#include <vector>
const int MAX_FRETS = 100;

class FretCalculator
{
  public:
  // default constructor
  FretCalculator();
  FretCalculator(double, int);
  // destructor
  ~FretCalculator();
  // methods
  // frets
  void calcFrets();
  void calcDulcFrets();
  void printFrets();
  void writeFrets();
  // mutator
  void setScaleLength(double);
  void setNumFrets(int);
  void setFretsPerOctave(int);
  void setPrecision(int);
  void setDulcHalfFrets(const std::vector<int> &vec);
  // accessor
  double getScaleLength();
  int getNumFrets();
  int getFretsPerOctave();
  int getPrecision();
  // help/version/warranty
  void printHelp();
  void printVersion();
  void printWarranty();

  // data
  private:
    double scale_length;
    int num_frets;
    double nut_to_fret [MAX_FRETS];
    int frets_per_octave;
    int precision;
};

#endif // FRETCALCULATOR_H

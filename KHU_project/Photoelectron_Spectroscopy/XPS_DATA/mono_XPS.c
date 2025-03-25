#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char* path="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_DATA";
char* atoms[]={"Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi"};

int main(void){
  int i;
  FILE* fp;
  if((fp=popen("../SESSA.MAC_v2.2/Sessa\\ v2.2.app/Contents/MacOS/SESSA\\ v2.2 -c", "w"))==NULL){
    perror("popen");exit(1);
  }
  for(i=0; i<=81; i++){
    fprintf(fp, "PROJECT RESET\n"); fflush(fp);
    fprintf(fp, "SAMPLE SET MATERIAL /%s/ LAYER 1 \n", atoms[i]); fflush(fp);
    fprintf(fp, "MODEL SET NCOL 10 REGION 1 \n"); fflush(fp);
    fprintf(fp, "MODEL SIMULATE \n"); fflush(fp);
    fprintf(fp, "MODEL SAVE SPECTRA \"%s/Li_Bi/%s_\" \n", path, atoms[i]); fflush(fp);
  }
  pclose(fp);
  return 0;
}

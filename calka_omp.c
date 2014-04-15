#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

double timeDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  double diff = (((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
                 ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec));
  return diff / 1000000000;
}

double fun(double x){
  return pow(M_E, (-1) * pow(x,2) );
}

double quadrates(double a, double b, double size, double (*f)(double)){
  double h = ( b - a ) / size;
  double x = a;
  double wynik = 0;
  int n = (int) size;

  int i;

  #pragma omp parallel for private(i), reduction(+:wynik)
  for(i = 0; i < n; i++){
    wynik += ( ( f( x + (i * h) ) + f( x + ((i+1)*h) ) ) / 2 ) * h;
  }

  return wynik;
}

double trapezoidal(double a, double b, double size, double (*f)(double)){
  double h = (b - a) / size;

  double x = a;
  double wynik = 0;
  int n = (int) size;

  int i;
#pragma omp parallel for private(i), reduction(+:wynik)
  for(i =0; i < n; i++){
    wynik += f(x + i*h) + f(x + (i+1)*h);
  }

  wynik *= h/2;

  return wynik;
}

double simpsons(double a, double b, double size, double (*f)(double)){
  double h = (b-a) / (2*size);
  double x = a;
  double wynik = 0;
  int n = (int) size;

  int i;
#pragma omp parallel for private(i), reduction(+:wynik)
  for(i = 0; i <= 2*n; i++){
    if(i ==0 || i==2*n){
      wynik += f( x + (i * h) );
    }
    else if(i%2 != 0){
      wynik += 4 * f( x + (i * h) );
    }
    else if(i%2 == 0){
      wynik += 2 * f( x + (i * h) );
    }
  }

  wynik *= h / 3;

  return wynik;
}


int main(int argc, char **argv){

  struct timespec start_q, end_q, start_t, end_t, start_s, end_s;
  FILE *file1;
  file1 = fopen("results.out", "a");
  double a = -2, b = 2;
  double size = (double) atoi( argv[1] );
  
  clock_gettime(CLOCK_MONOTONIC, &start_q);
  double integral_quadrates = quadrates(a,b,size, fun);
  clock_gettime(CLOCK_MONOTONIC, &end_q);
  fprintf(file1,"quadrates   : %.32lf\n", integral_quadrates);
  
  clock_gettime(CLOCK_MONOTONIC, &start_t);
  double itegral_trapezoidal = trapezoidal(a,b, size, fun);
  clock_gettime(CLOCK_MONOTONIC, &end_t);
  fprintf(file1,"trapezoidal : %.32lf\n", itegral_trapezoidal);
  
  clock_gettime(CLOCK_MONOTONIC, &start_s);
  double itegral_simpsons = simpsons(a,b, size, fun);
  clock_gettime(CLOCK_MONOTONIC, &end_s);
  fprintf(file1,"simpsons    : %.32lf\n\n", itegral_simpsons);

  fprintf(file1,"Czas quadrates   : %.16f\n", timeDiff(&end_q, &start_q));
  fprintf(file1,"Czas trapezoidal : %.16f\n", timeDiff(&end_t, &start_t));
  fprintf(file1,"Czas simpsons    : %.16f\n", timeDiff(&end_s, &start_s));
  fclose(file1);
  return 0;
}

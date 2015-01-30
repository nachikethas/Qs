#include "part1.h"

#define NUMARV 100000

int runSim( double lambda, double mu, unsigned long numArv){

  MMKNQ q1 (lambda, mu);

  srand48 (unsigned(time(NULL)));

  while (true){

    if ( q1.getArvTime() > q1.getDepTime() ){
      // We have a departure
      q1.cDep();
      //      cout<<"Dept! :"<<q1.getNumArv()<<endl;
    }
    else{
      // We have an arrival
      q1.cArv();
      //      cout<<"Arrival! :"<<q1.getNumArv()<<endl;
    }
    
    if ( q1.getNumArv() > numArv)
      break;
    
  }//  while (true){

  double *sD;
  double sDcp[MAX_DIST];
  sD = q1.getStatDist();
  for (int i=0; i<MAX_DIST; i++)
    sDcp[i] = sD[i];

  cout<<"Utilization: "<<q1.avgUtil()<<endl;
  cout<<"Average number of packets in the system: "<<q1.avgSize()<<endl;
  cout<<"Average delay in the system: "<<q1.avgDelay()<<endl;
  cout<<"Blocking ratio: "<<q1.blockProb()<<endl;
  cout<<"Idle period of the server: "<<q1.idleTime()<<endl;
  
  // Optional outputs
  /*
  cout<<"Average service time: "<<q1.avgSTime()<<endl;
  cout<<"Average second moment of service time: "<<q1.eST2()<<endl;
  cout<<"Average wait time in queue: "<<q1.waitQ()<<endl;
  */

  printf("Stationary distribution: 0->%f, 1->%f, 2->%f, 3->%f, 4->%f .....\n", sDcp[0], sDcp[1], sDcp[2], sDcp[3], sDcp[4]);
  
  return 0;
}// int main(){

int runSim( double lambda, double mu, unsigned long numArv, double p){

  MMKNQ q1 (lambda, mu, p);

  srand48 (unsigned(time(NULL)));

  while (true){

    if ( q1.getArvTime() > q1.getDepTime() ){
      // We have a departure
      q1.cDep();
      //      cout<<"Dept! :"<<q1.getNumArv()<<endl;
    }
    else{
      // We have an arrival
      q1.cArv();
      //      cout<<"Arrival! :"<<q1.getNumArv()<<endl;
    }
    
    if ( q1.getNumArv() > numArv)
      break;
    
  }//  while (true){

  double *sD;
  double sDcp[MAX_DIST];

  cout<<"Utilization: "<<q1.avgUtil()<<endl;
  cout<<"Average number of packets in the system: "<<q1.avgSize()<<endl;
  cout<<"Average delay in the system: "<<q1.avgDelay()<<endl;
  cout<<"Blocking ratio: "<<q1.blockProb()<<endl;
  cout<<"Idle period of the server: "<<q1.idleTime()<<endl;
  sD = q1.getStatDist();

  for (int i=0; i<MAX_DIST; i++)
    sDcp[i] = sD[i];

  printf("Stationary distribution: 0->%f, 1->%f, 2->%f, 3->%f, 4->%f.....\n", sDcp[0], sDcp[1], sDcp[2], sDcp[3], sDcp[4]);

  return 0;
}// int main(){

int runSim( double lambda, double mu, unsigned long numArv, int K, int N){

  MMKNQ q1 (lambda, mu, K, N);

  srand48 (unsigned(time(NULL)));

  while (true){

    if ( q1.getArvTime() > q1.getDepTime() ){
      // We have a departure
      q1.cDep();
      //      cout<<"Dept! :"<<q1.getNumArv()<<endl;
    }
    else{
      // We have an arrival
      q1.cArv();
      //      cout<<"Arrival! :"<<q1.getNumArv()<<endl;
    }
    
    if ( q1.getNumArv() > numArv)
      break;
    
  }//  while (true){

  double *sD;
  double sDcp[MAX_DIST];
  cout<<"Utilization: "<<q1.avgUtil()<<endl;
  cout<<"Average number of packets in the system: "<<q1.avgSize()<<endl;
  cout<<"Average delay in the system: "<<q1.avgDelay()<<endl;
  cout<<"Blocking ratio: "<<q1.blockProb()<<endl;
  cout<<"Idle period of the server: "<<q1.idleTime()<<endl;
  sD = q1.getStatDist();

  for (int i=0; i<MAX_DIST; i++)
    sDcp[i] = sD[i];

  printf("Stationary distribution: 0->%f, 1->%f, 2->%f, 3->%f, 4->%f .....\n", sDcp[0], sDcp[1], sDcp[2], sDcp[3], sDcp[4]);

  return 0;
}// int main(){


int main (int argc, char* argv[]){

  if (argc != 2){
    cout<<"Invalid number of options entered! Terminating program....\n";
    exit(1);
  }

  int opt = atoi (argv[1]);

  switch (opt){

  case 1: runSim (0.7, 1.0, 5000); break;
  case 2: runSim (0.95, 1.0, 5000); break;
  case 3: runSim (0.75, 1.0, 10000, 3, 6); break;
  case 4: runSim (0.8, 1.0, 3000000, 0.05); break;
  case 5: runSim (0.3, 1.0, 3000000, 0.4); break;
  case 6: runSim (0.7, 1.0, 1000000); break;
  case 7: runSim (0.95, 1.0, 5000000); break;
  case 8: runSim (0.75, 1.0, 1000000, 3, 6); break;

  default: cout<<"Invalid option!! Terminating program....\n"; break;
    
  }//   switch (opt){

  return 0;
}// int main (int opt){

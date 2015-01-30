#include "part2.h"

#define NUMARV 1000000

int main(int argc, char* argv[]){

  if (argc != 3){
    cout<<"Invalid number of options entered! Terminating program....\n";
    exit(1);
  }

  unsigned long numArv = atoi(argv[1]);
  unsigned long seed = atoi(argv[2]);

  //  cout<<"The seed is: "<<seed<<endl;
  //  cout<<"The numArv is: "<<numArv<<endl;

  MG1Q q1 ( 0.2, 1.00);

  // seed = unsigned(time(NULL));
  srand48 (seed);

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

  cout<<"Mean number of packets in the system: "<<q1.avgSize()<<endl;
  cout<<"Average delay per customer: "<<q1.avgDelay()<<endl;  cout<<"Average service time: "<<q1.avgSTime()<<endl;
  //  Uncomment the line below to display E[S^2]
  //  cout<<"Second moment of ST: "<<q1.eST2()<<endl;
  cout<<"Average waiting time: "<<q1.waitQ()<<endl;
  cout<<"Idle period of server: "<<q1.idleTime()<<endl;
  cout<<"Blocking ratio: "<<q1.blockProb()<<endl;
  //  cout<<"The seed is: "<<seed<<endl;

  return 0;
}// int main(){

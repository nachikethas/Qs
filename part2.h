#include <iostream>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <cmath>
#include <climits>
#include <sstream>

using namespace std;

#define ARV 1
#define DEP 0
#define MAX_DIST 10000

class MG1Q{

private:

  // ---- The following make up the core of the queue --------------
  bool busy;                    // The state of the server
  double arvTime;               // Represents the arrival time of the customer currently in service.
  double depTime;               // Represents the departure time of the customer currently in service
  double nextArv;               // The time of the next new arrival
  double lambda;                // The arrival rate
  double mu;                    // The departure rate
  queue<double> buffer;         // Holds the arrival times of the customers waiting for sevice

  // -------------- Bookkeeping ---------------------

  unsigned long numArrivals;    // Total num of arrivals since the start of the simulation
  double totalDelay;            // Sum of the delays of each customer
  unsigned long totalPkts;      // Sum of number of customers in the system as seen at arrival times
  unsigned long arvBlocked;     // Total number of arrivals blocked over the entire run
  int P[MAX_DIST];              // Limiting probability that there are 'i' customers in the system;
  unsigned long idle;           // The number of times the server is idle
  double clock;

  double sT;                    // Service time of server
  double sT2;                   // Second moment of service time.

  double expRV(double rate);
  double bPareto();

public:
  
  MG1Q(double, double);

  double getArvTime() {return nextArv;}       // Returns the arrival time of the next Customer
  double getDepTime() {return depTime;}       // Returns the departure time of the currently servicing customer
  unsigned long getNumArv() {return numArrivals;}

  double avgUtil();
  double avgSize();
  double avgDelay();
  double blockProb();
  double idleTime();

  double avgSTime();                         // The average service time.
  double eST2();                             // Second moment of the service time. 
  double waitQ()                             // Average waiting time in queue.
  {return avgDelay() - avgSTime();}

  int cArv();
  int cDep();
  
};

double MG1Q :: avgSTime(){
  return sT/double(numArrivals);
}// int MG1Q :: avgUtil(){

double MG1Q :: eST2(){
  return sT2/double(numArrivals);
}// int MG1Q :: avgUtil(){

MG1Q :: MG1Q (double arvRate, double depRate) {
  
  // Initialization of all variables
  numArrivals = 0;
  totalDelay = 0.0;
  totalPkts = 0;
  arvBlocked = 0;
  sT = 0.0;
  sT2 = 0.0;

  idle = 0;
  lambda = arvRate;
  mu = depRate;
  fill_n (P, MAX_DIST, 0);
  
  // An arrival occurs first.
  // Sending him to the server and updating state and statistics
  nextArv = expRV(lambda);

  //cout<<"expRV(l):"<<nextArv<<endl;

  clock = nextArv;
  numArrivals++;
  P[0]++;
  idle++;
  busy = true;
  arvTime = nextArv;

  double temp = bPareto();
  sT += temp;
  sT2 += temp*temp;

  depTime = arvTime + temp;                     // The departure time of this customer

  //cout<<"bPareto():"<<depTime-arvTime<<endl;

  totalDelay += depTime - arvTime;

  nextArv = clock + expRV(lambda);
}

double MG1Q :: avgUtil(){
  return 1-((double)idle/numArrivals);
}// int MG1Q :: avgUtil(){

double MG1Q :: avgSize(){
  return ((double)totalPkts/numArrivals);
}// int MG1Q :: avgUtil(){

double MG1Q :: avgDelay(){
  return (double)totalDelay/numArrivals;
}// int MG1Q :: avgUtil(){

double MG1Q :: blockProb(){
  return (double)arvBlocked/numArrivals;
}// int MG1Q :: avgUtil(){

double MG1Q :: idleTime(){
  return (double)idle/numArrivals;
}// int MG1Q :: avgUtil(){

double MG1Q :: expRV (double rate) {
  return (double) -(log(drand48())/rate);
}// unsigned long MG1Q :: expRV (int rate) {

double MG1Q :: bPareto(){

  double m = 1.0;
  double M = pow(10.0, 4.0);
  double alpha = 1.4;

  return m/pow(1.0-(1.0-pow(m/M, alpha))*drand48(), 1.0/alpha);
}

int MG1Q :: cDep(){

  clock = depTime;

  if (buffer.empty()){
    // There is no one waiting in the queue
    // Set the server as available and exit
    busy = false;
    depTime = LONG_MAX;                      // This will force an arrival next time.
    //cout<<"bPareto() Gen:"<<bPareto()<<endl;
    return 0;
  } 
  
  // There is someone in the wait queue
  // Make the first customer in queue the one being serviced
  // Reduce the queue size by 1
  arvTime = buffer.front();
  buffer.pop();
  
  // Generate the departure time for the customer being serviced
  double temp = bPareto();
  sT += temp;
  sT2 += temp*temp;

  depTime = clock + temp;

  //cout<<"bPareto():"<<depTime-clock<<endl;

  totalDelay += depTime - arvTime;

  return 1;
  
}// int MMknQ :: cDep(){

int MG1Q :: cArv(){

  clock = nextArv;

  // The statistics are updated just before this arrival enters.
  numArrivals++;
    
  if(busy){
    // The server is busy
    int inSys = buffer.size() + 1;
    totalPkts += inSys;
    P[inSys]++;

    // Push the new arrival to the end of the queue
    buffer.push(nextArv);

  }// if(busy){
  else{
    // The customer goes directly into service.
    // The buffer size should be zero here
    
    if (!buffer.empty()){
      cout<<"Incorrect buffer size detected! Will terminate ....\n";
      exit(0);
    }

    P[0]++;
    idle++;
    busy = true;
    arvTime = nextArv;
    // The departure time of this customer
    // This is also the system depTime as no one
    // else is in the system.
    double temp = bPareto();
    sT += temp;
    sT2 += temp*temp;

    depTime = arvTime + temp;                     
    totalDelay += depTime - arvTime;
  }

  nextArv = clock + expRV(lambda);
  return 0;
}// int MG1Q :: newArv(){


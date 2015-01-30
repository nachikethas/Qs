#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <cmath>
#include <climits>

using namespace std;

#define ARV 1
#define DEP 0
#define MAX_DIST 10000

class MMKNQ{

private:

  // ---- The following make up the core of the queue --------------
  int K;                        // The number of servers
  int N;                        // The buffer size
  bool *busy;                   // The state of each server
  double *arvTime;              // Represents the arrival time of the customer currently in service at some server.
  double depTime;               // Time of next Departure
  double nextArv;               // Time of next Arrival
  double lambda;                // The arrival rate
  double mu;                    // The departure rate
  queue<double> buffer;         // Holds the arrival times of the customers waiting for sevice
  double retProb;               // Probability that the customer returns without exiting.

  double *sDepTime;             // Represents the departure time of the customer currently in service at some server
  // -------------- Bookkeeping ---------------------

  unsigned long numArrivals;    // Total num of arrivals since the start of the simulation
  unsigned long repeatArvs;     // The number of times someone has cycled back into the queue
  double totalDelay;            // Sum of the delays of each customer
  unsigned long totalPkts;      // Sum of number of customers in the system as seen at arrival times
  unsigned long arvBlocked;     // Total number of arrivals blocked over the entire run
  int P[MAX_DIST];              // Limiting probability that there are 'i' customers in the system;
  unsigned long idle;           // The number of times the server is idle
  double sD[MAX_DIST];          // The stationary distribution.
  double util;                  // Keeps track of system utilization. Useful in multiple servers case.
  double sT;                    // Service time of server
  double sT2;                   // Second moment of service time.

  double clock;
  
  double expRV(double rate);
  double updateDepTime();
  int backToQ();                // Returns the user to the queue without departure.

public:
  
  MMKNQ(double, double);
  MMKNQ(double, double, int, int);
  MMKNQ(double, double, double);

  double getArvTime() {return nextArv;}       // Returns the arrival time of the next Customer
  double getDepTime() {return depTime;}       // Returns the departure time of the currently servicing customer
  unsigned long getNumArv() {return numArrivals;}
  double* getStatDist();

  double avgUtil();
  double avgSize();                          // The average number of customers in the system.
  double avgDelay();
  double blockProb();                        
  double idleTime();                         // Average time the system is idle.

  // The three functions below return correct
  // values only for M/M/1 queues.
  double avgSTime();                         // The average service time.
  double eST2();                             // Second moment of the service time. 
  double waitQ()                             // Average waiting time in queue.
  {return avgDelay() - avgSTime();}

  int cArv();
  int cDep();
  
};

double* MMKNQ :: getStatDist(){

  for (int i=0; i<MAX_DIST; i++)
    sD[i] = double(P[i])/double(numArrivals);
    
  return sD;
}

double MMKNQ :: avgSTime(){
  return sT/double(numArrivals);
}// int MMKNQ :: avgUtil(){

double MMKNQ :: eST2(){
  return sT2/double(numArrivals);
}// int MMKNQ :: avgUtil(){

double MMKNQ :: avgUtil(){
  double temp = 1-((double)idle/numArrivals);
  return util/double(numArrivals);
}// int MMKNQ :: avgUtil(){

double MMKNQ :: avgSize(){
  return ((double)totalPkts/numArrivals);
}// int MMKNQ :: avgUtil(){

double MMKNQ :: avgDelay(){
  return (double)totalDelay/(numArrivals-arvBlocked-repeatArvs);
}// int MMKNQ :: avgUtil(){

double MMKNQ :: blockProb(){
  return (double)arvBlocked/numArrivals;
}// int MMKNQ :: avgUtil(){

double MMKNQ :: idleTime(){
  return (double)idle/numArrivals;
}// int MMKNQ :: avgUtil(){

double MMKNQ :: expRV (double rate) {
  return (double) -(log(drand48())/rate);
}// unsigned long MMKNQ :: expRV (int rate) {


int MMKNQ :: backToQ(){
  // The customer who is suppsed to depart goes back to the queue directly. 
  // The clock and the next arrival remains untouched. Essentially, the 
  // customer goes back to the queue before the next arrival.
  
  /* 
     Note that this function is essentially a reproduction of the cArv()
     with the only change being that this arrival is "in between" the 
     normal poisson arrivals. 
   */

  // clock = nextArv;

  bool foundSrv = false;
  int freeSrv = -1;
  int freeSrvCnt = 0;
  // The statistics are updated just before this arrival enters.
  numArrivals++;
  repeatArvs++;
  
  for (int i=0; i<K; i++)
    if(!busy[i]){
    
      // This server is free
      freeSrv = i;
      freeSrvCnt ++;
      foundSrv = true;
    } // if(!busy[i]){

  int inSys = buffer.size() + K - freeSrvCnt;
  totalPkts += inSys;
  P[inSys]++;    

  if (inSys == 0)
    idle++;
  else
    util += 1.0 - double(freeSrvCnt)/double(K);
  
  if (foundSrv){

    // Server 'freeSrv' is free
    // The customer goes directly into service.
    busy[freeSrv] = true;
    arvTime[freeSrv] = clock;

    double temp = expRV(mu);
    sT += temp;
    sT2 += temp*temp;

    sDepTime[freeSrv] = arvTime[freeSrv] + temp;
    depTime = updateDepTime();
    totalDelay += sDepTime[freeSrv] - arvTime[freeSrv];

  }// if (foundSrv){
  else{
    // No server is free
    // We need to check if the system is full
    if( buffer.size() == (N-K))
      // The arrival will be blocked.
      arvBlocked++;
    else if (buffer.size() < (N-K))
      // Push the new arrival to the end of the queue
      buffer.push(clock);
    else{
      // We should never be here.
      cout<<"Fatal Error! Invalid Buffer size. Terminating program....\n";
      exit(0);
    }
  }

  // nextArv = clock + expRV(lambda);
  return 0;
}// int MMKNQ :: avgUtil(){

double MMKNQ :: updateDepTime(){
  // Search through you list of servers and return the lowest departure value
  double lowest = sDepTime[0];

  for (int i=0; i<K; i++){
    if (sDepTime[i] < lowest)
      lowest = sDepTime[i];
  }
  
  depTime = lowest;
  return lowest;
}// int MMKNQ :: avgUtil(){

MMKNQ :: MMKNQ (double arvRate, double depRate, int k, int n) {

  // Initialization of all variables.
  K = k;
  N = n;

  busy = new bool[K];
  arvTime = new double[K];
  sDepTime = new double[K];

  numArrivals = 0;
  repeatArvs = 0;
  util = 0.0;
  sT = 0.0;
  sT2 = 0.0;
  totalDelay = 0.0;
  totalPkts = 0;
  arvBlocked = 0;
  idle = 0;
  lambda = arvRate;
  mu = depRate;
  retProb = 0.0;

  fill_n (P, MAX_DIST, 0);
  fill_n (busy, K, false);
  fill_n (arvTime, K, 0.0);
  fill_n (sDepTime, K, LONG_MAX);

  // Adding the first arrival
  // Sending him to server 0

  nextArv = expRV(lambda);
  clock = nextArv;
  
  numArrivals++;
  P[0]++;
  idle++;

  busy[0] = true;
  arvTime[0] = nextArv;
  
  double temp = expRV(mu);
  sT += temp;
  sT2 += temp*temp;

  sDepTime[0] = arvTime[0] + temp;
  depTime = updateDepTime();

  totalDelay += sDepTime[0] - arvTime[0];
  nextArv = clock + expRV(lambda);

} // MMKNQ :: MMKNQ (double arvRate, double depRate) {

MMKNQ :: MMKNQ (double arvRate, double depRate) {

  // Initialization of all variables.
  K = 1;
  N = LONG_MAX;

  busy = new bool[K];
  arvTime = new double[K];
  sDepTime = new double[K];

  numArrivals = 0;
  repeatArvs = 0;
  util = 0.0;
  sT = 0.0;
  sT2 = 0.0;
  totalDelay = 0.0;
  totalPkts = 0;
  arvBlocked = 0;
  idle = 0;
  lambda = arvRate;
  mu = depRate;
  retProb = 0.0;

  fill_n (P, MAX_DIST, 0);
  fill_n (busy, K, false);
  fill_n (arvTime, K, 0.0);
  fill_n (sDepTime, K, LONG_MAX);

  // Adding the first arrival
  // Sending him to server 0

  nextArv = expRV(lambda);
  clock = nextArv;
  
  numArrivals++;
  P[0]++;
  idle++;

  busy[0] = true;
  arvTime[0] = nextArv;

  double temp = expRV(mu);
  sT += temp;
  sT2 += temp*temp;

  sDepTime[0] = arvTime[0] + temp;
  depTime = updateDepTime();

  totalDelay += sDepTime[0] - arvTime[0];
  nextArv = clock + expRV(lambda);

}

MMKNQ :: MMKNQ (double arvRate, double depRate, double p) {

  // Initialization of all variables.
  K = 1;
  N = LONG_MAX;

  busy = new bool[K];
  arvTime = new double[K];
  sDepTime = new double[K];

  numArrivals = 0;
  repeatArvs = 0;
  util = 0.0;
  sT = 0.0;
  sT2 = 0.0;
  totalDelay = 0.0;
  totalPkts = 0;
  arvBlocked = 0;
  idle = 0;
  lambda = arvRate;
  mu = depRate;
  retProb = p;

  fill_n (P, MAX_DIST, 0);
  fill_n (busy, K, false);
  fill_n (arvTime, K, 0.0);
  fill_n (sDepTime, K, LONG_MAX);

  // Adding the first arrival
  // Sending him to server 0

  nextArv = expRV(lambda);
  clock = nextArv;
  
  numArrivals++;
  P[0]++;
  idle++;

  busy[0] = true;
  arvTime[0] = nextArv;

  double temp = expRV(mu);
  sT += temp;
  sT2 += temp*temp;

  sDepTime[0] = arvTime[0] + temp;
  depTime = updateDepTime();

  totalDelay += sDepTime[0] - arvTime[0];
  nextArv = clock + expRV(lambda);

}

int MMKNQ :: cDep(){

  clock = depTime;

  if (buffer.empty()){
    // There is no one waiting in the queue
    // Set the concerned server as available and exit
    for (int i = 0; i<K; i++)
      if (depTime == sDepTime[i]){
	busy[i] = false;
	sDepTime[i] = LONG_MAX;                      // This will force an arrival next time.
	updateDepTime();
	if (drand48() < retProb)
	  backToQ();
	return 0;
      }

    cout<<"Fatal Error! Unrecognized departure time. Terminating program..."<<endl;
    cout<<"Clock: "<<clock<<"\tDeparture Time: "<<depTime<<endl;
    cout<<"sDepTimes "<<sDepTime[0]<<endl;

    exit(0);
  } 
  
  // There is someone in the wait queue
  // Make the first customer in queue the one being serviced
  // Reduce the queue size by 1

  bool foundSrv = false;
  int sN = -1;

  for (int i=0; i<K; i++)
    if(depTime == sDepTime[i]){
      foundSrv = true;
      sN = i;
      break;
    }

  if(!foundSrv){
    cout<<"Fatal Error! Unrecognized departure time. Terminating program..."<<endl;
    exit(0);
  }

  
  arvTime[sN] = buffer.front();
  buffer.pop();
  
  // Generate the departure time for the customer being serviced
  
  double temp = expRV(mu);
  sT += temp;
  sT2 += temp*temp;

  sDepTime[sN] = clock + temp;
  depTime = updateDepTime();

  totalDelay += sDepTime[sN] - arvTime[sN];

  if (drand48() < retProb)
    backToQ();

  return 1;
  
}// int MMknQ :: cDep(){

int MMKNQ :: cArv(){

  clock = nextArv;

  bool foundSrv = false;
  int freeSrv = -1;
  int freeSrvCnt = 0;
  // The statistics are updated just before this arrival enters.
  numArrivals++;
  
  for (int i=0; i<K; i++)
    if(!busy[i]){
    
      // This server is free
      freeSrv = i;
      freeSrvCnt ++;
      foundSrv = true;
    } // if(!busy[i]){

  int inSys = buffer.size() + K - freeSrvCnt;
  totalPkts += inSys;
  P[inSys]++;    

  if (inSys == 0)
    idle++;
  else
    util += 1.0 - double(freeSrvCnt)/double(K);
  
  if (foundSrv){

    // Server 'freeSrv' is free
    // The customer goes directly into service.
    busy[freeSrv] = true;
    arvTime[freeSrv] = nextArv;

    double temp = expRV(mu);
    sT += temp;
    sT2 += temp*temp;

    sDepTime[freeSrv] = arvTime[freeSrv] + temp;
    depTime = updateDepTime();
    totalDelay += sDepTime[freeSrv] - arvTime[freeSrv];

  }// if (foundSrv){
  else{
    // No server is free
    // We need to check if the system is full
    if( buffer.size() == (N-K))
      // The arrival will be blocked.
      arvBlocked++;
    else if (buffer.size() < (N-K))
      // Push the new arrival to the end of the queue
      buffer.push(nextArv);
    else{
      // We should never be here.
      cout<<"Fatal Error! Invalid Buffer size. Terminating program....\n";
      exit(0);
    }
  }

  nextArv = clock + expRV(lambda);
  return 0;
}// int MMKNQ :: newArv(){


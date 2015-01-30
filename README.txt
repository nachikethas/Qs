------------------- Part 1 ---------------------------
The aim of this part is to simulate queues with exponential arrival and service times under various
conditions given. To this end a generic queue class has been implemented that simulates an M/M/K/N
queue. The implementation of this class (named MMKNQ ) may be found in the source file part1.h.
The file part1.cpp contains the front end that accepts user inputs and simulates the queue under the
appropriate conditions specified initially.

For the cases I, II and III the number of arrivals given is not sufficient to ensure convergence of the values to what is expected theoretically. Hence I have added new cases as given below:

1. Case VI : Same as Case I but with 1000000 arrivals.

2. Case VII : Same as Case II but with 5000000 arrivals.

3. Case VIII : Same as Case III but with 1000000 arrivals.

Note that these cases will take around a minute to run. You can run the above in the same way as you would run the other cases. For example, to run case VII do:

$./part1 7

--------------------- Part 2 --------------------------
The aim of this part is to analyze the given M/G/1 system both theoretically and using simulations.
As previously the implementation of the M/G/1 queue is in the form of a class MG1Q that can be
found in the file part2.h. The file part2.cpp contains the front end that accepts the seed and the number
of arrivals from the user. Note that the first argument passed to the executable is the number of
arrivals followed by the seed. It is recommended that both values be lesser than or equal to 106.

You may run the executable as was specified in the project description. For example:-

$./part2 1000000 4443

The above will run the program for 1000000 arrivals with the seed set to 4443. Please keep the number of arrivals and the seed to be lesser than or equal to 1000000. 


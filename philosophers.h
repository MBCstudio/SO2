//
// Created by cinek on 3/13/2025.
//

#ifndef DININGPHILOSOPHERS_PHILOSOPHERS_H
#define DININGPHILOSOPHERS_PHILOSOPHERS_H

#include <atomic>
#include <thread>
#include <vector>

#include <vector>
#include <atomic>
#include <thread>

class DiningPhilosophers {
private:
    int num_philosophers;
    std::vector<std::atomic<bool>> forks;  // Forks -> true if in use
    std::atomic<int> eating_count;         // Track number of eating philosophers (efficiency)

public:
    DiningPhilosophers(int n);
    void philosopher(int id, int max_bites);
    void think(int id);
    void eat(int id);
};


void start_dining(int num_philosophers, int bites_per_philosopher);


#endif //DININGPHILOSOPHERS_PHILOSOPHERS_H

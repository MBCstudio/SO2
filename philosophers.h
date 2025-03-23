//
// Created by cinek on 3/13/2025.
//

#ifndef DININGPHILOSOPHERS_PHILOSOPHERS_H
#define DININGPHILOSOPHERS_PHILOSOPHERS_H

#include <atomic>
#include <thread>
#include <vector>

#include <mutex>
#include <thread>
#include <vector>

class DiningPhilosophers {
private:
    int num_philosophers;
    std::vector<std::mutex> forks;
    std::mutex eating_mutex;        // Mutex for securing eaters
    int eating_count;               // Amount of eaters



public:
    DiningPhilosophers(int n);
    void philosopher(int id, int max_bites);
    void think(int id);
    void eat(int id, int eaters);
};

void start_dining(int num_philosophers, int bites_per_philosopher);


#endif //DININGPHILOSOPHERS_PHILOSOPHERS_H

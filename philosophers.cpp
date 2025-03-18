//
// Created by cinek on 3/13/2025.
//

#include "philosophers.h"
#include <iostream>
#include <chrono>
#include <random>

using namespace std;

DiningPhilosophers::DiningPhilosophers(int n) : num_philosophers(n), forks(n), eating_count(0) {
    for (int i = 0; i < n; i++) {
        forks[i].store(false);  // Forks start as available
    }
}

void DiningPhilosophers::philosopher(int id, int max_bites) {
    int bites_taken = 0;
    int left_fork = id;
    int right_fork = (id + 1) % num_philosophers;
    default_random_engine generator(random_device{}());
    uniform_int_distribution<int> delay_dist(10, 100);  // make random

    while (bites_taken < max_bites) {
        think(id);

        // Floor N/2 is max eaters -> wait for turn
        while (eating_count.load() >= num_philosophers / 2) {
            this_thread::yield();
        }

        // Try to pick up both forks
        if (!forks[left_fork].exchange(true) && !forks[right_fork].exchange(true)) {
            eating_count.fetch_add(1);
            eat(id);
            bites_taken++;

            // Release forks
            forks[left_fork].store(false);
            forks[right_fork].store(false);
            eating_count.fetch_sub(1);
        } else {
            // If forks occupied release fork to help others
            forks[left_fork].store(false);
            forks[right_fork].store(false);
            this_thread::sleep_for(chrono::milliseconds(delay_dist(generator)));  // try again in short random time
        }
    }
}

void DiningPhilosophers::think(int id) {
    cout << "Philosopher " << id << " is thinking...\n";
    this_thread::sleep_for(chrono::milliseconds(500));
}

void DiningPhilosophers::eat(int id) {
    cout << "Philosopher " << id << " is eating...\n";
    this_thread::sleep_for(chrono::milliseconds(500));
}

void start_dining(int num_philosophers, int bites_per_philosopher) {
    vector<thread> threads;
    DiningPhilosophers table(num_philosophers);

    // Create philosopher threads
    for (int i = 0; i < num_philosophers; i++) {
        threads.emplace_back(&DiningPhilosophers::philosopher, &table, i, bites_per_philosopher);
    }

    // Join threads
    for (auto &t : threads) {
        t.join();
    }

    cout << "All philosophers have finished eating!\n";
}



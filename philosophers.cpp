//
// Created by cinek on 3/13/2025.
//

#include "philosophers.h"
#include <iostream>
#include <chrono>
#include <random>

using namespace std;

// global mutex for preventing 2 threads cout at once
mutex print_mutex;

DiningPhilosophers::DiningPhilosophers(int n) : num_philosophers(n), forks(n), eating_count(0) {}

void DiningPhilosophers::philosopher(int id, int max_bites) {
    int bites_taken = 0;
    int left_fork = id;
    int right_fork = (id + 1) % num_philosophers;

    default_random_engine generator(random_device{}());
    uniform_int_distribution<int> delay_dist(10, 50);  // random delay

    while (bites_taken < max_bites) {
        think(id);

        bool has_eaten = false;

        while (!has_eaten) {
            {
                lock_guard<mutex> lock(eating_mutex);
                // Max eaters = Floor(N/2)
                if (eating_count >= num_philosophers / 2) {
                    this_thread::sleep_for(chrono::milliseconds(10));
                    continue;
                }
            }

            // Try to pick up forks randomly
            bool reverse_order = (generator() % 2 == 0);
            int first_fork = reverse_order ? right_fork : left_fork;
            int second_fork = reverse_order ? left_fork : right_fork;

            // Lock these forks
            unique_lock<mutex> first_lock(forks[first_fork], try_to_lock);
            unique_lock<mutex> second_lock(forks[second_fork], try_to_lock);

            if (first_lock.owns_lock() && second_lock.owns_lock()) {
                {
                    lock_guard<mutex> lock(eating_mutex);
                    ++eating_count;  // add eaters
                }

                eat(id, eating_count);
                ++bites_taken;

                {
                    lock_guard<mutex> lock(eating_mutex);
                    --eating_count;  // subtract eaterts
                }

                has_eaten = true;
            }

            // Wait for a while if forks unavailable
            if (!has_eaten) {
                this_thread::sleep_for(chrono::milliseconds(delay_dist(generator)));
            }
        }

        // Short sleep to give others more chances to pick up forks
        this_thread::sleep_for(chrono::milliseconds(delay_dist(generator)));
    }
}

void DiningPhilosophers::think(int id) {
    lock_guard<mutex> print_lock(print_mutex);
    cout << "Philosopher " << id << " is thinking... \n";
    this_thread::sleep_for(chrono::milliseconds(500));
}

void DiningPhilosophers::eat(int id, int eaters) {
    lock_guard<mutex> print_lock(print_mutex);
    cout << "Philosopher " << id << " is eating... "<<"(number of eaters: "<<eaters<<")"<<endl;
    this_thread::sleep_for(chrono::milliseconds(500));
}

void start_dining(int num_philosophers, int bites_per_philosopher) {
    vector<thread> threads;
    DiningPhilosophers table(num_philosophers);

    // Create philosophers
    for (int i = 0; i < num_philosophers; i++) {
        threads.emplace_back(&DiningPhilosophers::philosopher, &table, i, bites_per_philosopher); // create thread in vec
    }

    for (auto &t : threads) {
        t.join(); // wait for thread end
    }

    lock_guard<mutex> print_lock(print_mutex);
    cout << "All philosophers have finished eating!\n";
}




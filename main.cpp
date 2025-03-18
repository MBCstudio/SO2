#include "philosophers.h"
#include <iostream>

using namespace std;

int main() {

    int num_philosophers;
    int bites_per_philosopher;
    cout<<"Ile Filozofow: ";
    cin>>num_philosophers;
    cout<<"Ile ugryzien: ";
    cin>>bites_per_philosopher;
    start_dining(num_philosophers, bites_per_philosopher);

    return 0;
}

# Problem jedzących filozofów - Jak uruchomić?

## Uruchomienie projektu
Projekt wykorzystuje **CMake** do konfiguracji i budowania kodu. Poniżej znajdują się kroki, które należy wykonać, aby skompilować i uruchomić program.

### Wymagania
- **CMake** 
- **Kompatybilny kompilator C++** 
- **Narzędzia do budowania** 

### Instrukcja uruchomienia

1. **Sklonuj repozytorium**:
   ```bash
   git clone https://github.com/MBCstudio/SO2.git
   cd SO2
   ```

2. **Stwórz katalog `build` i przejdź do niego**:
   ```bash
   mkdir build
   cd build
   ```

3. **Skonfiguruj projekt przy użyciu CMake**:
   ```bash
   cmake ..
   ```

4. **Skompiluj program**:
   ```bash
   cmake --build .
   ```
   Lub używając `make` (jeśli konfiguracja CMake wybrała `Makefile` jako generator):
   ```bash
   make
   ```

5. **Uruchom program**:
   ```bash
   ./SO2
   ```



# Problem jedzących filozofów - Opis

## Opis problemu
Problem jedzących filozofów to klasyczny problem synchronizacji w programowaniu współbieżnym, zaproponowany przez Edsgera Dijkstrę. Filozofów siedzą wokół okrągłego stołu i na przemian myśli oraz je. Każdy filozof ma do dyspozycji jeden widelec po swojej lewej stronie oraz jeden po prawej stronie. Aby móc jeść, filozof musi jednocześnie posiadać oba widelce. Problemem jest unikanie zakleszczenia (*deadlock*) oraz zagłodzenia (*starvation*) filozofów, gdyż wszyscy mogą próbować podnieść jeden widelec jednocześnie, blokując się nawzajem.

## Metoda rozwiązania – Ograniczenie liczby jedzących
Aby uniknąć zakleszczenia, zastosowano mechanizm ograniczenia maksymalnej liczby jedzących filozofów w danym momencie. Ograniczenie to wynosi \( \lfloor N/2 \rfloor \), gdzie \( N \) to liczba filozofów. Filozof może rozpocząć jedzenie tylko wtedy, gdy nie przekroczy tej wartości.

Każdy filozof próbuje podnieść dwa widelce atomowo, korzystając z operacji wymiany (*exchange*) na zmiennych atomowych, aby uniknąć konfliktów. Jeśli uda mu się zdobyć oba widelce, zaczyna jedzenie, a następnie je odkłada. W przeciwnym razie zwalnia zasoby i ponawia próbę po losowym czasie oczekiwania.

## Wejście programu
Program przyjmuje na wejściu:
- **Liczbę filozofów (N)**
- **Liczbę ugryzień potrzebnych do zakończenia (M)** – moment, gdy filozof uzyska oba widelce i spożyje posiłek M razy

## Warunek zakończenia
Program kończy działanie, gdy każdy filozof zje swoją ustaloną liczbę razy (M). W trakcie działania mechanizm ograniczenia liczby jedzących zapobiega blokadzie systemu oraz zagłodzeniu filozofów.

## Zalety rozwiązania
- **Brak zakleszczenia** – dzięki ograniczeniu liczby jedzących w danym czasie
- **Brak zagłodzenia** – filozofowie losowo ponawiają próby jedzenia, zwiększając szanse na sprawiedliwy dostęp do zasobów
- **Efektywność** – operacje na zmiennych atomowych zapewniają niski narzut synchronizacji

To rozwiązanie pozwala na efektywne zarządzanie zasobami w systemach wielowątkowych, unikając problemów typowych dla klasycznego podejścia do problemu jedzących filozofów.


# Problem jedzących filozofów - Wątki oraz momenty kluczowe

### Wątki w implementacji
W programie każdy filozof jest reprezentowany jako osobny wątek. Wątki te są tworzone w funkcji `start_dining`, a następnie uruchamiane jednocześnie:

```cpp
for (int i = 0; i < num_philosophers; i++) {
    threads.emplace_back(&DiningPhilosophers::philosopher, &table, i, bites_per_philosopher);
}
```

Każdy wątek wykonuje swoją logikę w funkcji `philosopher`, gdzie symulowane są stany myślenia i jedzenia.

### Sekcje krytyczne wątków
Program wykorzystuje mechanizmy synchronizacji do ochrony zasobów współdzielonych (widelców) i zapobiegania zakleszczeniom. Kluczowe momenty w działaniu programu:

1. **Oczekiwanie na możliwość jedzenia**
    - Aby zapobiec zakleszczeniom, wprowadzono ograniczenie maksymalnej liczby jedzących filozofów w danym momencie do `N/2`.
    - Jeśli liczba aktywnie jedzących filozofów osiągnie ten próg, inne wątki muszą poczekać.

   ```cpp
   while (eating_count.load() >= num_philosophers / 2) {
       this_thread::yield();
   }
   ```

2. **Próba podniesienia obu widelców**
    - Każdy filozof najpierw próbuje podnieść lewy i prawy widelec jednocześnie. Jeśli uda się podnieść oba, może jeść.
    - Operacja `exchange(true)` na zmiennych atomowych zapewnia, że tylko jeden wątek na raz może przejąć dany widelec.

   ```cpp
   if (!forks[left_fork].exchange(true) && !forks[right_fork].exchange(true)) {
       eating_count.fetch_add(1);
       eat(id);
       bites_taken++;
   ```

3. **Zwalnianie zasobów po jedzeniu**
    - Po zakończeniu jedzenia filozof odkłada widelce i zmniejsza licznik jedzących, aby inni mogli zacząć spożywanie posiłku.

   ```cpp
   forks[left_fork].store(false);
   forks[right_fork].store(false);
   eating_count.fetch_sub(1);
   ```

4. **Obsługa sytuacji, gdy nie uda się zdobyć obu widelców**
    - Jeśli filozofowi nie uda się podnieść obu widelców, zwalnia zajęte zasoby i czeka losowy czas przed ponowną próbą.

   ```cpp
   forks[left_fork].store(false);
   forks[right_fork].store(false);
   this_thread::sleep_for(chrono::milliseconds(delay_dist(generator)));
   ```



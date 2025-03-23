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
Problem jedzących filozofów to klasyczny problem synchronizacji w programowaniu współbieżnym, zaproponowany przez Edsgera Dijkstrę. Filozofowie siedzi wokół okrągłego stołu, gdzie na przemian myślą i jedzą. Każdy filozof ma do dyspozycji dwa widelce: jeden po swojej lewej stronie i jeden po prawej stronie. Aby móc jeść, filozof musi jednocześnie zdobyć oba widelce.

Problemem jest unikanie zakleszczenia (*deadlock*) oraz zagłodzenia (*starvation*) filozofów. Zakleszczenie może nastąpić, gdy każdy filozof podniesie jeden widelec i będzie czekać na drugi, blokując się nawzajem. Zagłodzenie natomiast występuje, gdy niektórzy filozofowie nigdy nie otrzymują obu widelców z powodu niesprawiedliwego dostępu do zasobów.

## Metoda rozwiązania – Ograniczenie liczby jedzących
Aby uniknąć zakleszczenia, wprowadzono ograniczenie maksymalnej liczby jedzących filozofów w danym momencie do \( \lfloor N/2 \rfloor \), gdzie \( N \) to liczba filozofów. Oznacza to, że jednocześnie jeść może najwyżej połowa filozofów.

Każdy filozof podejmuje próbę podniesienia obu widelców w losowej kolejności, co zmniejsza prawdopodobieństwo zakleszczenia. Jeśli nie uda mu się zdobyć obu widelców, zwalnia zajęty zasób i ponawia próbę po losowym czasie oczekiwania. 

## Wejście programu
Program przyjmuje na wejściu:
- **Liczbę filozofów (N)**
- **Liczbę ugryzień na filozofa (M)** – liczba posiłków, które każdy filozof musi spożyć przed zakończeniem programu

## Warunek zakończenia
Program zakończy działanie, gdy każdy filozof spożyje zadaną liczbę posiłków (M). Mechanizm ograniczenia liczby jedzących oraz losowe ponawianie prób zapobiegają blokadzie systemu i zagłodzeniu filozofów.

## Zalety rozwiązania
- **Brak zakleszczenia** – ograniczenie liczby jedzących w danym czasie minimalizuje ryzyko blokady
- **Brak zagłodzenia** – losowe opóźnienia w ponawianiu prób zapewniają równe szanse na zdobycie zasobów
- **Efektywność** – wykorzystanie zmiennych atomowych zapewnia szybkie i bezpieczne operacje synchronizacyjne

To podejście umożliwa efektywne zarządzanie zasobami w systemach wielowątkowych, minimalizując ryzyko typowych problemów synchronizacyjnych.

---

# Problem jedzących filozofów - Wątki oraz momenty kluczowe

### Wątki w implementacji
Każdy filozof jest reprezentowany jako osobny wątek. Wątki są tworzone w funkcji `start_dining` i uruchamiane jednocześnie:

```cpp
for (int i = 0; i < num_philosophers; i++) {
    threads.emplace_back(&DiningPhilosophers::philosopher, &table, i, bites_per_philosopher);
}
```

Każdy wątek wykonuje logikę w funkcji `philosopher`, gdzie symulowane są stany myślenia i jedzenia.

### Sekcje krytyczne wątków
Program wykorzystuje mechanizmy synchronizacji do ochrony współdzielonych zasobów (widelców) i unikania zakleszczenia. Najważniejsze momenty w działaniu programu to:

1. **Oczekiwanie na możliwość jedzenia**
   - Jeśli liczba aktywnie jedzących filozofów osiągnie `N/2`, inne wątki muszą poczekać.

   ```cpp
   while (eating_count.load() >= num_philosophers / 2) {
       this_thread::yield();
   }
   ```

2. **Próba podniesienia obu widelców**
   - Filozof podnosi widelce w losowej kolejności. Jeśli nie uda mu się zdobyć obu, zwalnia zasoby i czeka losowy czas przed ponowną próbą.

   ```cpp
   bool reverse_order = (generator() % 2 == 0);
   int first_fork = reverse_order ? right_fork : left_fork;

   unique_lock<mutex> first_lock(forks[first_fork], try_to_lock);
   if (!first_lock.owns_lock()) continue;

   unique_lock<mutex> second_lock(forks[second_fork], try_to_lock);
   if (!second_lock.owns_lock()) continue;
   ```

3. **Zwalnianie zasobów po jedzeniu**
   - Po zakończeniu jedzenia filozof odkłada widelce i zmniejsza licznik jedzących, umożliwiając innym rozpoczęcie jedzenia.

   ```cpp
   forks[left_fork].store(false);
   forks[right_fork].store(false);
   eating_count.fetch_sub(1);
   ```

4. **Obsługa sytuacji, gdy nie uda się zdobyć obu widelców**
   - Jeśli filozof nie zdobył obu widelców, zwalnia zasoby i czeka losowy czas przed ponowną próbą.

   ```cpp
   forks[left_fork].store(false);
   forks[right_fork].store(false);
   this_thread::sleep_for(chrono::milliseconds(delay_dist(generator)));
   ```





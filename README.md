# Algoritmi Igra nasprotnika - Modra

## Struktura projekta

- `docs/` - dokumentacija projekta (opis delovanja in README v pdf)
- `src/` - izvorna koda
- `include/` - izvorna koda (glave datotek)
- `tests/` - primeri vhodnih podatkov
- `scripts/` - skripta za generiranje podatkov in optimalne resitve

## Prevajanje

Za prevajanje projekta je priporočena uporaba orodja **CMake**. V korenski mapi projekta izvedite naslednje ukaze:

```bash
mkdir build
cd build
cmake ..
make
```

## Poganjanje programa

Program pri zagonu zahteva obvezen argument, ki predstavlja pot do datoteke z vhodnimi podatki ustreznega formata. Primeri vhodnih datotek se nahajajo v mapi `tests/`, s končnico `.txt`.

Ko ste projekt prevedli s CMake in se nahajate v mapi `build`, zaženite:
```bash
./game_of_adversary ../tests/manual_test.txt
```

## Poganjanje testov

Proces prevajanja s CMake samodejno pripravi tudi testni program (`test_runner`). Teste lahko najlažje zaženete s pomočjo orodja `ctest` znotraj mape `build`:

```bash
cd build
ctest --verbose
```

### Testiranje z ročno vnešenimi podatki

Za testiranje z ročno vnešenimi podatki je potrebno urediti datoteko `tests/manual_test.txt` in v njej določiti število elementov ter vrednosti posameznih elementov. Nato teste, kot običajno, zaženete s pomočjo orodja `ctest` znotraj mape `build`:

```bash
cd build
ctest --verbose
```

## Generiranje naključnih vhodnih podatkov

Za generiranje novih vhodnih podatkov za različne porazdelitve (enakomerna, konstantna, linearna, normalna in eksponentna) služi priložena Python skripta `scripts/utils.py`. Prav tako se hkrati ustvarijo optimalne rešitve generiranih vhodnih podatkov z ogrodjem **ortools**, ki se shranijo v datoteke z enakim imenom kot vhodni podatki, le da imajo končnico `_optimal.out`. Vse datoteke se shranijo v mapo `tests/`.

```bash
# Aktivirajte okolje (če še ni ustvarjeno: python -m venv .venv)
source .venv/bin/activate

# Namestite potrebne knjižnice
pip install -r requirements.txt

# Zaženite skripto
python scripts/utils.py
```

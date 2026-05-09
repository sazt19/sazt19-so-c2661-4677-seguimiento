# Taller 8

## Agenda

1. Preliminares
2. Algoritmo del Banquero
3. Algoritmos de Memoria Continua (CMA - Continuous Memory Algorithms)
4. Compilación condicional
5. Preguntas y respuestas de la práctica

## Preliminares

Abra una terminal. Abra el directorio de repositorio y de los talleres

```
cd <repositorio-enlace>
cd talleres
```

Descargue el taller en formato zip, descomprimalo.

```
wget https://github.com/jfcmacro/TallerSO_08/archive/refs/heads/master.zip
unzip master.zip
rm master.zip
```

Mire la estructura actual

```
tree .
```

Entre al directorio del taller

```
cd TallerSO_08-master
```

Adicione los ficheros

```bash
git add README.md .gitignore
```

Adicione los ficheros del proyecto, esto adiciona todos ficheros del taller.

```bash
find . -name *.c -exec git add {} \;
find . -name *.h -exec git add {} \;
find . -name .keep -exec git add {} \;
find . -name makefile -exec git add {} \;
find . -name *.mk -exec git add {} \;
```

Acometa (*commit*) el proyecto.

```
git commit -m "Iniciando el Taller 08"
git push
```

## Algoritmo del banquero

* [Algoritmo del banquero](./banquero)

## Algoritmos de Memoria Continua (CMA - Continuous Memory Algorithms)


Este es un simulador de algoritmos de memoria continua:

   * Primer ajuste
   * Mejor ajuste
   * Peor ajuste

Cada algoritmo recibe tres argumentos:
   * `mem_avail`: una lista de tuplas, cada una con base y limite.
   * `req`: un requerimiento para la memoria
   * `index` posición en la `mem_avail`. Es un índice circular: `(index % len(mem_avail))`.

Cada algoritmo returna dos posibles valores: `None` si el requerimiento no puede ser completado, o una cuadrúpla:
   * `mem_avail`: una lista de tuplas de memoria de están disponibles (puede estar vacía).
   * `base`: la nueva base donde el requerimiento de un proceso inicia.
   * `limit`: nuevo limite.
   * `index`: el indice en la `mem_avail` donde fue encontrado el requerimiento. Si la memoria fue consumida completamente, la siguiente posición disponible.

## Test

First, set the `PYTHONPATH` variable replace (`<path-of-your-project`)
with your directory path.

Linux

```shell
export PYTHONPATH=$PYTHONPATH:<path-of-your-project>
```

Install two libraries

```shell
python3 -m pip install pytest parameterized
```

```shell
python -m pip install pytest parameterized
```

Windows

```shell
SET PYTHONPATH=%PYTHONPATH%;<path-of-your-project>
```

Install two libraries

```shell
python3 -m pip install pytest parameterized
```

```shell
python -m pip install pytest parameterized
```


Running the test, replace with the name of your algorithm implementation: `first`, `best`, `worst`.

```shell
python3 -m unittest test/test_basic_<your_algorithm>_fit.py
python3 -m unittest test/test_other_<your_algorithm>_fit.py
```

```shell
python -m unittest test/test_basic_<your_algorithm>_fit.py
python -m unittest test/test_other_<your_algorithm>_fit.py
```

## Execute cma simulator

First install click

```shell
python3 -m pip install click
```

```shell
python -p pip install click
```

To run the program

Windows:

```shell
python3 cma.py --memmap .\resources\memmap\memmap_1.txt --reqs .\resources\reqs\reqs_1.txt
```

Linux:

```shell
python cma.py --memmap ./resources/memmap/memmap_1.txt --reqs ./resources/reqs/reqs_1.txt
```

The previous execution executes all algorithms you can change to execute different algorithm.

### To execute the first algorithm.

Windows

```shell
python3 cma.py --memmap .\resources\memmap\memmap_1.txt --reqs .\resources\reqs\reqs_1.txt --function first
```

Linux

```shell
python cma.py --memmap ./resources/memmap/memmap_1.txt --reqs ./resources/reqs/reqs_1.txt --function first
```

### To execute the best algorithm.

Windows:

```shell
python3 cma.py --memmap .\resources\memmap\memmap_1.txt --reqs .\resources\reqs\reqs_1.txt --function best
```

Linux:

```shell
python cma.py --memmap ./resources/memmap/memmap_1.txt --reqs ./resources/reqs/reqs_1.txt --function best
```

### To execute the worst algorithm.

Windows:

```shell
python3 cma.py --memmap .\resources\memmap\memmap_1.txt --reqs .\resources\reqs\reqs_1.txt --function worst
```

Linux:

```shell
python cma.py --memmap ./resources/memmap/memmap_1.txt --reqs ./resources/reqs/reqs_1.txt --function worst
```

### To execute in different possition

Windows:

```shell
python3 cma.py --memmap .\resources\memmap\memmap_1.txt --reqs .\resources\reqs\reqs_1.txt --pos 3
```

Linux:

```shell
python cma.py --memmap ./resources/memmap/memmap_1.txt --reqs ./resources/reqs/reqs_1.txt --pos 3
```

## Condicional

* [compcond](./compcond)
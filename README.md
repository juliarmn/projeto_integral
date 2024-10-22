# Projeto do cálculo de Integral

Neste projeto usamos a fórmula do trapézio para calcular uma integral dupla:

$$
\int_{0}^{1.5} \int_{0}^{1.5} \sin(x^2 + y^2) \, dx \, dy
$$

Assim, usamos a OpenMP para comparar o desempenho em relação ao número de intervalos de x e y e número de threads.

## Para rodar

Para rodar pode fazer automatizado com:

    ./rodar.sh

Que faz todas as combinações de intervalo de x e y, além do número de threads. Ou pode fazer manualmente, com:

    OMP_NUM_THREADS=$threads ./p $n_intervals_x $n_intervals_y

O valor de $threads é o número de threads (int) e os intervalos igualmente.
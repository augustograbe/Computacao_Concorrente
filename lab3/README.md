# Implementação e avaliaçãoo de aplicações concorrentes (parte 2)
## Resultados:
### Matriz 500x500
| | Teste 1 | Teste 2 | Teste 3 | Média |
|---|---|---|---|---|
| **Inicio**        | 0.0020319s | 0.001716s  | 0.0015195s | 0.0017558s |
| **Concorrente**   | 0.2047608s | 0.1969268s | 0.1921491s | 0.1979455s |
| **Final**         | 0.0016969s | 0.0012724s | 0.0015248s | 0.0014980s |

O ganho em relação a implementação sequencial do lab2 foi de 

S=0.6078356/(0.0017558+0.1979455+0.0014980) = 3.021062201

Ou seja, 3.938710566-3.021062201 = 0.917648365 a menos que o valor estimado no lab2.

### Matriz 1000x1000
| | Teste 1 | Teste 2 | Teste 3 | Média |
|---|---|---|---|---|
| **Inicio**        | 0.0057205s | 0.0060596s | 0.0029818s | 0.0049206s |
| **Concorrente**   | 1.747177s  | 1.743123s  | 1.640848s  | 1.710382s  |
| **Final**         | 0.0032352s | 0.0040997s | 0.0031488s | 0.0034945s |

O ganho em relação a implementação sequencial do lab2 foi de 

S=6.7305685/(0.0049206+1.710382+0.0034945) = 3.915859818

Ou seja, 3.980781193 - 3.915859818 = 0.064921375 a menos que o valor estimado no lab2.

### Matriz 2000x2000
| | Teste 1 | Teste 2 | Teste 3 | Média |
|---|---|---|---|---|
| **Inicio**        | 0.0068963s | 0.0079085s | 0.0132476s | 0.0103102s |
| **Concorrente**   | 16.29329s  | 16.03492s  | 16.61481s  |  16.31434s |
| **Final**         | 0.004339s  | 0.003119s  | 0.0097745s | 0.0057441s |

O ganho em relação a implementação sequencial do lab2 foi de 

S=59.7680697/(0.0103102+16.31434+0.0057441) = 3.659928144

Ou seja, 3.992324423 - 3.659928144 = 0.332396279 a menos que o valor estimado no lab2.

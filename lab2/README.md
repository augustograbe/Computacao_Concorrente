## Resultados:
### Matriz 500x500
| | Teste 1 | Teste 2 | Teste 3 | Média |
|---|---|---|---|---|
| **Inicio**        | 0.0016872s | 0.0018784s | 0.0017729s | 0.0017795s |
| **Multiplicação** | 0.5980954s | 0.5967036s | 0.6192495s | 0.6046828s |
| **Final**         | 0.0014480s | 0.0014438s | 0.0012283s | 0.0013733s |

Tempo total sequencial: 0.6078356s

Com 4 processadores, a parte paralela gastará p/n = 0.6046828/4 = 0.1511707s

### Matriz 1000x1000
| | Teste 1 | Teste 2 | Teste 3 | Média |
|---|---|---|---|---|
| **Inicio**        | 0.0070056s | 0.0077925s | 0.0085368s | 0.0077783s |
| **Multiplicação** | 6.724778s  | 6.654932s  | 6.779501s  | 6.719737s |
| **Final**         | 0.0029247s | 0.0031899s | 0.0030452s | 0.0030532s|

Tempo total sequencial: 6.7305685s

Com 4 processadores, a parte paralela gastará p/n = 6.719737/4 = 1.67993425s

### Matriz 2000x2000
| | Teste 1 | Teste 2 | Teste 3 | Média |
|---|---|---|---|---|
| **Inicio**        | 0.0305779s | 0.0295831s | 0.0280758s | 0.029412266s |
| **Multiplicação** | 59.52293s  | 60.04929s  | 59.61708s  | 59.72976667s |
| **Final**         | 0.0086260s | 0.0091496s | 0.0088967s | 0.008890766s |

Tempo total sequencial: 59.7680697s

Com 4 processadores, a parte paralela gastará p/n = 59.72976667/4 = 14.93244167s

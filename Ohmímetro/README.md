<header>
<h2> Fluxograma Principal </h2>
</header>

<br>

```txt

INÍCIO
  ↓
Inicializa Serial e Display OLED
  ↓
Display pronto?
  ├── NÃO → Exibe erro e trava execução
  └── SIM
         ↓
   Limpa display
         ↓
   Inicia loop principal
         ↓
   Limpa display
         ↓
   Configura texto (tamanho, posição)
         ↓
   Repete 99 vezes:
         ├─ Lê valor do ADC (tensão)
         ├─ Calcula Vout
         ├─ Faz média de Vout
         └─ Aguarda 10ms
         ↓
   Vout >= 4.995V?
     ├── SIM → 
     │       ├─ Zera média
     │       ├─ Exibe mensagem "Insira um Resistor"
     │       ├─ Mostra erro no serial
     │       └─ Espera 500ms e reinicia loop
     └── NÃO
         ↓
   Calcula resistência desconhecida (resistencia)
         ↓
   Chama função mostrarResultado(mediaVout, resistencia)
         ├─ Exibe valor em Ohms ou kOhms no display
         ├─ Mostra resultado no Serial
         ├─ Configura texto para faixas do resistor
         ├─ Calcula faixa1, faixa2, multiplicador
         ├─ Arredondamento das faixas se necessário
         ├─ Desenha resistor no display
         └─ Mostra nomes das cores das faixas no display
         ↓
   Atualiza display
         ↓
   Aguarda 100ms
         ↓
FIM DO LOOP (repete)

```

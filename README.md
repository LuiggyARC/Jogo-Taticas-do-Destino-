# ⚔️ Táticas do Destino

**Táticas do Destino** é um RPG tático por turnos para Android inspirado nas mecânicas clássicas de **Dungeons & Dragons 3.5**, combinando combate estratégico em grid, progressão profunda de personagens, inteligência artificial tática e uma campanha original em desenvolvimento.

---

# 🚀 Atualizações Recentes

## 🔧 Correções Críticas de Infraestrutura (JNI)

### Compatibilidade com Android Games SDK

- Correção do erro de compilação relacionado ao acesso interno da `GameActivity`.
- Adequação completa ao padrão `javaGameActivity` utilizado pelo Android Games SDK.
- Ajustes no sistema JNI para garantir compatibilidade entre Kotlin, Java e C++.

### Sistema Dinâmico de Carregamento de Telas

- Correção definitiva do crash de `Class Not Found`.
- Implementação de carregamento dinâmico de Activities via `ClassLoader`.
- Integração entre o código nativo C++ e o sistema Android de Activities.
- Agora o motor consegue abrir telas dinamicamente sem conflitos internos.

---

# ⚡ Estabilidade e Performance

## Fim da Tela Preta

- Remoção do efeito de tela preta piscando entre transições.
- Ajuste do tema Android para modo translúcido.
- Desativação das animações padrão do sistema Android.
- Otimização visual para deixar apenas os efeitos cinematográficos do jogo.

## Motor de Áudio Persistente

- Correção do bug de músicas duplicadas.
- Sistema de música contínua entre menus.
- `Tema1.mp3` agora permanece ativo sem reiniciar ao trocar de tela.
- Compatibilidade total entre música de fundo e efeitos sonoros.

---

# 🎨 Interface e Visual (UI/UX)

## Sistema de Fade Cinematográfico

- Implementação de Fade Out e Fade In suaves.
- Transições com duração aproximada de 0.5 segundos.
- Navegação mais fluida e profissional.
- Eliminação de cortes bruscos.

## Menu Dinâmico

### Estrutura Visual

- Logo posicionada no topo direito.
- Botões ampliados com escala 2.5.
- Molduras estilizadas.
- Efeito visual de botão pressionado.

### Sistema de Texto Nativo

- Implementação de renderização nativa para os textos dos botões.
- Centralização automática dos textos.
- Melhor compatibilidade visual com diferentes resoluções.

Exemplos:

- NOVO JOGO
- CONTINUAR
- CONFIGURAÇÕES
- SAIR

## Precisão de Toque

- Correção completa da matemática das áreas de clique.
- Sincronização entre renderização visual e coordenadas de toque.
- Eliminação de sobreposição invisível entre botões.

---

# 🎮 Modo de Teste (Gameplay Base)

## Sistema de Zoom

- Implementação de zoom automático 2x.
- Melhor visualização dos sprites.
- Maior definição visual durante os testes.

## Posicionamento de Personagens

- Ajuste correto da posição do Herói.
- Ajuste correto da posição dos Monstros.
- Correção de alinhamento com o solo do mapa.
- Melhor escala visual dos personagens.

## Barras de Vida Dinâmicas

- Implementação do sistema visual de HP.
- Barras verdes dinâmicas.
- Atualização em tempo real.
- Molduras decorativas para os personagens.

---

# 🧠 Resultado Atual do Projeto

O jogo agora possui:

- Navegação fluida.
- Sistema estável de transição entre telas.
- Música persistente.
- Interface mais profissional.
- Base sólida para expansão do gameplay.
- Ambiente pronto para implementação da lógica de combate.
- Sistema visual mais cinematográfico.
- Estrutura preparada para movimentação e combate tático.

---

# 🎯 Objetivo do Projeto

Criar uma experiência de RPG tático mobile inspirada em D&D 3.5, focada em:

- Estratégia.
- Progressão de personagens.
- Combate em grid.
- Inteligência artificial.
- Exploração.
- Evolução narrativa.

---

# 📊 Status Atual

## Sistemas Implementados

- Sistema de atributos.
- Classes inspiradas em D&D 3.5.
- Sistema de talentos.
- Sistema de magias.
- Combate tático em grid.
- IA para inimigos.
- Sistema de XP.
- Sistema de loot.
- Save/Load.
- Menus funcionais.
- Música ambiente.
- Fade cinematográfico.
- Zoom automático.
- Barras de vida.
- Sistema JNI integrado.
- Sistema dinâmico de Activities.

---

# 🛣️ Próximos Passos

## Curto Prazo

- Implementação da movimentação tática.
- Sistema de ataque.
- Sistema de turnos.
- Integração dos sprites finais.
- Sistema de colisão.

## Médio Prazo

- Sistema completo de combate.
- Magias funcionais.
- Inventário.
- Equipamentos.
- IA avançada.
- Sistema de quests.

## Longo Prazo

- Campanha completa.
- Chefes únicos.
- Expansão de classes.
- Multiplayer local/testes cooperativos.
- Sistema de áudio avançado.
- Expansões futuras.

---

# 📷 Capturas de Tela

## Tela Inicial

<img width="2400" height="1080" alt="Screenshot_20260607_154427" src="https://github.com/user-attachments/assets/f68a4f5b-7a42-4cdb-8982-df32656f7799" />


---

## Criação de Personagem

<img width="1672" height="941" alt="Criação de Personagem" src="https://github.com/user-attachments/assets/e319cc66-72d2-4c5b-9d7f-5cdcaa9eacb8" />

---

## Tela Save e Load

<img width="1672" height="941" alt="Tela_Continuar" src="https://github.com/user-attachments/assets/720ee305-9cd6-4c9f-bf0a-0dee71da93ef" />

---

## Tela de testes

<img width="2400" height="1080" alt="Screenshot_20260607_160856" src="https://github.com/user-attachments/assets/f47fa79b-ea17-4007-b827-a0ed4eaf6ae7" />


# 👨‍💻 Autor

## Luiggy Alberto

GitHub:

https://github.com/LuiggyARC

---

Projeto desenvolvido como estudo avançado de:

- Desenvolvimento Android.
- JNI.
- Renderização 2D.
- Inteligência Artificial.
- Arquitetura de jogos.
- Sistemas de RPG tático.
- Integração Kotlin + C++.

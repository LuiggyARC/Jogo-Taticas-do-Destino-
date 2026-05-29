# ⚔️ Táticas do Destino

**Táticas do Destino** é um RPG tático por turnos para Android, inspirado nas mecânicas clássicas de **Dungeons & Dragons 3.5**, combinando combate estratégico em grid, progressão profunda de personagens, inteligência artificial tática e uma campanha original em desenvolvimento.

O objetivo do projeto é transformar regras tradicionais de RPG de mesa em uma experiência digital estratégica, preservando sistemas como atributos, raças, classes, talentos, magias, testes de resistência, iniciativa, equipamentos, progressão de níveis e combate posicional.

---

## 🎯 Objetivo do Projeto

Criar uma experiência de RPG tático mobile que combine:

- Profundidade estratégica inspirada em D&D 3.5
- Combate por turnos baseado em posicionamento
- Progressão significativa de personagens
- Inteligência artificial para inimigos e criaturas
- Campanha original dividida em capítulos
- Sistema expansível para novas classes, monstros, mapas e histórias

---

## 🌎 Premissa da Campanha

No primeiro capítulo, um grupo de aventureiros é enviado para investigar um culto misterioso que busca libertar uma entidade ancestral capaz de dominar os planos material e espiritual.

Durante a jornada, o jogador enfrentará criaturas cada vez mais perigosas, explorará diferentes regiões, evoluirá seus personagens e tomará decisões estratégicas em batalhas táticas.

---

## 🏗️ Estado Atual do Desenvolvimento

O projeto já possui uma base funcional para o motor principal de regras, combate e progressão. A estrutura atual permite expansão para novas campanhas, inimigos, itens, magias e sistemas visuais.

### Núcleo de Regras Implementado

- Sistema de atributos:
  - Força
  - Destreza
  - Constituição
  - Inteligência
  - Sabedoria
  - Carisma
- Raças com bônus raciais
- Modificadores de tamanho
- Classes clássicas inspiradas em D&D 3.5
- Progressão automática por classe
- Cálculo de BAB (Base Attack Bonus)
- Salvamentos automáticos
- Dados de vida por classe
- Sistema de talentos (Feats)
- Sistema de magias com CD, resistência e slots

---

## 🛡️ Classes e Progressão

O jogo possui estrutura para classes clássicas de RPG, incluindo:

- Guerreiro
- Mago
- Bárbaro
- Ladino
- Ranger
- Druida
- Outras classes planejadas para expansão futura

Cada classe possui regras próprias de progressão, ataques, salvamentos e dados de vida.

---

## ⭐ Sistema de Talentos

Talentos implementados ou estruturados:

- Ataque Poderoso
- Foco em Arma
- Tiro Certeiro
- Talentos adicionais planejados para expansão

O sistema foi criado para permitir o crescimento estratégico dos personagens conforme avançam de nível.

---

## 🔮 Sistema de Magias

O motor de magias inclui:

- Magias de nível 0 a 3
- Slots limitados por classe
- Classe de Dificuldade (CD)
- Resistência à Magia
- Regras específicas por conjurador
- Estrutura expansível para novas magias

---

## ⚔️ Sistema de Combate Tático

### Grid e Movimento

- Combate baseado em grid
- Movimento diagonal com regra 1-2-1
- Terreno normal
- Terreno difícil
- Obstáculos
- Controle de posicionamento

### Combate Corpo a Corpo e à Distância

- Ataques corpo a corpo
- Ataques à distância
- Penalidade por alcance
- Linha de visão (LoS)
- Sistema de cobertura
- Modificadores automáticos de combate

### Turnos e Iniciativa

- Gerenciamento de iniciativa
- Ordem automática de turnos
- Controle de duração de efeitos
- Estrutura para habilidades temporárias, como fúria e buffs

---

## 🤖 Inteligência Artificial

A IA dos inimigos foi planejada para tomar decisões táticas, incluindo:

- Busca por cobertura
- Flanqueamento
- Uso de vantagem de alcance
- Priorização de ameaças
- Uso de habilidades especiais
- Adaptação ao posicionamento do jogador

Exemplos de comportamentos previstos ou estruturados:

- Beholder utilizando raios especiais
- Troll utilizando regeneração
- Criaturas com alcance posicionando-se estrategicamente

---

## 🌍 Mundo, História e Progressão

### Estrutura de Campanha

- Trilha principal planejada com 20 níveis
- Evolução de dificuldade por mapa
- Mapas variando de 10x10 até 20x20
- Inimigos organizados por tiers
- Progressão narrativa por capítulos

### Sistema de XP

Progressão exponencial inspirada em RPGs clássicos:

- 1.000 XP
- 2.000 XP
- 4.000 XP
- 8.000 XP
- E assim sucessivamente

---

## 🐺 Companheiros Animais

O projeto possui estrutura para companheiros animais, especialmente voltada para classes como:

- Druida
- Ranger

Esses aliados poderão acompanhar o jogador durante batalhas e progressão da campanha.

---

## 💰 Economia, Equipamentos e Loot

### Loja

Sistema planejado/estruturado para compra de:

- Armas
- Armaduras
- Equipamentos básicos

### Ferreiro

Sistema de aprimoramento de equipamentos:

- +1
- +2
- +3
- +4
- +5
- +6

Com progressão de custo em ouro.

### Itens Raros

Monstros difíceis podem conceder acessórios raros com bônus reais, como:

- Manoplas
- Cintos
- Tiaras
- Artefatos especiais

---

## 💾 Sistema de Salvamento

Persistência planejada/estruturada para:

- Progresso da campanha
- Personagens
- Itens
- Equipamentos
- Ouro
- Nível atual da história

---

## 🎨 Sistema Visual

O projeto utiliza uma abordagem 2D com foco em leitura tática do campo de batalha.

Recursos visuais:

- Sprites 2D
- Câmera tática
- Pan
- Zoom
- Barras de vida sobre personagens
- Telas de inventário
- Menu principal
- Tela de criação de personagens

---

## 👥 Criação de Personagem

O sistema de criação de personagem possui estrutura para:

- Escolha de raça
- Escolha de classe
- Sorteio de atributos
- Montagem de equipe
- Preparação para progressão individual

---

## 📱 Tecnologias Utilizadas

- Kotlin
- Android Studio
- Android SDK
- Renderização 2D
- Estrutura preparada para Android 11 (API 30+) ou superior

---

## 🚧 Roadmap

### Curto Prazo

- Integrar sprites definitivos dos personagens
- Refinar a tela de criação de personagem
- Criar diálogos da história
- Melhorar feedback visual das ações
- Ajustar interface de combate

### Médio Prazo

- Expandir lista de magias
- Adicionar novos monstros
- Criar chefes únicos
- Melhorar animações de ataque
- Adicionar efeitos visuais
- Implementar novos equipamentos

### Longo Prazo

- Campanha completa
- Missões secundárias
- Sistema de conquistas
- Sons e trilha sonora
- Expansões de conteúdo
- Balanceamento avançado

---

## 📷 Capturas de Tela

> Espaço reservado para imagens do projeto.

### Tela Inicial

Adicionar print aqui.

### Criação de Personagem

Adicionar print aqui.

### Combate Tático

Adicionar print aqui.

### Inventário

Adicionar print aqui.

---

## 📊 Status

**Fase atual:** desenvolvimento ativo.

O motor principal de regras, combate e progressão já possui uma base sólida. O foco atual está na integração dos sprites definitivos, refinamento visual, construção da narrativa e melhoria da experiência do usuário.

---

## 👨‍💻 Autor

**Luiggy Alberto**

GitHub: [LuiggyARC](https://github.com/LuiggyARC)

Projeto desenvolvido como estudo avançado de desenvolvimento Android, arquitetura de jogos, inteligência artificial para NPCs, sistemas de RPG e design de combate tático.

# Sistemas-Embarcados---Stock-Car---GLCD
Projeto Final do Curso Sistemas Embarcados

Departamento de Computação - UFSCar

Professor Dr. Emerson Carlos Pedrino

# Jogo de Corrida para Microcontrolador AT89S52 com GLCD 128x64

Este repositório contém o código-fonte e os arquivos de simulação para um jogo de corrida simples desenvolvido para um microcontrolador 8051 (especificamente o AT89S52) com interface gráfica via display GLCD (Graphic Liquid Crystal Display) de 128x64 pixels. O projeto demonstra conceitos fundamentais de programação embarcada, controle de hardware e desenvolvimento de lógicas de jogo.

##  Visão Geral do Projeto

O jogo simula uma corrida de carros onde o jogador controla um veículo que se move horizontalmente para desviar de obstáculos (outros carros) e das bordas de uma pista que se desloca verticalmente. A pontuação aumenta com o tempo, e a dificuldade é escalonada com o aumento da velocidade do jogo.

### Funcionalidades Implementadas:
* **Controle do Jogador:** Movimento lateral do carro via botões.
* **Pista Dinâmica:** Geração procedural de uma pista com curvas, proporcionando um ambiente de jogo variável.
* **Inimigos:** Geração aleatória de obstáculos (carros inimigos) que se movem verticalmente.
* **Sistema de Colisão:** Detecção de colisão entre o carro do jogador e as bordas da pista ou inimigos.
* **Pontuação e Dificuldade:** Pontuação em tempo real e aumento gradual da velocidade do jogo para escalar a dificuldade.
* **Interface Gráfica:** Renderização de sprites para o carro do jogador, inimigos, linhas da pista, texto (GAME OVER, SCORE) e dígitos da pontuação.

##  Tecnologias e Ferramentas

* **Microcontrolador:** AT89S52 (família 8051)
* **Display:** GLCD 128x64 (compatível com controladores como KS0108)
* **Ambiente de Desenvolvimento (IDE):** Keil uVision
* **Simulação de Hardware:** Proteus Design Suite (para simulação do circuito e do código embarcado)
* **Linguagem de Programação:** C


##  Como Compilar e Simular

### 1. Compilação do Código (Keil uVision)
1.  Abra o projeto no Keil uVision.
2.  Compile o código-fonte (`.c`) para gerar o arquivo `.hex` (firmware). Este arquivo conterá o código executável para o microcontrolador.
3.  Abra Root no proteus e carregue o arquivo .c no microcontrolador.
4.  Compile o código

## Acesse o vídeo de Demonstração!

[![Assista ao vídeo](https://img.youtube.com/vi/rZBkgv5a9EE/0.jpg)](https://youtu.be/rZBkgv5a9EE)

# MonnyLang

## Tudo descrito será o que ela será quando tiver em um nível mais alto de desenvolvimento

MonnyLang é uma **linguagem de scripting leve**, escrita em **C**, projetada para ser **embutida em aplicações e game engines**, com foco em **simplicidade**, **controle de memória** e **desempenho previsível**.

Ela foi desenvolvida como parte de uma game engine própria, permitindo a criação de lógica de jogo e scripts sem comprometer performance ou depender de runtimes externos.

---

## ✨ Principais características

- Linguagem interpretada, implementada inteiramente em **C**
- Arquitetura pensada para **integração direta com engines**
- Controle explícito de memória
- Execução rápida e previsível
- Sintaxe simples, inspirada em linguagens imperativas
- Fácil extensão via código C (funções nativas)

---

## 🎯 Objetivo do projeto

O objetivo da MonnyLang é servir como uma alternativa leve a linguagens de script tradicionais (como Lua), oferecendo:

- Maior controle sobre alocação e ciclo de vida dos objetos
- Facilidade de integração com sistemas escritos em C/C++
- Base educacional para estudo de **interpretação de linguagens**, **VMs** e **design de linguagens**

---

## 🛠️ Arquitetura

O projeto é estruturado em módulos bem definidos:

- **Lexer** – Tokenização do código fonte
- **Parser** – Análise sintática e construção da estrutura interna
- **Bytecode / AST** – Representação intermediária do código
- **Virtual Machine (VM)** – Execução das instruções
- **Gerenciamento de memória** – Alocador próprio e controle de objetos

A arquitetura foi pensada para ser clara, extensível e adequada a projetos de sistemas.

---

## 🚀 Exemplo de código

```monny
set x = 10;
set y = 20;

func soma(a, b) {
    return a + b;
}

display(soma(x, y));


Jogo labirinto - Computação Gráfica
-----------------------------------------------------------------------

Este projeto foi desenvolvido como um trabalho da matéria de Computação Gráfica. O intuito do jogo é atravessar a mesa sem esbarrar nos obstáculos que ficam se mexendo constantemente.
Para cada vez que se chaga no final o nível é aumentado e a velocidade dos muros aumenta, sendo que, caso colida com algum muro ou com as extremidades da mesa o jogador perde o jogo e o mesmo é resetado.

## Requisitos
* OpenGL;
* OpenCV (Versão usada: 2.4.1);
* Visual Studio 2010 ou superior.

## Configuração
**OpenGL e OpenCV**
    * Clicar c/ botão direito no projeto **CG01**
        - C/C++
            - Geral
                - Diretórios de Inclusão Adicionais
                    - Editar
                        - Incluir o caminho das pastas do OpenGL e OpenCV

## Interações com o teclado

É possível controlar a posição do observador com as seguintes teclas:
* W, S, A e D.

Pause / Start do jogo:
* Tecla ESPAÇO.

Controle de direção da esfera:
* Setas (GLUT_KEY_RIGHT, GLUT_KEY_LEFT, GLUT_KEY_UP e GLUT_KEY_DOWN).

Inclinação da mesa de jogo:
* Num 8 – (inclina para cima);
* Num 2 – (inclina para baixo);

-----------------------------------------------------------------------

## Screenshots:  

Labirinto:
![Labirinto](https://github.com/felidan/jogo-obstaculos-openGL/blob/master/Labirinto.png)

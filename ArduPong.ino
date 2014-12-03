/*
 Copyright (c) 2013-2014 softWEB Production All Right Reserved, https://swp.pt
 This source is subject to the softWEB Production Licensing models.
 All other rights reserved.
 
 THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 PARTICULAR PURPOSE.
 
 
 Title:		ArduPong
 Descript.  Arduino based Pong like game
 Version:	1.0
 Company:	softWEB Production
 Author:	Isaias Lourenco
 Copyright: softWEB Production © Moerbius, 2014
 License:   MIT
 Website:	https://swp.pt
 
*/
 
 
/*
 Atribuição dos pinos
 
 LCD:
 D3
 D4
 D5
 D6
 D7
 
 Paddle (Pot):
 A0
 
 Botões:
 Esquerda:
 A1
 
 Cima:
 A2
 
 Direita:
 A3
 
 Baixo:
 A4
 
 Enter:
 A5
 
 */


#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "logo.h"
#include "Button.h"
#include "messages.h"

#define POT1PIN     A0
#define MAXTURNS    5
#define BALLDELAY   25

struct PADDLE {
    long currentXpos;
    long currentYpos;
    int width;
    int height;
};

struct BALL {
    double currentXpos;
    double currentYpos;
    int radius;
    int angle;      //Angulo entre a trajetória da bola e o eixo horizontal
    int angleSign;  //cima = +1, baixo = -1, horizontal = 0
    int direction;  //esquerda = 1, direita = 0
};

PADDLE paddle1, paddle2;
BALL ball;
Button btnEnter = Button(A5);

//MESSAGES msg;

unsigned long currentMillis;
unsigned long lastMillis;
int score1;
int score2;
boolean scored;
boolean gameover;
boolean isPaused;

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

void game();
void drawBorders();
void drawBall();
void drawScore();
void drawPaddles();
void resetBall();

void setup() {
    
    Serial.begin(9600);
    
    display.begin();
    display.clearDisplay();
    display.setContrast(50);
    
    paddle1.width = 3;
    paddle1.height = 14;
    paddle1.currentXpos = 0;
    paddle1.currentYpos = display.height() / 2 - paddle1.height / 2;
    
    paddle2.width = 3;
    paddle2.height = 14;
    paddle2.currentXpos = display.width() - 3;
    paddle2.currentYpos = display.height() / 2 - paddle2.height / 2;
    
    ball.radius = 2;
    
    lastMillis = 0;
    score1 = 0;
    score2 = 0;
    scored = false;
    gameover = false;
    isPaused = false;
    
    resetBall();
    
    display.drawBitmap(0, 0, logo, 84, 48, BLACK);
    display.display();
    delay(5000);
    
}

void loop() {
    
    display.clearDisplay();
    
    game();
    
    //display.println(btnEnter.isPressed() ? "true" : "false");
    
    display.display();
    
    if(btnEnter.isPressed())
    {
        isPaused = !isPaused;
    }
    
}

void game() {
    
    if(!isPaused)
    {
        drawPaddles();
        drawScore();
        drawBall();
        drawBorders();
        
        if(!gameover && scored)
        {
            scored = false;
            resetBall();
        }
        
        if(gameover)
        {
            display.clearDisplay();
            //cada char tem 6 pixeis por 7 pixeis -> 54
            display.setCursor((display.width() / 2) - (54 / 2), (display.height() / 2) - (7 / 2));
            //display.print("GAME OVER");
            display.print(getString(msgGAMEOVER));
            display.display();
            
            if(btnEnter.isPressed())
            {
                score1 = 0;
                score2 = 0;
                
                isPaused = false;
                
                gameover = false;
            }
        }
    }
    else
    {
        display.clearDisplay();
        display.setCursor((display.width() / 2) - (30 / 2), (display.height() / 2) - (7 / 2));
        //display.print("PAUSE");
        display.print(getString(msgPAUSE));
        display.display();
    }
}

void drawBorders() {
    
    if(!gameover)
    {
        //Linha vertical central
        display.drawLine(display.width() / 2, 0, display.width() / 2, display.height(), BLACK);
        //Desenha o circulo central
        display.drawCircle(display.width() / 2 - ball.radius / 2 + 1, display.height() / 2 - ball.radius / 2 + 1, 3, BLACK);
    }
    
}

void drawBall() {
    
    if(!gameover)
    {
        currentMillis = millis();
        
        if(currentMillis - lastMillis > BALLDELAY)
        {
            //Verifica se a bola chegou a cima ou a baixo e envia a bola de volta
            if(ball.currentYpos <= 3 || ball.currentYpos + ball.radius >= display.height() - 1)
            {
                if(ball.angleSign == -1)    //Bola a ir para baixo
                {
                    ball.angleSign = 1;      //Envia a bola para cima
                }
                else
                {
                    ball.angleSign = -1;     //Envia a bola para baixo
                }
            }
            
            //Verifica se a bola chegou ao paddle1
            if(ball.currentXpos <= paddle1.width + ball.radius + 1 && ball.direction == 1)
            {   //Bola está ao alcance do paddle1
                if(ball.currentYpos + ball.radius >= paddle1.currentYpos && ball.currentYpos <= paddle1.currentYpos + paddle1.height)
                {
                    ball.direction = 0; //Bola bateu no paddle1 e mudou de direção
                }
            }
            
            //Verifica se a bola chegou ao paddle2
            if(ball.currentXpos >= display.width() - paddle2.width - ball.radius - 1 /*&& ball.currentXpos <= display.height()*/ && ball.direction == 0)
            {   //Bola está ao alcance do paddle2
                if(ball.currentYpos + ball.radius >= paddle2.currentYpos && ball.currentYpos <= paddle2.currentYpos + paddle2.height)
                {
                    ball.direction = 1; //1bola bateu no paddle2 e mudou de direção
                }
            }
            
            if(ball.currentXpos > 0 && ball.currentXpos + ball.radius < display.width())
            {   //Bola não está perto do extremo esquerdo ou do extremo direito
                if(ball.direction == 0)
                {
                    //Bola está a ir para a direita
                    ball.currentXpos += cos(ball.angle);
                    
                    if(ball.angleSign == -1)
                    {
                        ball.currentYpos -= sin(ball.angle);
                    }
                    else
                    {
                        ball.currentYpos += sin(ball.angle);
                    }
                }
                else
                {
                    //Bola está a ir para a esquerda
                    ball.currentXpos -= cos(ball.angle);
                    
                    if(ball.angleSign == -1)
                    {
                        ball.currentYpos -= sin(ball.angle);
                    }
                    else
                    {
                        ball.currentYpos += sin(ball.angle);
                    }
                }
            }
            else
            {
                if(!scored && !gameover)
                {
                    if(ball.direction == 0)
                    {
                        if(score1 < MAXTURNS - 1)
                        {
                            score1++;
                        }
                        else
                        {
                            gameover = true;
                        }
                    }
                    else
                    {
                        if(score2 < MAXTURNS - 1)
                        {
                            score2++;
                        }
                        else
                        {
                            gameover = true;
                        }
                    }
                    scored = true;
                }
            }
            
            lastMillis = millis();
        }
        
        //display.fillCircle(ball.lastXpos, ball.lastYpos, ball.radius, WHITE);
        display.fillCircle(ball.currentXpos, ball.currentYpos, ball.radius, BLACK);
    }
}

void drawScore() {
    
    if(!gameover)
    {
        display.setTextColor(BLACK, WHITE);
        display.setCursor(display.width() / 2 - 10, 0);
        display.print(score1);
        display.setCursor(display.width() / 2 + 6, 0);
        display.print(score2);
    }
}

void drawPaddles() {
    
    if(!gameover)
    {
        //   1024 -> display.height - paddle1.height
        //POT1PIN -> x
        paddle1.currentYpos = analogRead(POT1PIN) * (float)(display.height() - paddle1.height) / 1024;
        paddle2.currentYpos = analogRead(POT1PIN) * (float)(display.height() - paddle2.height) / 1024;
        display.fillRect(paddle1.currentXpos, paddle1.currentYpos, paddle1.width, paddle1.height, BLACK);
        display.fillRect(paddle2.currentXpos, paddle2.currentYpos, paddle2.width, paddle2.height, BLACK);
    }
}

void resetBall() {
    ball.currentXpos = display.width() / 2 - ball.radius / 2 + 1;
    ball.currentYpos = display.height() / 2 - ball.radius / 2 + 1;
    //ball.lastXpos = ball.currentXpos;
    //ball.lastYpos = ball.currentYpos;
    ball.angle = 45;    //Angulo entre a trajetória da bola e o eixo horizontal
    ball.angleSign = -1; //cima = +1, baixo = -1, horizontal = 0
    ball.direction = 1; //esquerda = 1, direita = 0
}



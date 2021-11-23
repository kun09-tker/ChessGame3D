#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "Cell.h"
#include<iostream>
#include<string>
using namespace std;

string ruleSwapPlayer(string s){
    if(s=="BULE") return "YELLOW";
    return "BULE"; 
}

bool ruleCheckBoundary(int x, int y){
    if(x<8 && x>=0 && y<8 && y>=0) return true;
    return false;
}

void ruleClear(Cell Board[8][8]){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            Board[i][j].move = false;
            Board[i][j].target = false;
        }
    }
}

void ruleSwapChess(Cell Board[8][8],int xcurrent, int ycurrent, int xmove, int ymove,int &x_down, int &y_down, GLfloat &shape){
    
    int tmp = 1;
    char player = Board[xcurrent][ycurrent].name[1];
    if(player=='Y') tmp = -1;

    if(ruleCheckBoundary(xmove-tmp,ymove)){
                
        if(Board[xmove-tmp][ymove].name[0]=='P' 
        && Board[xmove-tmp][ymove].name[1]!=player 
        && Board[xmove-tmp][ymove].name!="empty"){   // kiểm tra phía sau cóc tốt đối thủ hay không, có thì ăn
            
            shape = Board[xmove-tmp][ymove].shape;
            x_down = xmove-tmp;
            y_down = ymove;

            Board[xmove-tmp][ymove] = Cell();
        }
    }

    Board[xmove][ymove] = Board[xcurrent][ycurrent];
    Board[xcurrent][ycurrent] = Cell();
    ruleClear(Board);
}


void ruleDirection(Cell Board[8][8] ,int xlocal, int ylocal){
    int tmp = 1;
    char name_chess = Board[xlocal][ylocal].name[0];
    char player = Board[xlocal][ylocal].name[1];
    if(player=='Y') tmp = -1;
    if(name_chess=='R'){
        int numStep = 3;
        int d[numStep+1] = {0,1,0,-1};   

        for(int i=0; i<4 ;i++){

            int xd = d[i];
            int yd = d[numStep-i];
            
            while(true){

                if(ruleCheckBoundary(xlocal+xd,ylocal+yd)){
                    if(Board[xlocal+xd][ylocal+yd].name[1]!=player && Board[xlocal+xd][ylocal+yd].name!="empty"){
                        Board[xlocal+xd][ylocal+yd].target = true;
                        break;
                    }
                    else if(Board[xlocal+xd][ylocal+yd].name == "empty"){
                        Board[xlocal+xd][ylocal+yd].move = true;
                    }
                    else if(Board[xlocal+xd][ylocal+yd].name!="empty"){
                        break;
                    }
                }
                else break;

                xd += d[i];
                yd += d[numStep-i];
            }
        }
    }
    else if(name_chess=='N'){
        int numStep = 7;
        int d[numStep+1] = {-1,1,2,2,1,-1,-2,-2};      // x = x + d[i], y = y + d[7-i] là tọa độ bước đi của mã
        for(int i=0; i<8 ;i++){
            if(ruleCheckBoundary(xlocal+d[i],ylocal+d[numStep-i])){
                if(Board[xlocal+d[i]][ylocal+d[numStep-i]].name[1]!=player && Board[xlocal+d[i]][ylocal+d[numStep-i]].name!="empty"){
                    Board[xlocal+d[i]][ylocal+d[numStep-i]].target = true;
                }
                else if(Board[xlocal+d[i]][ylocal+d[numStep-i]].name == "empty"){
                    Board[xlocal+d[i]][ylocal+d[numStep-i]].move = true;
                }
            }
        }
    }
    else if(name_chess=='B'){
        int numStep = 3;
        int dx[numStep+1] = {1,1,-1,-1}; 
        int dy[numStep+1] = {1,-1,1,-1};

        for(int i=0; i<4 ;i++){

            int xd = dx[i];
            int yd = dy[i];
            
            while(true){   
                if(ruleCheckBoundary(xlocal+xd,ylocal+yd)){

                    if(Board[xlocal+xd][ylocal+yd].name[1]!=player && Board[xlocal+xd][ylocal+yd].name!="empty"){
                        Board[xlocal+xd][ylocal+yd].target = true;
                        break;
                    }
                    else if(Board[xlocal+xd][ylocal+yd].name == "empty"){
                        Board[xlocal+xd][ylocal+yd].move = true;
                    }
                    else if(Board[xlocal+xd][ylocal+yd].name!="empty"){
                        break;
                    }
                }
                else break;

                xd += dx[i];
                yd += dy[i];
            }
        }
    }
    else if(name_chess=='K'){
        int numStep = 3;
        int d[numStep+1] = {0,1,0,-1};   // x = x + d[i], y = y + d[3-i] là tọa độ bước đi của vua
        for(int i=0; i<4 ;i++){
            if(ruleCheckBoundary(xlocal+d[i],ylocal+d[numStep-i])){
                if(Board[xlocal+d[i]][ylocal+d[numStep-i]].name[1]!=player && Board[xlocal+d[i]][ylocal+d[numStep-i]].name!="empty"){
                    Board[xlocal+d[i]][ylocal+d[numStep-i]].target = true;
                }
                else if(Board[xlocal+d[i]][ylocal+d[numStep-i]].name == "empty"){
                    Board[xlocal+d[i]][ylocal+d[numStep-i]].move = true;
                }
            }
        }
    }
    else if(name_chess=='Q'){
        int numStep = 7;
        int dx[numStep+1] = {0,1,1,1,0,-1,-1,-1}; 
        int dy[numStep+1] = {-1,-1,0,1,1,1,0,-1};

        for(int i=0; i<8 ;i++){

            int xd = dx[i];
            int yd = dy[i];
            
            while(true){   
                if(ruleCheckBoundary(xlocal+xd,ylocal+yd)){

                    if(Board[xlocal+xd][ylocal+yd].name[1]!=player && Board[xlocal+xd][ylocal+yd].name!="empty"){
                        Board[xlocal+xd][ylocal+yd].target = true;
                        break;
                    }
                    else if(Board[xlocal+xd][ylocal+yd].name == "empty"){
                        Board[xlocal+xd][ylocal+yd].move = true;
                    }
                    else if(Board[xlocal+xd][ylocal+yd].name!="empty"){
                        break;
                    }
                }
                else break;

                xd += dx[i];
                yd += dy[i];
            }
        }
    }
    else if(name_chess=='P'){
        if(xlocal==1 || xlocal==6){ // tốt vị trí ban đầu được đi 1 - 2 ô 
            if(ruleCheckBoundary(xlocal+tmp,ylocal)  && Board[xlocal+tmp][ylocal].name == "empty") Board[xlocal+tmp][ylocal].move = true;
            if(ruleCheckBoundary(xlocal+2*tmp,ylocal)  && Board[xlocal+tmp][ylocal].name == "empty") Board[xlocal+2*tmp][ylocal].move = true;
        }
        if(ruleCheckBoundary(xlocal+tmp,ylocal+tmp)){
            if(Board[xlocal+tmp][ylocal+tmp].name[1]!=player && Board[xlocal+tmp][ylocal+tmp].name!="empty"){   // kiểm tra có phải gặp đối thử hay không
                Board[xlocal+tmp][ylocal+tmp].target = true;
            }
        }
        if(ruleCheckBoundary(xlocal+tmp,ylocal-tmp)){
            if(Board[xlocal+tmp][ylocal-tmp].name[1]!=player && Board[xlocal+tmp][ylocal-tmp].name!="empty"){
                Board[xlocal+tmp][ylocal-tmp].target = true;
            }
        }
        if(ruleCheckBoundary(xlocal+tmp,ylocal) && Board[xlocal+tmp][ylocal].name == "empty") Board[xlocal+tmp][ylocal].move = true; // tốt vị trí khác được đi 1 
                                                                                                                                    //ô nhưng không thể đi nếu vật cảng phía trước

        // Luật bắt tốt qua đường
        int d[2] = {1,-1};
        for(int i = 0; i<2 ;i++){
            if(ruleCheckBoundary(xlocal,ylocal+d[i])){
                if(Board[xlocal][ylocal+d[i]].name[0]=='P' 
                && Board[xlocal][ylocal+d[i]].name[1]!=player 
                && Board[xlocal][ylocal+d[i]].name!="empty"){   // kiểm tra có phải gặp đối thủ là tốt hay không
                    Board[xlocal+tmp][ylocal+d[i]].target = true;
                }
            }
        }

    }

}


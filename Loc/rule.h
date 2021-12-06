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

void ruleCheck(Cell Board[8][8], int x, int y, bool &check){
    if(Board[x][y].name[0] == 'K') check = true;
}

bool ruleCheckLineEmpty_Y(Cell Board[8][8], int x, int yfrom, int yto){

    int from = min(yfrom,yto);
    int to = max(yfrom,yto);

    //cout << from << " " << to << endl;

    for(int j=from; j<to; j++){
        //cout << "x=" << x << " " << "y=" << j << endl;
        if(Board[x][j].name!="empty" && j!=from) return false;
    }
    return true;
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

void ruleSwapChess(Cell Board[8][8], vector<GLfloat> &Eat_bule, vector<GLfloat> &Eat_yellow,
                    int xcurrent, int ycurrent,
                    int xmove, int ymove,
                    int &x_down, int &y_down, 
                    GLfloat &shape,
                    map<string,bool> &pawn_en_passant, bool &castling,
                    map<string,bool> &chess_move, bool &engame){
    
    int tmp = 1;
    char player = Board[xcurrent][ycurrent].name[1];
    
    chess_move[to_string(xcurrent)+to_string(ycurrent)] = true;

    if(player=='Y') tmp = -1;
    
    if(castling){

        if(xmove == 0){
            if(ymove == 2){
                Board[xmove][3] = Board[xmove][0];
                Board[xmove][0] = Cell();
            }
            if(ymove == 6){
                Board[xmove][5] = Board[xmove][7];
                Board[xmove][7] = Cell();
            }
        }

        if(xmove == 7){
            if(ymove == 1){
                Board[xmove][2] = Board[xmove][0];
                Board[xmove][0] = Cell();
            }

            if(ymove == 5){
                Board[xmove][4] = Board[xmove][0];
                Board[xmove][0] = Cell();
            }
        }

        castling = false;
    }

    if(abs(xcurrent-xmove)==2 && Board[xcurrent][ycurrent].name[0]=='P') 
        pawn_en_passant[to_string(xmove)+to_string(ymove)] = true;

    if(ruleCheckBoundary(xmove-tmp,ymove) && ruleCheckBoundary(xmove,ymove)){
    
        if(Board[xmove-tmp][ymove].name[0]=='P' 
        && Board[xmove-tmp][ymove].name[1]!=player 
        && Board[xmove-tmp][ymove].name!="empty"
        && Board[xmove][ymove].name=="empty"){   // kiểm tra phía sau con tốt đối thủ hay không, có thì ăn
            
            shape = Board[xmove-tmp][ymove].shape;
            x_down = xmove-tmp;
            y_down = ymove;

            if(Board[x_down][y_down].name[1] == 'B') Eat_bule.push_back(shape);
            else if (Board[x_down][y_down].name[1] == 'Y') Eat_yellow.push_back(shape);

            Board[xmove-tmp][ymove] = Cell();
        }
    }

    if(Board[xmove][ymove].name[1] == 'B') Eat_bule.push_back(Board[xmove][ymove].shape);
    else if (Board[xmove][ymove].name[1] == 'Y') Eat_yellow.push_back(Board[xmove][ymove].shape);

    Board[xmove][ymove] = Board[xcurrent][ycurrent];
    Board[xcurrent][ycurrent] = Cell();

    ruleClear(Board);
}


void ruleDirection(bool showMove,Cell Board[8][8] ,int xlocal, int ylocal, 
                    map<string,bool> &pawn_en_passant, map<string,bool> &chess_move, bool&castling,bool &check){
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
                        if(showMove) Board[xlocal+xd][ylocal+yd].target = true;
                        ruleCheck(Board,xlocal+xd,ylocal+yd,check);
                        break;
                    }
                    else if(Board[xlocal+xd][ylocal+yd].name == "empty"){
                        if(showMove) Board[xlocal+xd][ylocal+yd].move = true;
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
                    if(showMove) Board[xlocal+d[i]][ylocal+d[numStep-i]].target = true;
                    ruleCheck(Board,xlocal+d[i],ylocal+d[numStep-i],check);
                }
                else if(Board[xlocal+d[i]][ylocal+d[numStep-i]].name == "empty"){
                    if(showMove) Board[xlocal+d[i]][ylocal+d[numStep-i]].move = true;
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
                        if(showMove) Board[xlocal+xd][ylocal+yd].target = true;
                        ruleCheck(Board,xlocal+xd,ylocal+yd,check);
                        break;
                    }
                    else if(Board[xlocal+xd][ylocal+yd].name == "empty"){
                        if(showMove) Board[xlocal+xd][ylocal+yd].move = true;
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
        int numStep = 7;
        int dx[numStep+1] = {0,1,1,1,0,-1,-1,-1}; 
        int dy[numStep+1] = {-1,-1,0,1,1,1,0,-1};   // x = x + d[i], y = y + d[3-i] là tọa độ bước đi của vua
        for(int i=0; i<8 ;i++){
            if(ruleCheckBoundary(xlocal+dx[i],ylocal+dy[i])){
                if(Board[xlocal+dx[i]][ylocal+dy[i]].name[1]!=player && Board[xlocal+dx[i]][ylocal+dy[i]].name!="empty"){
                    if(showMove) Board[xlocal+dx[i]][ylocal+dy[i]].target = true;
                    ruleCheck(Board,xlocal+dx[i],ylocal+dy[i],check);
                }
                else if(Board[xlocal+dx[i]][ylocal+dy[i]].name == "empty"){
                    if(showMove) Board[xlocal+dx[i]][ylocal+dy[i]].move = true;
                }
            }
        }

        if(!chess_move[to_string(xlocal)+to_string(ylocal)]){
            int d[2] = {0,7};
            for(int i=0; i<2; i++){
                int add_move = (d[i] == 0) ? -2 : 2;
                if(!chess_move[to_string(xlocal)+to_string(d[i])] && ruleCheckLineEmpty_Y(Board,xlocal,ylocal,d[i])){
                    if(showMove) Board[xlocal][ylocal+add_move].move = true;
                    castling = true;
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
                        if(showMove) Board[xlocal+xd][ylocal+yd].target = true;
                        ruleCheck(Board,xlocal+xd,ylocal+yd,check);
                        break;
                    }
                    else if(Board[xlocal+xd][ylocal+yd].name == "empty"){
                        if(showMove) Board[xlocal+xd][ylocal+yd].move = true;
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
            for(int i=1; i<3 ;i++){
                if(ruleCheckBoundary(xlocal+i*tmp,ylocal)){
                    if(Board[xlocal+i*tmp][ylocal].name == "empty"){
                        if(showMove) Board[xlocal+i*tmp][ylocal].move = true;
                    }
                    else break;
                }
            }
        }
        if(ruleCheckBoundary(xlocal+tmp,ylocal+tmp)){
            if(Board[xlocal+tmp][ylocal+tmp].name[1]!=player && Board[xlocal+tmp][ylocal+tmp].name!="empty"){   // kiểm tra có phải gặp đối thử hay không
                if(showMove) Board[xlocal+tmp][ylocal+tmp].target = true;
                ruleCheck(Board,xlocal+tmp,ylocal+tmp,check);
            }
        }
        if(ruleCheckBoundary(xlocal+tmp,ylocal-tmp)){
            if(Board[xlocal+tmp][ylocal-tmp].name[1]!=player && Board[xlocal+tmp][ylocal-tmp].name!="empty"){
                if(showMove) Board[xlocal+tmp][ylocal-tmp].target = true;
                ruleCheck(Board,xlocal+tmp,ylocal-tmp,check);
            }
        }
        if(ruleCheckBoundary(xlocal+tmp,ylocal) && Board[xlocal+tmp][ylocal].name == "empty") 
            if(showMove) Board[xlocal+tmp][ylocal].move = true; // tốt vị trí khác được đi 1 
                                                                //ô nhưng không thể đi nếu vật cảng phía trước

        // Luật bắt tốt qua đường
        if((player=='B' && xlocal==4) || (player=='Y' && xlocal==3)){
            int d[2] = {1,-1};
            for(int i = 0; i<2 ;i++){
                if(ruleCheckBoundary(xlocal,ylocal+d[i])){
                    if(Board[xlocal][ylocal+d[i]].name[0]=='P' 
                    && Board[xlocal][ylocal+d[i]].name[1]!=player 
                    && Board[xlocal][ylocal+d[i]].name!="empty"
                    && pawn_en_passant[to_string(xlocal)+to_string(ylocal+d[i])]){   // kiểm tra có phải gặp đối thủ là tốt hay không
                        Board[xlocal+tmp][ylocal+d[i]].target = true;
                        ruleCheck(Board,xlocal+tmp,ylocal+d[i],check);
                    }
                }
            }
        }
        if(showMove) pawn_en_passant.clear();
    }

}


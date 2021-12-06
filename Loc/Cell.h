#include<iostream>
#include<string>
using namespace std;
class Cell
{
    public:
        GLfloat shape;   // hình dạng con cờ
        string name;    // tên cờ
        bool choose;    // đánh dấu ô chọn (xanh lá)
        bool target;    // đánh dấu những ô có thể tấn công đối thủ (đỏ)
        bool move;      // đánh dấu những ô có thể đi (xanh lá)
        Cell(string n = "empty",GLfloat s = 0){
            this->name = n;
            this->shape = s; 
            this->choose = false;
            this->target = false;
            this->move = false;
        }
};


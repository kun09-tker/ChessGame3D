class Chess : public Object {
    // Toạ độ trong bàn cờ
    int posX, posY;

    // Tọa độ có thể di chuyển đến
    std::vector<std::vector<int> > availableMovements;

public:
    Chess(){};
    ~Chess(){};

    // Tính toán các tọa độ có thể đi đến
    virtual void computeAvailableMovements(std::vector<Object *> &owner,
                                           std::vector<Object *> &opponent){};
};
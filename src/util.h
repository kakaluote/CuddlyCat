//
// Created by kakaluote on 2020/3/18.
//

#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <vector>
#include <iostream>
#include <memory>

#define COST_MAX 100000000

class Node{
public:
    Node():x(0),y(0),g(0),h(COST_MAX),parent(nullptr){}
    Node(int _x,int _y):x(_x),y(_y),g(0),h(COST_MAX),parent(nullptr){}
    Node(int _x,int _y,double _g,double _h):x(_x),y(_y),g(_g),h(_h),parent(nullptr){}
    ~Node(){
        parent = nullptr;
    }
    double GetCost() const {
        return g+h;
    }

    int x;
    int y;
    double g;
    double h;
    std::shared_ptr<Node> parent;
};

typedef std::shared_ptr<Node> NodePtr;
typedef std::vector<NodePtr> NodePtrList;
typedef std::vector<Node> NodeList;

void PrintVector(const NodeList& v);
void PrintVector(const NodePtrList& v);


bool IsSameLine(const Node& p1,const Node& p2,const Node& p3);


class Vec2{
public:
    Vec2():x(0),y(0){}
    Vec2(double _x,double _y):x(_x),y(_y){}

    double x;
    double y;
};
typedef std::vector<Vec2> PointList;
void PrintPointList(const PointList& v);

#endif //TEST_UTIL_H

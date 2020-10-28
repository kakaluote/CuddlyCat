//
// Created by kakaluote on 2020/3/18.
//
#include <cmath>
#include "util.h"

void PrintVector(const NodeList& v){
    for(auto &p : v){
        std::cout << p.x<<","<<p.y<<" cost "<<p.GetCost()<<" addr "<<&p;
        if(p.parent != nullptr){
            std::cout <<" || parent "<<p.parent->x<<","<<p.parent->y<<" cost "<<p.parent->GetCost()<<" addr "<<p.parent;
        }
        std::cout <<std::endl;
    }
}
void PrintVector(const NodePtrList& v){
    for(auto &p : v){
        std::cout << p->x<<","<<p->y<<" cost "<<p->GetCost()<<std::endl;
    }
}
void PrintPointList(const PointList& v){
    for(auto &p : v){
        std::cout << p.x<<","<<p.y<<std::endl;
    }
}

bool IsSameLine(const Node& p1,const Node& p2,const Node& p3){
    auto v1x = (double)p2.x-p1.x;
    auto v1y = (double)p2.y-p1.y;
    auto v2x = (double)p3.x-p2.x;
    auto v2y = (double)p3.y-p2.y;
    auto v1l = std::sqrt(v1x*v1x+v1y*v1y);
    v1x /= v1l;
    v1y /= v1l;
    auto v2l = std::sqrt(v2x*v2x+v2y*v2y);
    v2x /= v2l;
    v2y /= v2l;
    return std::abs(v1x*v2x+v1y*v2y) > 0.99;
}
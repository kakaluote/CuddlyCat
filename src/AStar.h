//
// Created by kakaluote on 2020/3/18.
//

#ifndef TEST_ASTAR_H
#define TEST_ASTAR_H

#include <vector>
#include "util.h"
#include <queue>
#include <memory>
#include <functional>


#define CELL_CLEAR 0
#define CELL_BLOCK 1
#define TINY_NUM 0.00001

class AStar {
public:
    AStar();

    AStar(int _w, int _h);

    AStar(int _w, int _h, double _cell_size);

    ~AStar();

    bool Init();

    void PrintMap();

    void ClearBlock(double x, double y);

    void ClearBlock(int x, int y);

    void SetBlock(int x, int y);

    void SetBlock(const std::vector<std::vector<int> > &blocks);

    void SetBlock(double x, double y);

    bool Solve(const Node &start, const Node &end, NodePtrList &path);

    void Flood();

    void PrintRoom();

    bool Solve(const Vec2 &start, const Vec2 &end, double radius, PointList &path);

	const std::vector<std::vector<int> >& get_map() const {
		return map;
	}

	bool IsBlock(const int &x, const int &y);

private:
    bool IsReachEnd(const NodePtr &p, const Node &end);

    void BuildPath(NodePtr &p, NodePtrList &path);

    bool IsBlock(const NodePtr &p);

    

    bool IsInMap(const NodePtr &p);

    double CalcCost(const Node &p1, const Node &p2);

    double CalcCost(const NodePtr &p1, const Node &p2);

    bool IsInCloseSet(const NodePtr &p);

    bool IsInOpenSet(const NodePtr &p);

    NodePtr OpenSetTop();

    void OpenSetPush(const NodePtr &p);

    bool IsUnitBlocked(int x, int y, int unit_size);

    void CheckNeibor(int x, int y, NodePtr &parent, const Node &start, const Node &end, int unit_size = 0);

    void CheckNeibors(NodePtr &p, const Node &start, const Node &end, int unit_size = 0);

    void ResetFlag();

    void ResetMap();

    bool IsBlock(const Node &p);

    bool IsInMap(const Node &p);

    bool IsInMap(const int &x, const int &y);

    bool IsAtCross(double x);

    void LineIntersectCell(const Node &p1, const Node &p2, NodeList &cells);

    void AddCell(const Node &p1, const Node &p2, const Node &p, NodeList &cells);

    bool IsLineBlocked(const Node &p1, const Node &p2, int unit_size = 0);

    bool Search(const Node &start, const Node &end, NodePtrList &path, int unit_size = 0);

    void SmoothPath(NodePtrList &path);

    void SmoothPath(const Vec2 &start, const Vec2 &end, NodePtrList &tmp_path, PointList &path, int unit_size = 0);

    bool IsLineBlocked(const Vec2 &p1, const Vec2 &p2, int unit_size = 0);

    bool TiltLineBlocked1(double x1, double y1, double x2, double y2);

    bool TiltLineBlocked2(double x1, double y1, double x2, double y2);

    void Flood(int i, int j, int mark);

    bool CanFlood(int i, int j);

    bool FindSeedAndFlood(int mark);

    NodeList FindNearestOutsideNodes(const Node &p, int len, int mark, int unit_size = 0);

    bool RefineEndPoint(const Node &start, const Node &end, Node &refine_end, int unit_size = 0);
	bool RefineEndPointReal(const Node &start, const Node &end, Node &refine_end, int unit_size=0);

    bool IsAllSameMark(int x, int y, int mark, int unit_size);

    void MakeInsideMap(Node &node);

    void MakeInsideMapReal(Vec2 &node, double radius = 0.5);

	void ResetRoomFlag();

private:
    int w;
    int h;
    double cell_size;
    std::vector<std::vector<int> > map;
    std::vector<std::vector<int> > flag;
    std::vector<std::vector<int> > room_flag;
    NodePtrList close_set;
    NodePtrList open_set;
    std::function<bool(const NodePtr &, const NodePtr &)> cmp;


};


#endif //TEST_ASTAR_H

//
// Created by kakaluote on 2020/3/18.
//

#include "AStar.h"
#include <cmath>
#include <iostream>
#include <algorithm>

AStar::AStar() : w(0), h(0), cell_size(1) {
    Init();
}

AStar::~AStar() {
}

AStar::AStar(int _w, int _h) : w(_w), h(_h), cell_size(1) {
    Init();
}

AStar::AStar(int _w, int _h, double _cell_size) : w(_w), h(_h), cell_size(_cell_size) {
    Init();
}

bool AStar::Init() {
    for (int i = 0; i < w; i++) {
        map.emplace_back(h);
        flag.emplace_back(h);
        room_flag.emplace_back(h);
    }
    cmp = [](const NodePtr &x, const NodePtr &y) { return x->GetCost() > y->GetCost(); };
    return true;
}

void AStar::ClearBlock(int x, int y) {
    if (IsInMap(x, y)) {
        map[x][y] = CELL_CLEAR;
    }
}

void AStar::ClearBlock(double x, double y) {
    int i = std::floor(x / cell_size);
    int j = std::floor(y / cell_size);
    if (IsInMap(i, j)) {
        map[i][j] = CELL_CLEAR;
    }
}

void AStar::SetBlock(int x, int y) {
    if (IsInMap(x, y)) {
        map[x][y] = CELL_BLOCK;
    }
}

void AStar::SetBlock(double x, double y) {
    int i = std::floor(x / cell_size);
    int j = std::floor(y / cell_size);
    if (IsInMap(i, j)) {
        map[i][j] = CELL_BLOCK;
    }
}

void AStar::SetBlock(const std::vector<std::vector<int> > &blocks) {
    for (auto &block : blocks) {
        SetBlock(block[0], block[1]);
    }
}

void AStar::PrintMap() {
    for (auto &i : map) {
        for (int j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}

void AStar::ResetMap() {
    for (auto &i : map) {
        for (int &j : i) {
            j = 0;
        }
    }
}

void AStar::ResetFlag() {
    for (auto &i : flag) {
        for (int &j : i) {
            j = 0;
        }
    }
}

void AStar::ResetRoomFlag() {
	for (auto &i : room_flag) {
		for (int &j : i) {
			j = 0;
		}
	}
}

inline bool AStar::IsReachEnd(const NodePtr &p, const Node &end) {
    return p->x == end.x && p->y == end.y;
}

void AStar::BuildPath(NodePtr &p, NodePtrList &path) {
    while (p != nullptr) {
        path.push_back(p);
        p = p->parent;
    }
    std::reverse(path.begin(), path.end());
}

inline bool AStar::IsBlock(const Node &p) {
    return IsBlock(p.x, p.y);
}

inline bool AStar::IsBlock(const NodePtr &p) {
    return IsBlock(p->x, p->y);
}

inline bool AStar::IsBlock(const int &x, const int &y) {
    if (!IsInMap(x, y)) {
        return false;
    }
    return map[x][y] == CELL_BLOCK;
}

inline bool AStar::IsInMap(const int &x, const int &y) {
    return x >= 0 && x < w && y >= 0 && y < h;
}

inline bool AStar::IsInMap(const Node &p) {
    return IsInMap(p.x, p.y);
}

inline bool AStar::IsInMap(const NodePtr &p) {
    return IsInMap(p->x, p->y);
}

inline double AStar::CalcCost(const Node &p1, const Node &p2) {
    double x_dist = std::fabs(p1.x - p2.x);
    double y_dist = std::fabs(p1.y - p2.y);
    return x_dist + y_dist + (1.414 - 2.0) * std::fmin(x_dist, y_dist);
}

inline double AStar::CalcCost(const NodePtr &p1, const Node &p2) {
    double x_dist = std::fabs(p1->x - p2.x);
    double y_dist = std::fabs(p1->y - p2.y);
    return x_dist + y_dist + (1.414 - 2.0) * std::fmin(x_dist, y_dist);
}

inline bool AStar::IsInCloseSet(const NodePtr &p) {
    return flag[p->x][p->y] == -1;
}

inline bool AStar::IsInOpenSet(const NodePtr &p) {
    return flag[p->x][p->y] == 1;
}

void AStar::OpenSetPush(const NodePtr &p) {
    open_set.push_back(p);
    std::push_heap(open_set.begin(), open_set.end(), cmp);
    flag[p->x][p->y] = 1;
}

NodePtr AStar::OpenSetTop() {
    std::pop_heap(open_set.begin(), open_set.end(), cmp);
    auto p = open_set.back();
    open_set.pop_back();
    flag[p->x][p->y] = 0;
    return p;
}

bool AStar::IsUnitBlocked(int x, int y, int unit_size) {
    if (unit_size == 0) {
        if (!IsInMap(x, y) || IsBlock(x, y)) {
            return true;
        }
    } else {
        for(int i=-unit_size;i<=unit_size;i++){
            for(int j=-unit_size;j<=unit_size;j++){
                if(i*i+j*j<=unit_size*unit_size){
                    if (!IsInMap(x + i, y + j) || IsBlock(x + i, y + j)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void AStar::CheckNeibor(int x, int y, NodePtr &parent, const Node &start, const Node &end, int unit_size) {
    auto p = std::make_shared<Node>(Node(x, y));
    if (IsUnitBlocked(p->x, p->y, unit_size) || IsInCloseSet(p) || IsInOpenSet(p)) {
        return;
    }
    p->parent = parent;

    if (p->x == parent->x || p->y == parent->y) {
        p->g = parent->g + 1;
        //        p->h = parent->h - 1;
    } else {
        p->g = parent->g + 1.414;
        //        p->h = parent->h - 1.414;
    }
    //    p->g = CalcCost(p, start);
    p->h = CalcCost(p, end);
    OpenSetPush(p);
}

// 检查相邻点，角点减少了需要检查的点
void AStar::CheckNeibors(NodePtr &p, const Node &start, const Node &end, int unit_size) {
    CheckNeibor(p->x, p->y + 1, p, start, end, unit_size);
    CheckNeibor(p->x, p->y - 1, p, start, end, unit_size);
    CheckNeibor(p->x + 1, p->y, p, start, end, unit_size);
    CheckNeibor(p->x - 1, p->y, p, start, end, unit_size);

    if (!IsBlock(p->x + 1, p->y) && !IsBlock(p->x, p->y + 1)) {
        CheckNeibor(p->x + 1, p->y + 1, p, start, end, unit_size);
    }
    if (!IsBlock(p->x + 1, p->y) && !IsBlock(p->x, p->y - 1)) {
        CheckNeibor(p->x + 1, p->y - 1, p, start, end, unit_size);
    }
    if (!IsBlock(p->x - 1, p->y) && !IsBlock(p->x, p->y + 1)) {
        CheckNeibor(p->x - 1, p->y + 1, p, start, end, unit_size);
    }
    if (!IsBlock(p->x - 1, p->y) && !IsBlock(p->x, p->y - 1)) {
        CheckNeibor(p->x - 1, p->y - 1, p, start, end, unit_size);
    }
}

// A*搜索路径
bool AStar::Search(const Node &start, const Node &end, NodePtrList &path, int unit_size) {
    if (!IsInMap(start) || !IsInMap(end) || IsBlock(start) || IsBlock(end)) {
        std::cout << "out of map or in block" << std::endl;
        return false;
    }

    if (!IsLineBlocked(start, end, unit_size)) {
        path.push_back(std::make_shared<Node>(start));
        path.push_back(std::make_shared<Node>(end));
        return true;
    }

    ResetFlag();
    open_set.clear();
    close_set.clear();

    //    int step = 0;
    OpenSetPush(std::make_shared<Node>(Node(start.x, start.y, 0, 0)));
    while (!open_set.empty()) {
        auto p = OpenSetTop();
        //        step += 1;
        if (IsReachEnd(p, end)) {
            BuildPath(p, path);
            //            std::cout << step << std::endl;
            return true;
        }

        close_set.push_back(p);
        flag[p->x][p->y] = -1;

        CheckNeibors(p, start, end, unit_size);
    }
    //    std::cout << step << std::endl;

    std::cout << "can not find path" << std::endl;
    return false;
}

void AStar::SmoothPath(NodePtrList &path) {
    if (path.size() < 3) {
        return;
    }
    NodePtrList tmp_path(std::move(path));
    path.clear();
    int p1_i = 0;
    path.push_back(tmp_path[p1_i]);
    for (int i = 2; i < tmp_path.size(); i++) {
        if (IsSameLine(*tmp_path[p1_i], *tmp_path[i - 1], *tmp_path[i])) {
            continue;
        }
        if (IsLineBlocked(*tmp_path[p1_i], *tmp_path[i])) {
            p1_i = i - 1;
            path.push_back(tmp_path[p1_i]);
        }
    }
    path.push_back(tmp_path.back());
}

// 单位化格子点搜索路径
bool AStar::Solve(const Node &start, const Node &end, NodePtrList &path) {
    Node end_node = end;
    MakeInsideMap(end_node);
    Node real_end;
    auto ok = RefineEndPoint(start, end_node, real_end);
    if (!ok) {
        return false;
    }

    ok = Search(start, real_end, path);
    if (!ok) {
        return false;
    }

    SmoothPath(path);

    return true;
}

// 坐标在角点，即小数部分 = 0.5
// 可修改等于判断阈值，增大角点影响范围
inline bool AStar::IsAtCross(double x) {
    return std::abs(x - std::floor(x) - 0.5) < 0.001;
}

// 不去重，不判断起点终点，似乎耗时更短
void AStar::AddCell(const Node &p1, const Node &p2, const Node &p, NodeList &cells) {
    //    if ((p.x == p1.x && p.y == p1.y) || (p.x == p2.x && p.y == p2.y)) {
    //        return;
    //    }
    //    for (auto &c:cells) {
    //        if (p.x == c.x && p.y == c.y) {
    //            return;
    //        }
    //    }
    cells.push_back(p);
}

// 获取两点连线上经过的格子，角点覆盖4个格子
void AStar::LineIntersectCell(const Node &p1, const Node &p2, NodeList &cells) {
    auto dx = std::abs(p1.x - p2.x);
    auto dy = std::abs(p1.y - p2.y);

    if (dx < TINY_NUM) {
        for (int i = std::min(p1.y, p2.y) + 1; i < std::max(p1.y, p2.y); i++) {
            cells.emplace_back(p1.x, i);
        }
    } else if (dy < TINY_NUM) {
        for (int i = std::min(p1.x, p2.x) + 1; i < std::max(p1.x, p2.x); i++) {
            cells.emplace_back(i, p1.y);
        }
    } else if (dx >= dy) {
        int x1 = p1.x;
        int y1 = p1.y;
        int x2 = p2.x;
        int y2 = p2.y;
        if (p1.x > p2.x) {
            x1 = p2.x;
            y1 = p2.y;
            x2 = p1.x;
            y2 = p1.y;
        }

        auto fx = [x1, y1, x2, y2](double x) { return (double) (y2 - y1) * (x - x1) / (double) (x2 - x1) + y1; };

        std::vector<double> inter_pts;
        double l = 0.5;
        while (x1 + l < x2) {
            inter_pts.push_back(fx(x1 + l));
            l += 1;
        }

        for (int i = 0; i < inter_pts.size(); i++) {
            if (IsAtCross(inter_pts[i])) {
                AddCell(p1, p2, Node(x1 + i, std::floor(inter_pts[i])), cells);
                AddCell(p1, p2, Node(x1 + i, std::floor(inter_pts[i]) + 1), cells);
                AddCell(p1, p2, Node(x1 + i + 1, std::floor(inter_pts[i])), cells);
                AddCell(p1, p2, Node(x1 + i + 1, std::floor(inter_pts[i]) + 1), cells);
            } else {
                AddCell(p1, p2, Node(x1 + i, std::round(inter_pts[i])), cells);
                AddCell(p1, p2, Node(x1 + i + 1, std::round(inter_pts[i])), cells);
            }
        }
    } else if (dx < dy) {
        int x1 = p1.x;
        int y1 = p1.y;
        int x2 = p2.x;
        int y2 = p2.y;
        if (p1.y > p2.y) {
            x1 = p2.x;
            y1 = p2.y;
            x2 = p1.x;
            y2 = p1.y;
        }

        auto fx = [x1, y1, x2, y2](double y) { return (double) (x2 - x1) * (y - y1) / (double) (y2 - y1) + x1; };

        std::vector<double> inter_pts;
        double l = 0.5;
        while (y1 + l < y2) {
            inter_pts.push_back(fx(y1 + l));
            l += 1;
        }

        for (int i = 0; i < inter_pts.size(); i++) {
            if (IsAtCross(inter_pts[i])) {
                AddCell(p1, p2, Node(std::floor(inter_pts[i]), y1 + i), cells);
                AddCell(p1, p2, Node(std::floor(inter_pts[i]) + 1, y1 + i), cells);
                AddCell(p1, p2, Node(std::floor(inter_pts[i]), y1 + i + 1), cells);
                AddCell(p1, p2, Node(std::floor(inter_pts[i]) + 1, y1 + i + 1), cells);
            } else {
                AddCell(p1, p2, Node(std::round(inter_pts[i]), y1 + i), cells);
                AddCell(p1, p2, Node(std::round(inter_pts[i]), y1 + i + 1), cells);
            }
        }
    }
}

// 判断两点间是否有阻挡
// 与LineIntersectCell相同，提前检查并返回，以提高效率
bool AStar::IsLineBlocked(const Node &p1, const Node &p2, int unit_size) {
    return IsLineBlocked(Vec2(p1.x, p1.y), Vec2(p2.x, p2.y), unit_size);
}

bool AStar::TiltLineBlocked1(double x1, double y1, double x2, double y2) {
    auto fx = [x1, y1, x2, y2](double x) { return (double) (y2 - y1) * (x - x1) / (double) (x2 - x1) + y1; };
    // 根据直线方程计算两点间，与垂直网格线的交点
    std::vector<double> inter_pts;
    int floor_x1 = std::floor(x1);
    double l = x1 - floor_x1;
    while (x1 + l < x2) {
        inter_pts.push_back(fx(x1 + l));
        l += 1;
    }
    // 若为角点，添加4个格子
    // 不为角点，添加左右两个格子
    for (int i = 0; i < inter_pts.size(); i++) {
        if (IsAtCross(inter_pts[i])) {
            auto floor_y = std::floor(inter_pts[i]);
            if (IsBlock(floor_x1 + i, floor_y) ||
                IsBlock(floor_x1 + i, floor_y + 1) ||
                IsBlock(floor_x1 + i + 1, floor_y) ||
                IsBlock(floor_x1 + i + 1, floor_y + 1)) {
                return true;
            }
        } else {
            auto round_y = std::round(inter_pts[i]);
            if (IsBlock(floor_x1 + i, round_y) ||
                IsBlock(floor_x1 + i + 1, round_y)) {
                return true;
            }
        }
    }
    return false;
}

bool AStar::TiltLineBlocked2(double x1, double y1, double x2, double y2) {
    int floor_y1 = std::floor(y1);

    auto fx = [x1, y1, x2, y2](double y) { return (double) (x2 - x1) * (y - y1) / (double) (y2 - y1) + x1; };

    std::vector<double> inter_pts;
    double l = y1 - floor_y1;
    while (y1 + l < y2) {
        inter_pts.push_back(fx(y1 + l));
        l += 1;
    }

    for (int i = 0; i < inter_pts.size(); i++) {
        if (IsAtCross(inter_pts[i])) {
            auto floor_x = std::floor(inter_pts[i]);
            if (IsBlock(floor_x, floor_y1 + i) ||
                IsBlock(floor_x, floor_y1 + i + 1) ||
                IsBlock(floor_x + 1, floor_y1 + i) ||
                IsBlock(floor_x + 1, floor_y1 + i + 1)) {
                return true;
            }
        } else {
            auto round_x = std::round(inter_pts[i]);
            if (IsBlock(round_x, floor_y1 + i) ||
                IsBlock(round_x, floor_y1 + i + 1)) {
                return true;
            }
        }
    }
    return false;
}

// 任意点版本（单位化，非格子点）判断两点间是否有阻挡
bool AStar::IsLineBlocked(const Vec2 &p1, const Vec2 &p2, int unit_size) {
    auto dx = std::abs(p1.x - p2.x);
    auto dy = std::abs(p1.y - p2.y);
    auto near_p1 = Node(std::floor(p1.x), std::floor(p1.y));
    auto near_p2 = Node(std::floor(p2.x), std::floor(p2.y));

    if (dx < TINY_NUM) {
        // 垂直线
        for (int i = std::min(near_p1.y, near_p2.y) + 1; i < std::max(near_p1.y, near_p2.y); i++) {
            for (int j = -unit_size; j <= unit_size; j++) {
                if (IsBlock(near_p1.x + j, i)) {
                    return true;
                }
            }
        }
    } else if (dy < TINY_NUM) {
        // 水平线
        for (int i = std::min(near_p1.x, near_p2.x) + 1; i < std::max(near_p1.x, near_p2.x); i++) {
            for (int j = -unit_size; j <= unit_size; j++) {
                if (IsBlock(i, near_p1.y + j)) {
                    return true;
                }
            }
        }
    } else if (dx >= dy) {
        // 倾斜小于45度
        auto x1 = p1.x;
        auto y1 = p1.y;
        auto x2 = p2.x;
        auto y2 = p2.y;
        if (p1.x > p2.x) {
            x1 = p2.x;
            y1 = p2.y;
            x2 = p1.x;
            y2 = p1.y;
        }

        for (int j = -unit_size; j <= unit_size; j++) {
            if (TiltLineBlocked1(x1, y1 + j, x2, y2 + j))
                return true;
        }

    } else if (dx < dy) {
        // 倾斜大于45度
        auto x1 = p1.x;
        auto y1 = p1.y;
        auto x2 = p2.x;
        auto y2 = p2.y;
        if (p1.y > p2.y) {
            x1 = p2.x;
            y1 = p2.y;
            x2 = p1.x;
            y2 = p1.y;
        }

        for (int j = -unit_size; j <= unit_size; j++) {
            if (TiltLineBlocked2(x1 + j, y1, x2 + j, y2))
                return true;
        }
    }
    return false;
}

void AStar::SmoothPath(const Vec2 &start, const Vec2 &end, NodePtrList &tmp_path, PointList &path, int unit_size) {
    path.push_back(start);
    tmp_path.emplace_back(std::make_shared<Node>(Node(end.x, end.y)));
    auto p1 = std::make_shared<Node>(Node(start.x, start.y));
    for (int i = 1; i < tmp_path.size(); i++) {
        if (IsSameLine(*p1, *tmp_path[i - 1], *tmp_path[i])) {
            continue;
        }
        if (IsLineBlocked(*p1, *tmp_path[i], unit_size)) {
            p1 = tmp_path[i - 1];
            path.emplace_back(p1->x, p1->y);
        }
    }
    path.push_back(end);
}

// 任意两点寻路（非单位化的格子点）
// 注意：假定起点总是在地图内，且不在阻挡内
// 路径不会经过阻挡拐点，不可从(0,1)到(1,0)
//   0 1
// 0 # o
// 1 o #
bool AStar::Solve(const Vec2 &start, const Vec2 &end, double radius, PointList &path) {
    // 单位化
    auto norm_start = Vec2(start.x / cell_size, start.y / cell_size);
    auto norm_end = Vec2(end.x / cell_size, end.y / cell_size);
    double norm_radius = radius / cell_size;
    // 不在map内的点，映射到map内
    MakeInsideMapReal(norm_end, norm_radius);

	int unit_size = std::round(norm_radius*2);
	if (unit_size > 0) {
		unit_size -= 1;
	}

    // 可直达
    if (!IsLineBlocked(norm_start, norm_end, unit_size)) {
        path.push_back(start);
        path.emplace_back(norm_end.x * cell_size, norm_end.y * cell_size);
        return true;
    }

    // 对齐到格子点
    Node near_start(std::floor(norm_start.x), std::floor(norm_start.y));
    Node near_end(std::floor(norm_end.x), std::floor(norm_end.y));

    // 检查终点是否不可达（被阻挡包围），若不可达，则查找距离最近的可达点
    // 找到若干可达点，距离 = dist(起点,可达点)+dist(可达点,终点)
    // 参数1：起点，参数2：终点，参数3：最近可达点
    Node real_end;
    auto ok = RefineEndPointReal(near_start, near_end, real_end, unit_size);
    if (!ok) {
        return false;
    }

    // 搜索路径
    NodePtrList tmp_path;
    ok = Search(near_start, real_end, tmp_path, unit_size);
    if (!ok) {
        return false;
    }

	


    // 路径平滑，相邻3点，第1和第3可直达，则去掉第2点
    // 非格子点版本，起点终点（非格子点）参与平滑
    if (real_end.x == near_end.x && real_end.y == near_end.y) {
        // 终点未被修改，终点为对齐前的点
        SmoothPath(norm_start, norm_end, tmp_path, path, unit_size);
    } else {
        // 因为终点不可达（被阻挡包围）而被修改，终点为被修改后的点，就是寻路终点
        // 去掉最后一点，SmoothPath中会再加上
        tmp_path.pop_back();
        SmoothPath(norm_start, Vec2(real_end.x, real_end.y), tmp_path, path, unit_size);
    }


    // 返回任意点路径（非单位化）
    for_each(path.begin(), path.end(), [this](Vec2 &val) -> void {
        val.x *= cell_size;
        val.y *= cell_size;
    });
    return true;
}

void AStar::PrintRoom() {
    for (auto &i : room_flag) {
        for (int j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}

bool AStar::CanFlood(int i, int j) {
    if (!IsInMap(i, j) || IsBlock(i, j)) {
        return false;
    }
    if (room_flag[i][j] != 0) {
        return false;
    }
    return true;
}

// 由于不可经过阻挡拐点，故递归的向上下左右方向填充
void AStar::Flood(int i, int j, int mark) {
    std::vector<std::vector<int> > pts;
    pts.push_back(std::vector<int>{i, j});
    while (!pts.empty()) {
        auto p = pts.back();
        int x = p[0];
        int y = p[1];
        pts.pop_back();
        room_flag[x][y] = mark;
        if (CanFlood(x - 1, y)) {
            pts.push_back(std::vector<int>{x - 1, y});
        }
        if (CanFlood(x + 1, y)) {
            pts.push_back(std::vector<int>{x + 1, y});
        }
        if (CanFlood(x, y - 1)) {
            pts.push_back(std::vector<int>{x, y - 1});
        }
        if (CanFlood(x, y + 1)) {
            pts.push_back(std::vector<int>{x, y + 1});
        }
    }
}

bool AStar::FindSeedAndFlood(int mark) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (map[i][j] == CELL_CLEAR && room_flag[i][j] == 0) {
                Flood(i, j, mark);
                return true;
            }
        }
    }
    return false;
}

// 填充法，区分room（地图互相连通的区域为1个room），处于不同room的两点不可达
// room_flag中 0 表示阻挡,1,2,3... 表示不同room
void AStar::Flood() {
	ResetRoomFlag();
    int mark = 1;
    while (true) {
        auto trigger_flood = FindSeedAndFlood(mark);
        if (!trigger_flood) {
            break;
        }
        mark += 1;
    }
}


// 单位半径内都一样的mark,去掉4个角，更接近圆
bool AStar::IsAllSameMark(int x, int y, int mark, int unit_size) {
    if (unit_size == 0) {
        if (!IsInMap(x, y) || room_flag[x][y] != mark) {
            return false;
        }
    } else {
        for(int i=-unit_size;i<=unit_size;i++){
            for(int j=-unit_size;j<=unit_size;j++){
                if(i*i+j*j<=unit_size*unit_size){
                    if (!IsInMap(x + i, y + j) || room_flag[x + i][y + j] != mark) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

NodeList AStar::FindNearestOutsideNodes(const Node &p, int len, int mark, int unit_size) {
    NodeList ret;
    for (int i = 0; i < 2 * len + 1; i++) {
        int x = p.x - len + i;
        int y = p.y - len;
        if (IsAllSameMark(x, y, mark, unit_size)) {
            ret.emplace_back(x, y);
        }
        x = p.x - len + i;
        y = p.y + len;
        if (IsAllSameMark(x, y, mark, unit_size)) {
            ret.emplace_back(x, y);
        }
    }
    for (int i = 1; i < 2 * len; i++) {
        int x = p.x - len;
        int y = p.y - len + i;
        if (IsAllSameMark(x, y, mark, unit_size)) {
            ret.emplace_back(x, y);
        }
        x = p.x + len;
        y = p.y - len + i;
        if (IsAllSameMark(x, y, mark, unit_size)) {
            ret.emplace_back(x, y);
        }
    }
    return ret;
}

bool AStar::RefineEndPoint(const Node &start, const Node &end, Node &refine_end, int unit_size) {
    if (IsAllSameMark(end.x, end.y, room_flag[start.x][start.y], unit_size)) {
        refine_end = end;
        return true;
    }

    // 寻找最近可达点
    // 从终点开始，往外一圈一圈的查找，直到一圈中包含与起点同room的点
    // 搜索最大圈半径是抵达地图边界的最长距离
    auto lens = {end.x, end.y, w - end.x - 1, h - end.y - 1};
    int max_len = *std::max_element(lens.begin(), lens.end());
    for (int i = 0; i < max_len; i++) {
        auto nodes = FindNearestOutsideNodes(end, i + 1, room_flag[start.x][start.y], unit_size);
        if (!nodes.empty()) {
            double min_dist = COST_MAX;
            for (auto &node : nodes) {
                auto dist = CalcCost(node, start) + CalcCost(node, end);
                if (dist < min_dist) {
                    min_dist = dist;
                    refine_end = node;
                }
            }
            return true;
        }
    }

    std::cout << "can not find good end point" << std::endl;
    return false;
}


bool AStar::RefineEndPointReal(const Node &start, const Node &end, Node &refine_end, int unit_size) {
	if (IsAllSameMark(end.x, end.y, room_flag[start.x][start.y], unit_size)) {
		refine_end = end;
		return true;
	}

	// 寻找最近可达点
	// 从终点开始，往外一圈一圈的查找，直到一圈中包含与起点同room的点
	// 搜索最大圈半径是抵达地图边界的最长距离
	auto lens = { end.x, end.y, w - end.x - 1, h - end.y - 1 };
	int max_len = *std::max_element(lens.begin(), lens.end());
	for (int i = 0; i < max_len; i++) {
		auto nodes = FindNearestOutsideNodes(end, i + 1, room_flag[start.x][start.y], unit_size);
		if (!nodes.empty()) {
			double min_dist = COST_MAX;
			for (auto &node : nodes) {
				auto dist = CalcCost(node, start) + CalcCost(node, end);
				if (dist < min_dist) {
					min_dist = dist;
					refine_end = node;
				}
			}
			return true;
		}
	}

	std::cout << "can not find good end point" << std::endl;
	return false;
}


void AStar::MakeInsideMap(Node &node) {
    if (node.x < 0) {
        node.x = 0;
    }
    if (node.x >= w) {
        node.x = w - 1;
    }
    if (node.y < 0) {
        node.y = 0;
    }
    if (node.y >= h) {
        node.y = h - 1;
    }
}

void AStar::MakeInsideMapReal(Vec2 &node, double radius) {
    if (node.x < 0 + radius) {
        node.x = 0 + radius;
    }
    if (node.x >= w - radius) {
        node.x = w - radius;
    }
    if (node.y < 0 + radius) {
        node.y = 0 + radius;
    }
    if (node.y >= h - radius) {
        node.y = h - radius;
    }
}
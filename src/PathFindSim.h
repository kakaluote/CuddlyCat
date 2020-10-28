#ifndef PATH_FIND_SIM_H
#define PATH_FIND_SIM_H

#include "AStar.h"
#include "b2_math.h"


class PathFindSim
{
public:
	PathFindSim(int _grid_w, int _grid_h, double _cell_size);

	b2Vec2 get_unit_pos() const { return unit_pos; }
	void set_unit_pos(b2Vec2 val) { unit_pos = val; }
	double get_unit_move_speed() const { return unit_move_speed; }
	void set_unit_move_speed(double val) { unit_move_speed = val; }
	double get_unit_radius() const { return unit_radius; }
	void set_unit_radius(double val) { unit_radius = val; }

	const std::vector<std::vector<int> >& get_map() const {
		return astar.get_map();
	}

	b2Vec2 get_target_pos() const { return target_pos; }
	void move_to(b2Vec2 val);

	void update();
	void set_block(double x, double y);
	void clear_block(double x, double y);
	bool is_block(double x, double y) {
		return astar.IsBlock(x, y);
	}

	int get_grid_w() const { return grid_w; }
	int get_grid_h() const { return grid_h; }
	const PointList& get_path() const { return path; }
	bool Is_reach_target_pos() const { return is_reach_target_pos; }
private:
	int grid_w;
	int grid_h;
	double cell_size;
	AStar astar;
	b2Vec2 unit_pos;
	double unit_move_speed;
	double unit_radius;
	b2Vec2 target_pos;

	bool is_reach_target_pos;

	PointList path;
	int cur_way_point_idx;
};


#endif
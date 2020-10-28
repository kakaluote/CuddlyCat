#include "PathFindSim.h"

#include "PathFindSim.h"

PathFindSim::PathFindSim(int _grid_w,int _grid_h,double _cell_size):
	grid_w(_grid_w),grid_h(_grid_h),cell_size(_cell_size), is_reach_target_pos(true)
{
	astar = AStar(grid_w, grid_h, cell_size);
	unit_move_speed = 0.1;
	unit_radius = 0.5;
}

void PathFindSim::set_block(double x, double y) {
	astar.SetBlock(x, y);
	astar.Flood();
}
void PathFindSim::clear_block(double x, double y) {
	astar.ClearBlock(x, y);
	astar.Flood();
}

void PathFindSim::move_to(b2Vec2 val) {
	path.clear();
	cur_way_point_idx = 1;
	bool ok = astar.Solve(Vec2(unit_pos.x, unit_pos.y), Vec2(val.x, val.y), unit_radius, path);
	if (ok) {
		is_reach_target_pos = false;
		target_pos = b2Vec2(path.back().x, path.back().y);
	}
	else {
		is_reach_target_pos = true;
	}
}

void PathFindSim::update()
{
	if (!is_reach_target_pos) {
		if (cur_way_point_idx < get_path().size()) {
			auto& p = path[cur_way_point_idx];
			auto move_direct = (b2Vec2(p.x,p.y) - unit_pos);
			double dist = move_direct.Normalize();
			double move_dist = unit_move_speed;
			if (move_dist > dist) {
				move_dist = dist;
			}
			unit_pos = unit_pos + move_direct * move_dist;
			if (dist < 0.001) {
				cur_way_point_idx++;
			}
		}
		else {
			is_reach_target_pos = true;
		}
	}
	


}

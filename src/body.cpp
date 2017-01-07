#include "body.h"

#include <numeric>
#include <algorithm>
#include <iostream>
namespace adder {

std::vector<glm::vec2> Body::_gen_sep_axes() {
	std::vector<glm::vec2> sep_axes;
	auto verts = _poly.vertices();
	for(int i = 0; i < verts.size() - 1; ++i) {
		glm::vec2 line_vec = {verts[i + 1][0] - verts[i][0], verts[i + 1][1] - verts[i][1]};
		glm::vec2 perp_vec = {line_vec[1], -line_vec[0]};
		sep_axes.push_back(glm::normalize(perp_vec));
	}
	glm::vec2 line_vec = {verts.front()[0] - verts.back()[0], verts.front()[1] - verts.back()[1]};
	glm::vec2 perp_vec = {line_vec[1], -line_vec[0]};
	sep_axes.push_back(glm::normalize(perp_vec));
	return sep_axes;
}

Body::Body(float mass, float moment_of_inertia, float charge, Polygon poly) :
	_mass{mass},
	_I{moment_of_inertia},
	_q{charge},
	_poly{poly} {
	_seperation_axes = _gen_sep_axes();
}

Body::Body(float mass, float moment_of_inertia, float charge, std::vector<glm::vec2> vertices, glm::vec2 pos) :
	_mass{mass},
	_I{moment_of_inertia},
	_q{charge},
	_pos{pos},
	_poly{vertices, pos} {
	_seperation_axes = _gen_sep_axes();
}

void Body::update(float dt) {
	glm::vec2 net_force = std::accumulate(_forces.begin(), _forces.end(), glm::vec2{0.f, 0.f});
	float net_torque = std::accumulate(_torques.begin(), _torques.end(), 0.f);
	_pos += _vel*dt;
	if(_mass > 0)
		_vel += (net_force / _mass)*dt;
	_ang += _ang_v*dt;
	if(_I > 0)
		_ang_v += (net_torque / _I)*dt;

	_poly.set_position({_pos[0], _pos[1], 0.f, 1.f});
	if(_forces.size() > 100) {
		_forces.clear();
		_forces.reserve(100);
		_forces.push_back(net_force);
	}
	if(_torques.size() > 100) {
		_torques.clear();
		_torques.reserve(100);
		_torques.push_back(net_torque);
	}
}

void Body::add_force(const glm::vec2 &force) {
	_forces.push_back(force);
}

void Body::add_torque(const float &torque) {
	_torques.push_back(torque);
}

void Body::set_velocity(const glm::vec2 &vel) {
	_vel = vel;
}

void Body::set_angular_velocity(const float &ang_v) {
	_ang_v = ang_v;
}

} // adder

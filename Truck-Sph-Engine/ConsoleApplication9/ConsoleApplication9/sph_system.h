/** File:		sph_system.h
** Author:		Dongli Zhang
** Contact:	dongli.zhang0129@gmail.com
**
** Copyright (C) Dongli Zhang 2013
**
** This program is free software;  you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY;  without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
** the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program;  if not, write to the Free Software 
** Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef __SPHSYSTEM_H__
#define __SPHSYSTEM_H__

#include "sph_type.h"

class Particle
{
public:
	uint id;
	float3 pos;
	float3 pos_prev;
	float3 vel;
	float3 acc;
	float3 ev;


	float dens;
	float pres;

	float surf_norm;
	int index ;

	Particle *next;

	int particles_around ; 
};

class SPHSystem
{
public:
	uint max_particle;
	uint num_particle;
	uint actual_particles_number;
	float kernel;
	float mass;
	int sections ;
	bool walls ; 

	float3 world_size;
	float cell_size;
	uint3 grid_size;
	float3 origin ; 
	uint tot_cell;


	float3 dw;
	float3 w; 
	float3 target ; 

	float3 gravity;
	float wall_damping;
	float rest_density;
	float gas_constant;
	float viscosity;
	float time_step;
	float surf_norm;
	float surf_coe;
	float wall_width; 

	float poly6_value;
	float spiky_value;
	float visco_value;

	float grad_poly6;
	float lplc_poly6;

	float kernel_2;
	float self_dens;
	float self_lplc_color;

	Particle *mem;
	Particle **cell;
	Particle*** particle_cell;

	uint sys_running;
	int max_paricles_around ; 

public:
	SPHSystem();
	~SPHSystem();
	void animation();
	void init_particles();
	void init_particles2();
	void init_system(); 
	void add_particle(float3 pos, float3 vel);
	void freeSph(); 
	void build_table();
	void comp_dens_pres();
	void comp_force_adv();
	void advection();
	void reflect(); 

	float capsule_radius; 
	float cT ; 
	float3 p0 , p1 , q  , p2 , p3 , p4 , p5; 
	
	float3 p0p1 ; 
	float3 p2p0; 
	plane planes[4]; 


	float3 truck_acc;

	float vec_len_2(float3 a , float3 b);
	float vec_len(float3 a, float3 b);
	void calc_q(float3 p);

	float capsule_penetration_test(float3 p) ;

	void adjust_contact_point(float3& pos) ;
	float3 unit_surface_normal(float3 p);

	void reflect2() ;
	void set_params(param paramas);

	void set_default_paramas();

	void adjust_with_plane(plane& pl , Particle* p );

	void update_particle(int i , float3 pos); 

	
private:
	int3 calc_cell_pos(float3 p);
	uint calc_cell_hash(int3 cell_pos);
};

#endif

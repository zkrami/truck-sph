/** File:		sph_type.h
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

#ifndef __SPHTYPE_H__
#define __SPHTYPE_H__

typedef unsigned int uint;
#include <math.h>

struct float3
{
	float x;
	float y;
	float z;
	float3(){  x = y = z = 0 ; }
	float3( float x , float y ,float z) :x(x) , y(y) ,z(z) {} 

	float3 cross(float3& v) const {
		float3 u = *this; 
		float3 res ; 
		res.x = (u.y * v.z - u.z * v.y); 
		res.y = -(u.x * v.z - u.z * v.x); 
		res.z = (u.x * v.y - u.y * v.x); 
		return res; 
	}

	float3 operator +(float3 &op) const {
		return float3( op.x + this->x , op.y + this->y , op.z + this->z ) ; 
	}
	void operator += (float3& op) {
		*this = *this + op ; 
	}

	float3 operator - (float3& op) const{
		return  *this + float3(-op.x , -op.y , -op.z) ; 
	}


	float3 operator *(float op) const {
		return float3( x * op , y * op , z * op) ; 
	}

};

struct double3
{
	double x;
	double  y;
	double z;
};

struct int3
{
	int x;
	int y;
	int z;
	int3(){ x = y = z = 0 ; } 
};

struct uint3
{
	uint x;
	uint y;
	uint z;
	uint3(){ x = y = z = 0 ; } 
};


struct param {

	float3 gravity ; 

	float rest_density;
	float gas_constant;
	float viscosity;
	float time_step;
	float surf_norm;
	float surf_coe; 
	uint actual_particles_number;
	float kernel;
	float mass;
	float capsule_radius ; 
	float cT ;
	bool walls; 

	

};

struct plane{

	public:
	float a , b ,c , d ;
	static plane create_plane(float3& v , float3& p ){
		plane t; 
		t.a = v.x; 
		t.b = v.y; 
		t.c = v.z ;
		t.d = -1 * ( v.x *  p.x  + v.y * p.y + v.z * p.z ) ; 
		return t; 
	}
	float evaluate(float3& p){
		return a * p.x + b * p.y + c * p.z + d; 
	}

	float3 projection(float3& p){
		float t = -d - a * p.x - b *p.y - c * p.z ; 
		t /= (a * a + b * b + c * c ) ; 
		float3 res ; 
		res.x  = p.x + t * a; 
		res.y = p.y + t * b ; 
		res.z = p.z + t * c ;

		return res; 
	}
	float distance(float3& p){
		
		return fabs(signed_distance(p)); 
	}
	float signed_distance(float3& p){
		float dist = a * p.x + b * p.y + c * p.z + d; 		
		dist /= sqrt(a *a + b * b + c * c ) ; 
		
		return dist;
	}

};
#endif

#include "stdafx.h"
#include "sph_system.h"
#include "sph_header.h"
void SPHSystem::init_system(){
	float th = vec_len(p0 , p1); 
	float max_len = th + 2 * capsule_radius ; 

	max_len *= 4; 
	wall_width = 0.025f; 
	sections = 8 ; 



	max_particle=12800;
	num_particle=0;



	world_size.x= max_len; 
	world_size.y=max_len; 
	world_size.z=max_len; 

	cell_size=kernel;

	grid_size.x=(uint)ceil(world_size.x/cell_size);
	grid_size.y= (uint)ceil(world_size.y/cell_size);
	grid_size.z= (uint)ceil(world_size.z/cell_size);
	tot_cell=grid_size.x*grid_size.y*grid_size.z;



	poly6_value=315.0f/(64.0f * PI * pow(kernel, 9));; // rho 
	spiky_value=-45.0f/(PI * pow(kernel, 6)); // pressure 
	visco_value=45.0f/(PI * pow(kernel, 6)); // viscosity 

	grad_poly6=-945/(32 * PI * pow(kernel, 9)); // gradient 
	lplc_poly6=-945/(8 * PI * pow(kernel, 9)); // laplacian 

	kernel_2=kernel*kernel;
	self_dens=mass*poly6_value*pow(kernel, 6);
	self_lplc_color=lplc_poly6*mass*kernel_2*(0-3/4*kernel_2);

	mem=(Particle *)malloc(sizeof(Particle)*max_particle);

	max_paricles_around = 1001 ; 

	particle_cell = (Particle ***)malloc(sizeof(Particle **)*max_paricles_around);

	for(int i =  0 ; i < max_paricles_around ; i ++) 
		particle_cell[i] = (Particle **) malloc(sizeof(Particle*) * max_particle); 


	cell=(Particle **)malloc(sizeof(Particle *)*tot_cell);

	sys_running=0;

	printf("Initialize SPH:\n");
	printf("World Width : %f\n", world_size.x);
	printf("World Height: %f\n", world_size.y);
	printf("World Length: %f\n", world_size.z);
	printf("Cell Size  : %f\n", cell_size);
	printf("Grid Width : %u\n", grid_size.x);
	printf("Grid Height: %u\n", grid_size.y);
	printf("Grid Length: %u\n", grid_size.z);
	printf("Total Cell : %u\n", tot_cell);
	printf("Poly6 Kernel: %f\n", poly6_value);
	printf("Spiky Kernel: %f\n", spiky_value);
	printf("Visco Kernel: %f\n", visco_value);
	printf("Self Density: %f\n", self_dens);


	init_particles2(); 

}


void SPHSystem::set_default_paramas()
{
	cT = 0; 
	capsule_radius = 0.1f; 

	p0.x = capsule_radius ; 
	p0.y = capsule_radius ; 
	p0.z = capsule_radius ; 

	p1.x = capsule_radius + 1 ; 
	p1.y = capsule_radius ; 
	p1.z = capsule_radius ; 

	p2 = p0 ;
	p2.z += 0.1f ; 

	p3 = p1 ;
	p3.x = 0.3f ; 


	p4 = p1 ; 
	p4.x = 0.9f; 


	p5 = p1 ; 

	p5.x = 0.6f ; 

		p0p1.x = p1.x - p0.x; 
	p0p1.y = p1.y - p0.y; 
	p0p1.z = p1.z - p0.z; 






	
	p2p0.x = p2.x - p0.x ; 
	p2p0.y = p2.y - p0.y ;
	p2p0.z = p2.z - p0.z; 





	planes[0] = plane::create_plane(p2p0 , p0); 



	planes[1] = plane::create_plane(p0p1 , p3); 

	planes[2] = plane::create_plane(p0p1 , p4);

	planes[3] = plane::create_plane(p0p1 , p5); 


	
	





	actual_particles_number = 1000 ; 
	kernel=0.028f;
	mass=0.02f;

	gravity.x=0.0f; 
	gravity.y=-9.8f;
	gravity.z=0.0f;
	wall_damping=-0.5f;
	rest_density=998.29f;
	gas_constant=0.4f; // gass stifness 
	viscosity=10.5f;
	time_step=0.003f;
	surf_norm=7.065f; // Threshold 
	surf_coe=0.0728f;

	walls = 1; 






}
SPHSystem::SPHSystem()
{




}

void SPHSystem::set_params(param paramas)
{
	this->actual_particles_number =  paramas.actual_particles_number ; 
	this->capsule_radius = paramas.capsule_radius ; 
	this->gas_constant = paramas.gas_constant ; 
	this->gravity = paramas.gravity ; 
	this->kernel = paramas.kernel ; 
	this->mass = paramas.mass ; 

	this->rest_density = paramas.rest_density ; 
	this->surf_coe =  paramas.surf_coe ; 
	this->surf_norm =  paramas.surf_norm ; 
	this->time_step =  paramas.time_step ; 
	this->viscosity =  paramas.viscosity ; 
	this->cT = paramas.cT;   

	this->walls =  paramas.walls ; 


}
void SPHSystem::freeSph(){


	free(mem);
	free(cell);

	for(int i =  0 ; i < max_paricles_around ; i ++) 
		free(particle_cell[i]); 

	free(particle_cell); 
}
SPHSystem::~SPHSystem()
{
	freeSph(); 
}

void SPHSystem::animation()
{

	if(sys_running == 0 ) return ; 
	build_table();
	comp_dens_pres();
	comp_force_adv();
	advection();
}


void SPHSystem::init_particles()
{
	float3 pos;
	float3 vel;

	vel.x=0.0f;
	vel.y=0.0f;
	vel.z=0.0f;

	for(pos.x=world_size.x*0.0f; pos.x<world_size.x*0.6f; pos.x+=(kernel*0.5f))
	{
		for(pos.y=world_size.y*0.0f; pos.y<world_size.y*0.9f; pos.y+=(kernel*0.5f))
		{
			for(pos.z=world_size.z*0.0f; pos.z<world_size.z*0.6f; pos.z+=(kernel*0.5f))
			{
				add_particle(pos, vel);
			}
		}
	}

	printf("Init Particle: %u\n", num_particle);
}
void SPHSystem::init_particles2()
{
	float3 pos;
	float3 vel;

	vel.x=0.0f;	
	vel.y=0.0f;
	vel.z=0.0f;

	for(pos.x=world_size.x*0.0f; pos.x<world_size.x; pos.x+=(kernel*0.5f))
	{
		for(pos.y=world_size.y*0.0f; pos.y<world_size.y; pos.y+=(kernel*0.5f))
		{
			for(pos.z=world_size.z*0.0f; pos.z<world_size.z; pos.z+=(kernel*0.5f))
			{



				if(num_particle == actual_particles_number) goto end; 
				calc_q(pos); 


				float d = capsule_penetration_test(pos);
				if (d < 1e-6) 
				{
					add_particle(pos, vel);
				}


			}
		}
	}
end :; 

	printf("Init Particle: %u\n", num_particle);
}

void SPHSystem::add_particle(float3 pos, float3 vel)
{

	if(num_particle == actual_particles_number) return ; 
	Particle *p=&(mem[num_particle]);

	p->id=num_particle;

	p->pos=pos;

	p->vel=vel;

	p->acc.x=0.0f;
	p->acc.y=0.0f;
	p->acc.z=0.0f;
	p->ev.x=0.0f;
	p->ev.y=0.0f;
	p->ev.z=0.0f;

	p->dens=rest_density;
	p->pres=0.0f;

	p->next=NULL;

	p->index = num_particle % sections ; 
	num_particle++;
}

void SPHSystem::build_table()
{
	Particle *p;
	uint hash;

	for(uint i=0; i<tot_cell; i++)
	{
		cell[i]=NULL;
	}

	for(uint i=0; i<num_particle; i++)
	{
		p=&(mem[i]);
		hash=calc_cell_hash(calc_cell_pos(p->pos));


		if(cell[hash] == NULL)
		{
			p->next=NULL;
			cell[hash]=p;
		}
		else
		{
			p->next=cell[hash];
			cell[hash]=p;
		}
	}
}

void SPHSystem::comp_dens_pres()
{
	Particle *p;
	Particle *np;

	int3 cell_pos;
	int3 near_pos;
	uint hash;

	float3 rel_pos;
	float r2;

	int maxn = 0 ; 

	for(uint i=0; i<num_particle; i++)
	{

		p=&(mem[i]); 
		cell_pos=calc_cell_pos(p->pos);



		p->dens=0.0f;
		p->pres=0.0f;

		p->particles_around = 0 ; 
		int j = 0  ; 



		for(int x=-1; x<=1; x++)
		{
			for(int y=-1; y<=1; y++)
			{
				for(int z=-1; z<=1; z++)
				{
					near_pos.x=cell_pos.x+x;
					near_pos.y=cell_pos.y+y;
					near_pos.z=cell_pos.z+z;
					hash=calc_cell_hash(near_pos);

					if(hash == 0xffffffff)
					{
						continue;
					}

					np=cell[hash];

					while(np != NULL)
					{
						rel_pos.x=np->pos.x-p->pos.x;
						rel_pos.y=np->pos.y-p->pos.y;
						rel_pos.z=np->pos.z-p->pos.z;
						r2=rel_pos.x*rel_pos.x+rel_pos.y*rel_pos.y+rel_pos.z*rel_pos.z;

						if( (walls &&  (np->index != p->index )) ||   r2<INF || r2>=kernel_2)
						{
							np=np->next;
							continue;
						}

						p->dens=p->dens + mass * poly6_value * pow(kernel_2-r2, 3);
						particle_cell[j][i] = np  ; 
						j++;
						np=np->next;

					}

				}
			}
		}
		p->particles_around =  j ; 
		p->dens=p->dens+self_dens;
		//p->pres=(pow(p->dens / rest_density, 7) - 1) *gas_constant; //here 
		p->pres = gas_constant * (p->dens-rest_density); 


	}


}

void SPHSystem::comp_force_adv()
{
	Particle *p;
	Particle *np;

	int3 cell_pos;
	int3 near_pos;
	uint hash = 0;

	float3 rel_pos;
	float3 rel_vel;

	float r2;
	float r;
	float kernel_r;
	float V;

	float pres_kernel;
	float visc_kernel;
	float temp_force;

	float3 grad_color;
	float lplc_color;

	for(uint i=0; i<num_particle; i++)
	{
		p=&(mem[i]); 
		cell_pos=calc_cell_pos(p->pos);

		p->acc.x=0.0f;
		p->acc.y=0.0f;
		p->acc.z=0.0f;




		grad_color.x=0.0f;
		grad_color.y=0.0f;
		grad_color.z=0.0f;
		lplc_color=0.0f;

		for(int j = 0 ;  j < p->particles_around ; j ++){

			np = particle_cell[j][i]; 
			rel_pos.x=p->pos.x-np->pos.x;
			rel_pos.y=p->pos.y-np->pos.y;
			rel_pos.z=p->pos.z-np->pos.z;
			r2=rel_pos.x*rel_pos.x+rel_pos.y*rel_pos.y+rel_pos.z*rel_pos.z;

			if(r2 < kernel_2 && r2 > INF)
			{
				r=sqrt(r2);
				V=mass/np->dens/2;
				kernel_r=kernel-r;
				//here



				pres_kernel = spiky_value * kernel_r * kernel_r /r  ; 
				if(fabsl(r) < INF) 
				{
					pres_kernel = r < 0 ? -spiky_value : spiky_value ; 
				}

				temp_force = ( (p->pres/pow(p->dens ,2)) + (np->pres/pow(np->dens,2))) *  mass * pres_kernel * p->dens ;

				p->acc.x=p->acc.x - rel_pos.x*temp_force;
				p->acc.y=p->acc.y - rel_pos.y*temp_force;
				p->acc.z=p->acc.z - rel_pos.z*temp_force;


				// end 




				rel_vel.x=np->ev.x-p->ev.x;
				rel_vel.y=np->ev.y-p->ev.y;
				rel_vel.z=np->ev.z-p->ev.z;

				visc_kernel=visco_value*(kernel-r);
				temp_force=V * viscosity * visc_kernel;
				p->acc.x=p->acc.x + rel_vel.x*temp_force; 
				p->acc.y=p->acc.y + rel_vel.y*temp_force; 
				p->acc.z=p->acc.z + rel_vel.z*temp_force; 

				float temp=(-1) * grad_poly6 * V * pow(kernel_2-r2, 2);
				grad_color.x += temp * rel_pos.x;
				grad_color.y += temp * rel_pos.y;
				grad_color.z += temp * rel_pos.z;
				lplc_color += lplc_poly6 * V * (kernel_2-r2) * (r2-3/4*(kernel_2-r2));
			}
		}


		lplc_color+=self_lplc_color/p->dens;
		p->surf_norm=sqrt(grad_color.x*grad_color.x+grad_color.y*grad_color.y+grad_color.z*grad_color.z);

		if(p->surf_norm > surf_norm)
		{
			p->acc.x+=surf_coe * lplc_color * grad_color.x / p->surf_norm;
			p->acc.y+=surf_coe * lplc_color * grad_color.y / p->surf_norm;
			p->acc.z+=surf_coe * lplc_color * grad_color.z / p->surf_norm;
		}
	}
}

void SPHSystem::reflect(){


	Particle *p;
	for(uint i=0; i<num_particle; i++)
	{
		p=&(mem[i]);
		if(p->pos.x >= world_size.x-BOUNDARY)
		{
			p->vel.x=p->vel.x*wall_damping;
			p->pos.x=world_size.x-BOUNDARY;
		}

		if(p->pos.x < 0.0f)
		{
			p->vel.x=p->vel.x*wall_damping;
			p->pos.x=0.0f;
		}

		if(p->pos.y >= world_size.y-BOUNDARY)
		{
			p->vel.y=p->vel.y*wall_damping;
			p->pos.y=world_size.y-BOUNDARY;
		}

		if(p->pos.y < 0.0f)
		{
			p->vel.y=p->vel.y*wall_damping;
			p->pos.y=0.0f;
		}

		if(p->pos.z >= world_size.z-BOUNDARY)
		{
			p->vel.z=p->vel.z*wall_damping;
			p->pos.z=world_size.z-BOUNDARY;
		}

		if(p->pos.z < 0.0f)
		{
			p->vel.z=p->vel.z*wall_damping;
			p->pos.z=0.0f;
		}

	}
}
void SPHSystem::update_particle(int i , float3 p){


	mem[i].pos_prev = mem[i].pos; 
	mem[i].pos = p  ; 

}
void SPHSystem::calc_q(float3 p) {


	float p0p1_len_2 = -vec_len_2(p0 , p1 ) ; 


	float t = (p0.x - p.x) * (p1.x - p0.x) + (p0.y - p.y) * (p1.y - p0.y) + (p0.z - p.z) * (p1.z - p0.z);
	t /= p0p1_len_2;
	t = t < 0.0f ? 0.0f : t ; 
	t = t > 1.0f ? 1.0f : t ; 

	q.x = p0.x + t * (p1.x - p0.x);
	q.y = p0.y + t * (p1.y - p0.y);
	q.z = p0.z + t * (p1.z - p0.z);

}
float SPHSystem::capsule_penetration_test(float3 p) {
	return vec_len(p ,  q) - capsule_radius;
}
float  SPHSystem::vec_len_2(float3 a , float3 b){

	float dx = a.x - b.x ; 
	float dy = a.y - b.y ; 
	float dz = a.z - b.z ; 
	return dx*dx + dy * dy + dz * dz ; 
}
float SPHSystem::vec_len(float3 a, float3 b){
	return sqrt(vec_len_2(a ,b )) ; 
}

void SPHSystem::adjust_with_plane(plane& pl , Particle* p ){


	float3 copy = p->pos; 

	float3 projectjon = pl.projection(p->pos); 
	float3 n = p->pos; 
	float d = vec_len(projectjon , p->pos); 
	if(d == 0 ) return ; 
	n.x -= projectjon.x; 
	n.y -= projectjon.y ; 
	n.z -= projectjon.z; 

	n.x /= d; 
	n.y /= d; 
	n.z /= d ;




	float damp = 1.0f + cT * d / (time_step * sqrt(p->vel.x * p->vel.x + p->vel.y * p->vel.y + p->vel.z * p->vel.z) );

	damp *= (p->vel.x * n.x +p->vel.y * n.y + p->vel.z * n.z);

	p->vel.x -= damp * n.x;
	p->vel.y -= damp * n.y;
	p->vel.z -= damp * n.z;

	p->pos.x -= d * n.x; 
	p->pos.y -= d *n.y ; 
	p->pos.z -= d * n.z; 

















}


void SPHSystem::adjust_contact_point(float3& pos) {
	float xq_len = capsule_radius / vec_len(pos , q); 

	pos.x = q.x + (pos.x - q.x) * xq_len;
	pos.y = q.y + (pos.y - q.y) * xq_len;
	pos.z = q.z + (pos.z - q.z) * xq_len;


}

float3 SPHSystem::unit_surface_normal(float3 p) {
	float3 n; 
	float xq_len = vec_len(p , q);

	n.x= ( q.x - p.x) / xq_len;
	n.y = ( q.y - p.y) / xq_len;
	n.z = (q.z - p.z) / xq_len;

	return n; 


}
void SPHSystem::reflect2() {










	for (int i = 0; i < (int)num_particle; i++) {
		Particle *p = &(mem[i]);





		// the main wall 

		if(walls){
			if(p->index <= 3 ){


				float pl_d  = planes[0].signed_distance(p->pos); 

				if(pl_d < 0 && pl_d != 0 ){
					adjust_with_plane(planes[0] , p); 
				}
			}else{


				float pl_d  = planes[0].signed_distance(p->pos); 

				if(pl_d > 0 && pl_d != 0 ){
					adjust_with_plane(planes[0] , p); 
				}
			}


			if(  (p->index == 0 || p->index == 4)){



				float pl_d  = planes[1].signed_distance(p->pos); 

				if(pl_d > 0 && pl_d != 0 ){
					adjust_with_plane(planes[1] , p); 
				}

			}
			if(p->index == 1 || p->index == 5){


				float pl_d  = planes[1].signed_distance(p->pos); 

				if(   (pl_d < 0 && pl_d != 0 )){
					adjust_with_plane(planes[1] , p); 
				}

				pl_d = planes[3].signed_distance(p->pos); 
				if(( pl_d > 0 && pl_d != 0 )){
					adjust_with_plane(planes[3] , p); 
				}
			}
			if(p->index == 2 || p->index == 6){


				float pl_d  = planes[2].signed_distance(p->pos); 

				if( (pl_d > 0 && pl_d != 0 )){
					adjust_with_plane(planes[2] , p); 
				}

				pl_d = planes[3].signed_distance(p->pos); 
				if(   (pl_d < 0 && pl_d != 0 )){
					adjust_with_plane(planes[3] , p); 
				}
			}

			if(p->index == 3 || p->index == 7){



				float pl_d  = planes[2].signed_distance(p->pos); 

				if( (pl_d < 0 && pl_d != 0 )){
					adjust_with_plane(planes[2] , p); 
				}

			}
		}



		calc_q(p->pos); 

		float d = capsule_penetration_test(p->pos);

		if (d > 1e-6) {
			float3 hi = p->pos_prev;
			float3 lo = p->pos;

			adjust_contact_point(p->pos);

			float3 n = unit_surface_normal(p->pos) ; 


			float damp = 1.0f + cT * d / (time_step * sqrt(p->vel.x * p->vel.x + p->vel.y * p->vel.y + p->vel.z * p->vel.z) );

			damp *= (p->vel.x * n.x +p->vel.y * n.y + p->vel.z * n.z);

			p->vel.x -= damp * n.x;
			p->vel.y -= damp * n.y;
			p->vel.z -= damp * n.z;


		}











	}
}

void SPHSystem::advection()
{
	Particle *p;
	
	for(uint i=0; i<num_particle; i++)
	{
		p=&(mem[i]);

		//float3 f = (p->pos - this->target).cross(this->dw); 


	

		p->vel.x = p->vel.x + p->acc.x * time_step / p->dens + gravity.x * time_step ;
		p->vel.y=p->vel.y+p->acc.y*time_step/p->dens+gravity.y*time_step ; 
		p->vel.z=p->vel.z+p->acc.z*time_step/p->dens+gravity.z*time_step ; 
		

		if(this->w .y != 0){

			float3 coriolis = p->vel.cross(this->w * 2) ; 

			p->vel += coriolis * time_step ; 




		}
		p->vel += truck_acc  * time_step ;



		float3 pos = p->pos; 
		p->pos.x=p->pos.x+p->vel.x*time_step;
		p->pos.y=p->pos.y+p->vel.y*time_step;
		p->pos.z=p->pos.z+p->vel.z*time_step;
		p->pos_prev = pos; 


		p->ev.x=(p->ev.x+p->vel.x)/2;
		p->ev.y=(p->ev.y+p->vel.y)/2;
		p->ev.z=(p->ev.z+p->vel.z)/2;

	}


	reflect2();
}

int3 SPHSystem::calc_cell_pos(float3 p)
{




	origin.x = p0.x - capsule_radius ; 
	origin.y = p0.y - capsule_radius ; 
	origin.z = p0.z - capsule_radius ; 
	p.x = p.x - origin.x + 2 ; 
	p.y = p.y - origin.y + 2 ; 
	p.z = p.z - origin.z + 2 ; 


	int3 cell_pos;
	cell_pos.x = int(floor((p.x) / cell_size))  ;
	cell_pos.y = int(floor((p.y) / cell_size)) ;
	cell_pos.z = int(floor((p.z) / cell_size)) ;

	return cell_pos;
}

uint SPHSystem::calc_cell_hash(int3 cell_pos)
{
	if(cell_pos.x<0 || cell_pos.x>=(int)grid_size.x || cell_pos.y<0 || cell_pos.y>=(int)grid_size.y || cell_pos.z<0 || cell_pos.z>=(int)grid_size.z)
	{
		return (uint)0xffffffff;
	}

	cell_pos.x = cell_pos.x & (grid_size.x-1);  
	cell_pos.y = cell_pos.y & (grid_size.y-1);  
	cell_pos.z = cell_pos.z & (grid_size.z-1);  

	return ((uint)(cell_pos.z))*grid_size.y*grid_size.x + ((uint)(cell_pos.y))*grid_size.x + (uint)(cell_pos.x);
}

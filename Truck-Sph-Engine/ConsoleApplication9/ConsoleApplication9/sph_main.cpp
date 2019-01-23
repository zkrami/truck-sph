

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include "stdafx.h"
#include "sph_header.h"
#include "sph_data.h"
#include "sph_timer.h"
#include "sph_system.h"



extern "C"{

SPHSystem *sph;

  __declspec( dllexport ) void init(){

	

	sph=new SPHSystem();		
	sph->set_default_paramas(); 
	
	sph->init_system();
	


}
__declspec( dllexport )  void update(){
	sph->animation(); 
}
__declspec( dllexport )  void pause(){
	sph->sys_running = 0 ; 
}
__declspec( dllexport )  void play(){
	sph->sys_running = 1; 
}

__declspec( dllexport ) void get_particle(int i , float* x , float *y , float *z){

	*x = sph->mem[i].pos.x;

	*y=sph->mem[i].pos.y;
	*z = sph->mem[i].pos.z;

}
__declspec( dllexport ) void free_sph(){
	
	delete sph ; 
}
	__declspec( dllexport ) void re_init(){
		sph->freeSph(); 
		sph->init_system(); 
		
	}



	__declspec( dllexport ) void set_paramas(float actual_particles_number , float capsule_radius , float gas_constant , float gravity_x , float gravity_y , float gravity_z  , 
					float kernel , float mass , float rest_desnity , float surf_coe , 
					float surf_norm , float time_step , float viscosity , float cT  , bool walls  )
		{

			param paramas ; 
			paramas.actual_particles_number =(int) actual_particles_number ; 
			paramas.capsule_radius = capsule_radius ; 
			paramas.gas_constant = gas_constant ; 
			float3 gravity ; 
			gravity.x = gravity_x ; 
			gravity.y = gravity_y ; 
			gravity.z = gravity_z ; 

			paramas.gravity = gravity ; 




			paramas.kernel = kernel; 
			paramas.mass = mass; 
			paramas.rest_density = rest_desnity ; 
			paramas.surf_coe  = surf_coe; 
			paramas.surf_norm = surf_norm; 
			paramas.time_step = time_step ; 
			paramas.viscosity = viscosity; 

			
			paramas.cT = cT ; 
		
			paramas.walls = walls; 
			sph->set_params(paramas); 

		


	}

	__declspec( dllexport ) void set_points(float p0_x, float p0_y, float p0_z, float p1_x, float p1_y, float p1_z,
        float p2_x, float p2_y, float p2_z, float p3_x, float p3_y, float p3_z, float p4_x, float p4_y, float p4_z
		, float p5_x, float p5_y, float p5_z){
			sph->p0.x = p0_x ; 
			sph->p0.y = p0_y ; 
			sph->p0.z = p0_z ; 

			sph->p1.x = p1_x ; 
			sph->p1.y = p1_y ; 
			sph->p1.z = p1_z ; 


			sph->p2.x = p2_x ; 
			sph->p2.y = p2_y ; 
			sph->p2.z = p2_z ; 

			sph->p3.x = p3_x ; 
			sph->p3.y = p3_y ; 
			sph->p3.z = p3_z ; 


			sph->p4.x = p4_x ; 
			sph->p4.y = p4_y ; 
			sph->p4.z = p4_z ; 

			sph->p5.x = p5_x ; 
			sph->p5.y = p5_y ; 
			sph->p5.z = p5_z ; 
	}

	__declspec( dllexport ) void truck_acc(float x , float y ,float z){

		sph->truck_acc.x = x; 
		sph->truck_acc.y = y; 
		sph->truck_acc.z =z ;
	}


	__declspec( dllexport ) void set_target(float x, float y, float z){
		sph->target.x = x; 
		sph->target.y = y; 
		sph->target.z = z; 
	}
	__declspec( dllexport ) void set_dw(float y){
		sph->dw.x = 0; 
		sph->dw.y = y; 
		sph->dw.z = 0; 
	}

	__declspec( dllexport ) void set_w(float y){
		sph->w.x = 0; 
		sph->w.y = y; 
		sph->w.z = 0; 
	}


	




}


__kernel void simulate(
	float dt1,
	__global float4* pos,
	__global float4* vel,
	__global float4* acc
	)
	{
		const float4 dt = (float4)(dt1,dt1,dt1,0.0f);
		const float4 dt2 = (float4)(0,0,0,dt1);
		int gti = get_global_id(0); // global id of work-item ( == index to update)

		float4 p = pos[gti];
		float4 v = vel[gti];
		float4 a = acc[gti];

		p += dt*v;
		v += dt*a;
		p -= dt2;

		//p *= p.s3 > 0 ? 1.0 : 0.0;
		//v *= p.s3 > 0 ? 1.0 : 0.0;

		pos[gti] = p;
		vel[gti] = v;
	}

__kernel void generate(
	__global float4* pos
	)
	{
		int gti = get_global_id(0); // global id of work-item ( == index to update)
		int n = get_global_size(0);	//  number of particles

		pos[gti] = (float4)(0.0f,0.0f,0.0f,1000.0f);
	}

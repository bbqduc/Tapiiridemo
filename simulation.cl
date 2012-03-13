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

		v += dt*a;
		p += dt*v;
		p -= dt2;

		pos[gti] = p;
		vel[gti] = v;
	}


__kernel void simulateNBODY(
	float dt1,
	__global float4* pos,
	__global float4* vel,
	__global float4* acc,
	__local float4* pblock
	)
	{
		const float4 dt = (float4)(dt1,dt1,dt1,0.0f);
		const float eps = 0.001f;

		int gti = get_global_id(0); // global id of work-item ( == index to update)
		int ti = get_local_id(0);

		int n = get_global_size(0);
		int nt = get_local_size(0);
		int nb = n/nt;

		float4 p = pos[gti];
		float4 v = vel[gti];
		float4 a = acc[gti];

		a=0;
		//v=0;

		for(int jb=0; jb<nb; jb++)
		{
			pblock[ti] = pos[jb*nt+ti]; // Cache one particle position
			barrier(CLK_LOCAL_MEM_FENCE); // Wait for others in this work-group

			for(int j=0; j<nt; j++) // For all cached particle positions
			{
				float4 p2 = pblock[j]; // Read a cached particle position
				float4 d = (p2-p)*100;
				float invr = rsqrt(d.x*d.x + d.y*d.y + d.z*d.z + eps); // eps to avoid division by zero
				float f = p2.w*invr*invr*invr;
				a += f*d; // accumulate acceleration
			}

			barrier(CLK_LOCAL_MEM_FENCE); // Wait for others in this work-group
		}

		float d = p.x*p.x + p.y*p.y + p.z*p.z;
		float4 temp = (float4)(1.0f,1.0f,1.0f,0.0f);
		a -= p*d;
		p += dt*v + 0.2f;
		v += dt*a / 1.0;

		pos[gti] = p;
		vel[gti] = v;
		acc[gti] = a;
	}

__kernel void generate(
	__global float4* pos
	)
	{
		int gti = get_global_id(0); // global id of work-item ( == index to update)
		int n = get_global_size(0);	//  number of particles

		pos[gti] = (float4)(0.0f,0.0f,0.0f,1000.0f);
	}

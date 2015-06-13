void kernel CollisionWithJumps(global const int* triangleCount, global const float* vA, global const int* iA, global const float* vB, global const int* iB, global bool* C)
{
	int index = get_global_id(0);

	if(index > triangleCount[2])
		return;
	
	C[3*index]=triangleCount[1];
	C[3*index+1]=1;
	C[3*index+2]=triangleCount[0];
};

void kernel CollisionWithJumps(global const int* triangleCount, global const float* vA, global const int* iA, global const float* vB, global const int* iB, global bool* C)
{
	int index = get_global_id(0);

	if(index > triangleCount[2])
		return;
	
	C[index]=triangleCount[0];
};

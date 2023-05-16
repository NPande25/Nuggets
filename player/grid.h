
//creates grid and allocates for grid, gridarray, and all gridcells within it
grid_t* grid_new(int NR, int NC);

//loads char map into grid initializing gridcells?
void grid_load(grid_t* grid, char* map);
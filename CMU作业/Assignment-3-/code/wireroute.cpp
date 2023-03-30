/**
 * Parallel VLSI Wire Routing via OpenMP
 * Name 1(andrew_id 1), Name 2(andrew_id 2)
 */

#include "wireroute.h"

#include <assert.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <omp.h>
#include <bits/stdc++.h>
static int _argc;
static const char **_argv;

const char *get_option_string(const char *option_name,
                              const char *default_value) {
  for (int i = _argc - 2; i >= 0; i -= 2)
    if (strcmp(_argv[i], option_name) == 0)
      return _argv[i + 1];
  return default_value;
}

int get_option_int(const char *option_name, int default_value) {
  for (int i = _argc - 2; i >= 0; i -= 2)
    if (strcmp(_argv[i], option_name) == 0)
      return atoi(_argv[i + 1]);
  return default_value;
}

float get_option_float(const char *option_name, float default_value) {
  for (int i = _argc - 2; i >= 0; i -= 2)
    if (strcmp(_argv[i], option_name) == 0)
      return (float)atof(_argv[i + 1]);
  return default_value;
}
static void show_help(const char *program_path) {
  	printf("Usage: %s OPTIONS\n", program_path);
  	printf("\n");
  	printf("OPTIONS:\n");
  	printf("\t-f <input_filename> (required)\n");
  	printf("\t-n <num_of_threads> (required)\n");
  	printf("\t-p <SA_prob>\n");
  	printf("\t-i <SA_iters>\n");
}
void new_route(wire_t &wire){
	if(rand()%2==0){
		wire.v=wire.x1+rand()%(wire.x2-wire.x1+1);
		wire.flag=0;
	}
	else{
		wire.v=std::min(wire.y1,wire.y2)+rand()%(abs(wire.y2-wire.y1)+1);
		wire.flag=1;
	}
}
int caculate_cost(wire_t wire,int flag,int v,cost_t *cost,int dim_x){
	int sum=0;
	if(wire.flag==0){
		for(int i=wire.x1;i<=v;i++) sum+=cost[wire.y1*dim_x+i];
		for(int i=std::min(wire.y1,wire.y2);i<=std::max(wire.y1,wire.y2);i++) sum+=cost[i*dim_x+v];
		for(int i=v+1;i<=wire.x2;i++) sum+=cost[wire.y2*dim_x+i];
	}
	else{
		for(int i=std::min(wire.y1,wire.y2);i<=v;i++) sum+=cost[i*dim_x+std::min(wire.y1,wire.y2)];
		for(int i=wire.x1;i<=wire.x2;i++) sum+=cost[wire.v*dim_x+i];
		for(int i=v+1;i<=std::max(wire.y1,wire.y2);i++) sum+=cost[i*dim_x+std::max(wire.y1,wire.y2)];
	}
	return sum;
}
void change_cost(wire_t wire,int k,cost_t *cost,int dim_x){
	if(wire.flag==0){
		for(int i=wire.x1;i<=wire.v-1;i++) cost[wire.y1*dim_x+i]+=k;
		for(int i=std::min(wire.y1,wire.y2);i<=std::max(wire.y1,wire.y2);i++) cost[i*dim_x+wire.v]+=k;
		for(int i=wire.v+1;i<=wire.x2;i++) cost[wire.y2*dim_x+i]+=k;
	}
	else{
		int x1=wire.x1,y1=wire.y1,x2=wire.x2,y2=wire.y2;
		if(wire.y1>wire.y2){
			std::swap(y1,y2);
			std::swap(x1,x2);
		}
		for(int i=y1;i<=wire.v-1;i++) cost[i*dim_x+x1]+=k;
		for(int i=std::min(wire.x1,wire.x2);i<=std::max(wire.x1,wire.x2);i++) cost[wire.v*dim_x+i]+=k;
		for(int i=wire.v+1;i<=y2;i++) cost[i*dim_x+x2]+=k;
	}
}
void init_route(int num_of_wires,wire_t *wires,cost_t *cost,int dim_x){
	for(int i=0;i<num_of_wires;i++){
		new_route(wires[i]);
		wires[i].sum=caculate_cost(wires[i],wires[i].flag,wires[i].v,cost,dim_x);
		change_cost(wires[i],1,cost,dim_x);
	}
}
int main(int argc, const char *argv[]) {
  	using namespace std::chrono;
  	typedef std::chrono::high_resolution_clock Clock;
  	typedef std::chrono::duration<double> dsec;

  	auto init_start = Clock::now();
  	double init_time = 0;

  	_argc = argc - 1;
  	_argv = argv + 1;

  	const char *input_filename = get_option_string("-f", NULL);
  	int num_of_threads = get_option_int("-n", 1);
 	double SA_prob = get_option_float("-p", 0.1f);
  	int SA_iters = get_option_int("-i", 5);
  	int error = 0;
  	if (input_filename == NULL) {
    	printf("Error: You need to specify -f.\n");
    	error = 1;
  	}

  	if (error) {
    	show_help(argv[0]);
    	return 1;
  	}
	
  	printf("Number of threads: %d\n", num_of_threads);
  	printf("Probability parameter for simulated annealing: %lf.\n", SA_prob);
  	printf("Number of simulated annealing iterations: %d\n", SA_iters);
  	printf("Input file: %s\n", input_filename);

  	FILE *input = fopen(input_filename, "r");
  	if (!input) {
    	printf("Unable to open file: %s.\n", input_filename);
    	return 1;
  	}

  	int dim_x, dim_y;
  	int num_of_wires;

  	fscanf(input, "%d %d\n", &dim_x, &dim_y);
  	fscanf(input, "%d\n", &num_of_wires);

  	wire_t *wires = (wire_t *)calloc(num_of_wires, sizeof(wire_t));
	int *mp=(int *)calloc(dim_x*dim_y+10,sizeof(int));
  	/* Read the grid dimension and wire information from file */
	for(int i=0;i<num_of_wires;i++){
		fscanf(input,"%d %d %d %d\n",&wires[i].x1,&wires[i].y1,&wires[i].x2,&wires[i].y2);
		if(wires[i].x1>wires[i].x2){
			std::swap(wires[i].x1,wires[i].x2);
			std::swap(wires[i].y1,wires[i].y2);
		} 
	}
  	cost_t *costs = (cost_t *)calloc(dim_x * dim_y, sizeof(cost_t));
  	/* Initialize cost matrix */
	for(int i=0;i<dim_y;i++)
		for(int t=0;t<dim_x;t++){
			costs[i*dim_x+t]=0;
		}  
  	/* Initailize additional data structures needed in the algorithm */
	init_route(num_of_wires,wires,costs,dim_x);

  	init_time += duration_cast<dsec>(Clock::now() - init_start).count();
  	printf("Initialization Time: %lf.\n", init_time);

  	auto compute_start = Clock::now();
  	double compute_time = 0;

  /** 
   * Implement the wire routing algorithm here
   * Feel free to structure the algorithm into different functions
   * Don't use global variables.
   * Use OpenMP to parallelize the algorithm. 
   */ 
	while(SA_iters>0){
		SA_iters--;
		
		for(int i=0;i<num_of_wires;i++){
			std::random_device rd;
    		std::mt19937 gen(rd());
    		std::uniform_real_distribution<> dis(0.0, 1.0);
    		double P_C = dis(gen);
			if(P_C>(1-SA_prob)){
				change_cost(wires[i],-1,costs,dim_x);
    			new_route(wires[i]);
    			change_cost(wires[i],1,costs,dim_x);
    			continue;
			}
			int x1=std::min(wires[i].x1,wires[i].x2),x2=std::max(wires[i].x1,wires[i].x2);
			int flag=0;
			change_cost(wires[i],-1,costs,dim_x);
			#pragma omp parallel for  num_threads(num_of_threads)
			for(int t=wires[i].x1;t<=wires[i].x2;t++){ 
				int sum=caculate_cost(wires[i],0,t,costs,dim_x);
				#pragma omp critical
        		{
            		if(sum<wires[i].sum){
                		wires[i].sum=sum;
                		wires[i].flag=0;
                		wires[i].v=t;
    	        	}
        		}
			}
			#pragma omp parallel for num_threads(num_of_threads)
			for(int t=std::min(wires[i].y1,wires[i].y2);t<=std::max(wires[i].y1,wires[i].y2);t++){
				int sum=caculate_cost(wires[i],1,t,costs,dim_x);
				#pragma omp critical
        		{
            		if(sum<wires[i].sum){
                		wires[i].sum=sum;
                		wires[i].flag=1;
                		wires[i].v=t;
    	        	}
        		}
			}
			change_cost(wires[i],1,costs,dim_x);
		}
	}
  	compute_time += duration_cast<dsec>(Clock::now() - compute_start).count();
  	printf("Computation Time: %lf.\n", compute_time);
	
  	/* Write wires and costs to files */
  	FILE *output1 = fopen("cost.txt", "w");
  	fprintf(output1, "%d %d\n", dim_x, dim_y);
  	for(int i=0;i<dim_y;i++){
  		for(int t=0;t<dim_x;t++){
  			fprintf(output1,"%d ",costs[i*dim_x+t]);	
		}
		fprintf(output1,"\n");
	  }
  		
  	
  	FILE *output2 = fopen("wire.txt", "w");
  	fprintf(output2, "%d %d\n", dim_x, dim_y);
  	fprintf(output2, "%d\n",num_of_wires);
  	for(int i=0;i<num_of_wires;i++){
  		if(wires[i].flag==0){
  			if(wires[i].v==wires[i].x1){
  				fprintf(output2,"%d %d %d %d %d %d\n",wires[i].x1,wires[i].y1,wires[i].x1,wires[i].y2,wires[i].x2,wires[i].y2);
			  }
			else if(wires[i].v==wires[i].x2){
				fprintf(output2,"%d %d %d %d %d %d\n",wires[i].x1,wires[i].y1,wires[i].x2,wires[i].y1,wires[i].x2,wires[i].y2);
			}
			else fprintf(output2,"%d %d %d %d %d %d %d %d\n",wires[i].x1,wires[i].y1,wires[i].v,wires[i].y1,wires[i].v,wires[i].y2,wires[i].x2,wires[i].y2);
		}
		else{
			if(wires[i].v==wires[i].y1){
  				fprintf(output2,"%d %d %d %d %d %d\n",wires[i].x1,wires[i].y1,wires[i].x2,wires[i].y1,wires[i].x2,wires[i].y2);
			  }
			else if(wires[i].v==wires[i].y2){
				fprintf(output2,"%d %d %d %d %d %d\n",wires[i].x1,wires[i].y1,wires[i].x1,wires[i].y2,wires[i].x2,wires[i].y2);
			}
			else fprintf(output2,"%d %d %d %d %d %d %d %d\n",wires[i].x1,wires[i].y1,wires[i].x1,wires[i].v,wires[i].x2,wires[i].v,wires[i].x2,wires[i].y2);
			
		}	
	}
	fclose(output1);
	fclose(output2);
  	return 0;
}

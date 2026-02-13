#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


#define MAX 10000

/* graph structure*/

typedef struct 
{
	int v_count;
	int e_count;
	int* vptr;
	int* eptr;
	int* edwt;
	int *comm;
	int *boundary;	
	float *clc;
	float *border_score;
	float *composite_score;
	int* seed;
}Graph;
/* printing array */

void print_array(int* arr, int size){
	int i;
	printf("\n");
	for(i = 0;i < size;i++)
	printf(" %d ",arr[i]);

}
/*Inserting values in CSR of original graph*/

Graph* CSR(char* file_name,char* f2){


	Graph* csr;
	FILE * fp, * f;
	int i=0,j=0;
	int left_v,right_v,prev_v,l_v,r_v;
	
	csr = (Graph*) malloc(sizeof(Graph));/* Allocate memory for Graph structure */
	if(csr == NULL){
		printf("\nDynamic memory allocation failed.\n");
		return NULL;
	}
	
	/* open input file */
	
	fp = fopen(file_name,"r");
	if (fp == NULL) {
        printf("Error opening file: %s\n", file_name);
        return NULL;
    }
	fscanf(fp,"%d\n%d\n",&csr->v_count,&csr->e_count);/* read no of vertices and edges from the file */
		
	/* Allocate memory for edge pointer array for vertices */
	csr->vptr = (int*) malloc((csr->v_count+1) * sizeof(int)); 
	/* here the extra index serves as a sentinel value: it marks the total number of edges in the graph*/ 
	
	if(csr->vptr == NULL){
		printf("\nDynamic memory allocation failed.\n");
		return NULL;
	}
	
	/* Allocate memory for edges of the vertices */
	csr->eptr = (int*) malloc(2*csr->e_count * sizeof(int));
	if(csr->eptr == NULL){
		printf("\nDynamic memory allocation failed.\n");
		return NULL;
	}
	
	csr->comm=(int*) malloc(csr->v_count * sizeof(int));
	if(csr->comm == NULL){
		printf("\nDynamic memory allocation failed.\n");
		return NULL;
	}
	csr->boundary = (int*)calloc(csr->v_count, sizeof(int)); /* Initialize to 0*/
	if(csr->boundary == NULL){
		printf("\nDynamic memory allocation failed.\n");
		return NULL;
	} 
	
	csr->clc = (float*)calloc(csr->v_count, sizeof(float)); /* Initialize to 0*/
	if(csr->clc == NULL){
		printf("\nDynamic memory allocation failed.\n");
	    exit(0);
	}
	
	csr->border_score = (float*)calloc(csr->v_count, sizeof(float)); /* Initialize to 0*/
	if(csr->border_score == NULL){
		printf("\nDynamic memory allocation failed.\n");
	    exit(0);
	}
	
	csr->composite_score = (float*)calloc(csr->v_count, sizeof(float)); /* Initialize to 0*/
	if(csr->composite_score == NULL){
		printf("\nDynamic memory allocation failed.\n");
	    exit(0);
	}
	/* Reading the first edge of the graph from file */
	fscanf(fp,"%d,%d\n",&left_v,&right_v);
    while(!feof(fp))  /*feof inbuilt  end of file*/
	{
		prev_v = left_v;
		csr->vptr[i] = j;
		/*Condition check for multiple edges originating from same vertex*/
		while(prev_v == left_v && !feof(fp))
		{
			csr->eptr[j] = right_v;	
			/*Reading each edge from file*/ 
			fscanf(fp,"%d,%d\n",&left_v,&right_v);	
			j++;
			/*handling the last edge of the graph*/
			if(feof(fp))
			{
				if(prev_v != left_v)
					csr->vptr[++i] = j;
				csr->eptr[j] = right_v;
			}
				
		}
		i++;
	}
	csr->vptr[csr->v_count] = j+1;
	fclose(fp);
	f = fopen(f2, "r");
     if (f == NULL) {
        printf("Error opening file: %s\n", f2);
        return NULL;
    }

	do
	{
	    fscanf(f,"%d %d\n",&l_v,&r_v);
	    csr->comm[l_v]=r_v;
		
	}while(!feof(f));
	
	fclose(f);
	csr->seed = (int*)calloc(csr->v_count, sizeof(int)); /* initialize with 0s */
			
	return csr;
	
}



/*Findng the boundary nodes*/
int boundary(Graph *graph)
{
	
	
	int i,j,b_count=0;
	
    /* Example: Printing the edge list*/
    
    for (i = 0; i < graph->v_count; i++) 
	{
    	
        for (j = graph->vptr[i]; j<graph->vptr[i+1]; j++) 
		{
            
			 if (graph->comm[i] != graph->comm[graph->eptr[j]]) 
			 {
                graph->boundary[i] = 1;
                graph->boundary[graph->eptr[j]] = 1;
                b_count++;/*counting number of boundary elements*/
                
                break;
             }
			  
	        
        }
        
    }
    
    /*
    printf("Boundary Vertices:\n");
    for (i = 0; i < graph->v_count; i++) {
        printf("%d -> %d\n", i, graph->boundary[i]);
    }*/
    return b_count;
}

/*bfs to find the shortest path and closeness centrality of each node in a community*/
float bfs(Graph *graph,int s)
{
	/*printf("\n Enter bfs \n");*/
    int *par,p,*d,*q,i,c=0,front=-1,rear=-1,u,v;
	long long sum=0;
    float cl;
	par=(int*)calloc(graph->v_count,sizeof(int));
	d=(int*)calloc(graph->v_count,sizeof(int));
	q=(int*)calloc(graph->v_count,sizeof(int));
	for(i=0;i<graph->v_count;i++){
		d[i]=-1;
		par[i]=-1;
	}
	d[s]=0;
	
     q[++rear]=s; /*enqueue*/
	while(front!= rear){
	u=q[++front]; /* dequeue*/

	c=c+1;
	for(i=graph->vptr[u];i<graph->vptr[u+1];i++){
		v=graph->eptr[i];
		if(graph->comm[v]==graph->comm[u]){	
		if(d[v]<0){
				d[v]=d[u]+1;
				par[v]=u;
	        	q[++rear]=v;
		
    	}
      }
	}
	
 }
	/* printf("\n%d",c);*/
	
	for(i=0;i<graph->v_count;i++){
		if(d[i]!=-1){
		
		  /*printf("\nDistance from node %d to node %d is :%d",s,i,d[i]);*/
		  sum+=d[i];
	   }
	}

	if (sum == 0 || c <= 1) return 0.0;
    cl = (float)(c - 1) / (float)sum;
	free(d);
    free(q);
    /*printf("\n Exit bfs \n");*/
    return cl;
}
/*calculate and store the closeness centrality of each node  with respect to community*/


void closenessCentrality(Graph *graph)
{
	printf("\n Enter closenessCentrality \n");
	int i;
	
		
	for(i=0;i<graph->v_count;i++)
	{
		float clc_val = bfs(graph, i);
        graph->clc[i] = clc_val;
        
	   
    }
    
    /*printf("\nCloseness Centrality:\n");
	for(i=0;i<graph->v_count;i++)
	{
		if(i<=739)
		printf("\n%d %f",i,graph->clc[i]);
	}
	*/
    printf("\n Exit closenessCentrality\n");
}

/*finding minimum and maximum centrality*/
float* min_max_centrality(Graph *graph)
{
	/*printf("\n Enter min_max_centrality \n");*/
	float *min_max=(float*)malloc(2*sizeof(float));
	int i;
	float min,max;
	min=graph->clc[0];
	max=graph->clc[0];
	

	for (i=1;i<graph->v_count;i++)
	{
	
	   if(graph->clc[i]<min)
	   {
	    	min=graph->clc[i];
	   }
	   if(graph->clc[i]>max)
	   {
	    	max=graph->clc[i];
	   }
	} 
	min_max[0]=min;
	min_max[1]=max;
	/*printf("\n Exit min_max_centrality\n");*/
	return min_max;
	
}

/*finding number of nodes having closeness centrality greater than or equals to  cc*/
int num_nodes(Graph *graph,float cc)
{
	/*printf("\n Enter num_nodes\n");*/
	int i,n_count=0;
	
	for(i=0;i<graph->v_count;i++)
	{
		if(graph->clc[i]>=cc)
		{
			
			n_count++;
		}
	}
	/*printf("Done nn\nn_count:%dof f\n ",n_count);*/
	return n_count;
	/*printf("\n Exit num_nodes\n");*/
	
}

/*finding the normalized score*/
void median_score(Graph *graph)
{
	printf("\n Enter median_score\n");
	float l=0,min,max,mid,*min_max;
	int b_count,n_count;
	
	min_max=min_max_centrality(graph);
	max=min_max[1];
	min=min_max[0];
	l=(max+min)/2.0;
	
	b_count=boundary(graph);
	float a=(max-min)/4.0;
	while(l>min && l<max && a>0.0000001)
	{
		
		n_count=num_nodes(graph,l);
		/*printf("\nl:%f,min:%f,max:%f,b_count:%d,n_count:%d,a:%f\n",l,min,max,b_count,n_count,a);*/
		if(n_count>b_count)
		{
			printf("\n n_count>b_count \n");
			l=l+a;
		}
		    
		else if(n_count<b_count)
		{
			printf("\n n_count<b_count \n");
			l=l-a;
		}
		    
		else
		    break;
		a=a/2;
		  
		
	}
	printf("\nMedian score:%f\n",l);
	printf("\n Exit median_score\n");
	free(min_max);
	
}

void boundary_score(Graph *graph)
{
	
	
	int i,j;
	float comm_count,deg;
	
    printf("\n Enter boundary_score\n");
    
    for (i = 0; i < graph->v_count; i++) 
	{ 
	    comm_count=0.0;
    	deg=0.0;
        for (j = graph->vptr[i]; j<graph->vptr[i+1]; j++) 
		{
            
			 if (graph->comm[i] != graph->comm[graph->eptr[j]]) 
			 {
                
                comm_count++;/*counting number of different comm connected edges*/ 
             }
			 
        }
        
        if(i==graph->v_count-1)
        {
		    deg=(2*graph->e_count) - graph->vptr[i]+1;
            
        	graph->border_score[i]=(comm_count/deg);
        	break;
		}
        else{
        	deg=graph->vptr[i+1]-graph->vptr[i];
        	
        	graph->border_score[i]=(comm_count/deg);
		}
           
    }
    
    /*printf("\nBorder score of Vertices:\n");
    for (i = 0; i < graph->v_count; i++) {
        printf("%d -> %f\n", i, graph->border_score[i]);
    }
    */
    printf("\n Exit boundary_score\n");
}

void composite_score(Graph *graph)
{
	int i;
	
    for(i=0;i<graph->v_count;i++)
    {
    	graph->composite_score[i]=graph->clc[i]+graph->border_score[i];
       /* printf("\n comp score for vertex %d is %f",i,graph->composite_score[i]);*/
	}
}


Graph* communityGraph(Graph *graph)
{
	printf("\n Enter communityGraph\n");

	int i,j,k,c_num=graph->comm[0],ck,ct,e=0,**adj;
	Graph* comm_csr;
	for(i=1;i<graph->v_count;i++)
	{
		if(graph->comm[i]>c_num)
		   c_num=graph->comm[i];
	}
	
	comm_csr = (Graph*)calloc(1,sizeof(Graph));/* Allocate memory for Graph structure */
	if(comm_csr == NULL)
	{
		printf("\nDynamic memory allocation failed.\n");
		return NULL;
	}
	comm_csr->v_count=c_num+1;
	comm_csr->vptr = (int*)calloc((comm_csr->v_count+1),sizeof(int));
	
	if(comm_csr->vptr == NULL)
	{
		printf("\nDynamic memory allocation failed.\n");
		return NULL;
	}
	
	/* Allocate memory for edges of the vertices */
	adj=(int**)calloc(comm_csr->v_count,sizeof(int*));
	for(i=0;i<comm_csr->v_count;i++)
	{
		adj[i]=(int*)calloc(comm_csr->v_count,sizeof(int));
	}
	for (i = 0; i < graph->v_count; i++) 
	{ 
	    
	    ck=graph->comm[i];
	    
        if(graph->boundary[i]==0)
         continue;
        
        else
        {
        	for(j=graph->vptr[i];j<graph->vptr[i+1];j++)
        	{
        		
        			ct = graph->comm[graph->eptr[j]];
        			if(ct!=ck)
					{
						adj[ck][ct]=adj[ck][ct]+1;
						if(adj[ck][ct]==1)
						   e++;
					
					}					
        		
			}
		}
   }
	printf("\n Printing the Community edges:-%d",e);
	comm_csr->e_count=e;
	comm_csr->eptr = (int*) calloc(comm_csr->e_count , sizeof(int));
	if(comm_csr->eptr == NULL)
	{
		printf("\nDynamic memory allocation failed.\n");
		return NULL;
	}
	comm_csr->edwt = (int*) calloc(comm_csr->e_count , sizeof(int));
	if(comm_csr->edwt == NULL)
	{
		printf("\nDynamic memory allocation failed.\n");
		return NULL;
    }
   
    k=0;
    for(i=0;i<comm_csr->v_count;i++)
    {
    	comm_csr->vptr[i]=k;
    	for(j=0;j<comm_csr->v_count;j++)
    	{
    		if(adj[i][j]>0)
    		{
    			comm_csr->eptr[k]=j;
    			comm_csr->edwt[k]=adj[i][j];
    			k++;
			}
    		
		}
	}
	comm_csr->vptr[comm_csr->v_count]=k;
	/*printing the csr*/
	
	/*printf("\n Printing the Community graph \n");
	for(i=0;i<comm_csr->v_count;i++)
    {
    	
    	for(j=comm_csr->vptr[i];j<comm_csr->vptr[i+1];j++)
    	{
    		if(j<=comm_csr->e_count)
    		{
    			printf("\n%d -- %d -- %d\n", i, comm_csr->eptr[j],comm_csr->edwt[j]);
			}
    		
		}
		
		
	}
	*/
	
	for(i = 0; i < comm_csr->v_count; i++) free(adj[i]);
    free(adj); 
    printf("\n Exit communityGraph\n");

	return comm_csr;
}
/*bfs to find the shortest path and closeness centrality for community graph*/
float comm_bfs(Graph *graph,int s)
{
	/*printf("\n Enter comm_bfs \n");*/
    int p,*d,*q,i,c=0,front=-1,rear=-1,u,v;
	long long sum=0;
    float cl;

	d=(int*)calloc(graph->v_count,sizeof(int));
	q=(int*)calloc(graph->v_count,sizeof(int));
	for(i=0;i<graph->v_count;i++){
		d[i]=-1;
		
	}
	d[s]=0;
	
     q[++rear]=s; /*enqueue*/
	while(front!= rear)
	{
	   u=q[++front]; /*dequeue*/

	   c=c+1;
	   for(i=graph->vptr[u];i<graph->vptr[u+1];i++)
	   {
		    v=graph->eptr[i];
		
		    if(d[v]<0)
			{
				d[v]=d[u]+1;
				
	        	q[++rear]=v;
		
    	    }
      
	   }
	
 }
	/* printf("\n%d",c);*/
	
	
	for(i=0;i<graph->v_count;i++){
		if(d[i]!=-1)
		{
		
		/*printf("\nDistance from node %d to node %d is :%d",s,i,d[i]);*/
		  sum+=d[i];
	   }
	}
    
	if (sum == 0 || c <= 1) return 0.0f;
    cl = (float)(c - 1) / (float)sum;
	free(d);
    free(q);
    /*printf("\n Exit commbfs \n");*/
    return cl;
}

/*calculate and store the closeness centrality of each node  with respect to community*/


void comm_closenessCentrality(Graph *graph)
{
	printf("\n Enter comm_closenessCentrality \n");
	int i;
	 graph->clc = (float*)calloc(graph->v_count, sizeof(float));
     if (!graph->clc) 
	 {
              printf("Memory allocation failed for community closeness centrality.\n");
              exit(1);
     }
	
		
	for(i=0;i<graph->v_count;i++)
	{
		float clc_val = comm_bfs(graph, i);
        graph->clc[i] = clc_val;
        
	   
    }
    /*
    printf("\nCloseness Centrality:\n");
	for(i=0;i<graph->v_count;i++)
	{
		printf("\n%d---%f",i,graph->clc[i]);
	}
	*/
    printf("\n Exit comm_closenessCentrality\n");
}
int* rankCommunityNodes(Graph *graph, Graph *comm_csr, int top_k) {
    printf("\nEnter rankCommunityNodes\n");
    int i, j;

    int *comm_index = (int*) malloc(comm_csr->v_count * sizeof(int));
    if (comm_index == NULL) return NULL;
    for (i = 0; i < comm_csr->v_count; i++) {
        comm_index[i] = i;
    }

    /* Sort communities by closeness centrality (descending)*/
    for (i = 0; i < comm_csr->v_count - 1; i++) {
        for (j = i + 1; j < comm_csr->v_count; j++) {
            if (comm_csr->clc[comm_index[i]] < comm_csr->clc[comm_index[j]]) {
                int temp = comm_index[i];
                comm_index[i] = comm_index[j];
                comm_index[j] = temp;
            }
        }
    }

    /* Compute node counts per community*/
    int *community_counts = (int*)calloc(comm_csr->v_count, sizeof(int));
    if (community_counts == NULL) { /* Error handling */ free(comm_index); return 0; }
    for (i = 0; i < graph->v_count; i++) {
        community_counts[graph->comm[i]]++;
    }

    int C = comm_csr->v_count;
    float W = (float)(C * (C + 1)) / 2.0; /*the rank weight*/

    int *nodes_to_select_per_community = (int*)calloc(comm_csr->v_count, sizeof(int));
    if (nodes_to_select_per_community == NULL) { /* Error handling */ free(comm_index); free(community_counts); return 0; }

    int remaining_top_k = top_k;

    /* Calculate initial proportional allocation*/
    for (i = 0; i < C; i++) {
        int rank = i + 1;
        int cid = comm_index[i];
        float weight = (float)(C - i) / W;
        /*Ensure we don't try to select more nodes than available in the community*/
        nodes_to_select_per_community[cid] = round(top_k * weight);
        if (nodes_to_select_per_community[cid] > community_counts[cid]) {
            nodes_to_select_per_community[cid] = community_counts[cid];
        }
    }

    /*Adjust to ensure exactly top_k nodes are selected (handle rounding issues)*/
    int current_selected = 0;
    for (i = 0; i < C; i++) {
        current_selected += nodes_to_select_per_community[i];
    }

    /*Distribute remaining nodes if current_selected is less than top_k*/
    int diff = top_k - current_selected;
    /*printf("Diff=%d",diff);*/
    do
	{    
    if (diff > 0) 
	{
        for (i = 0; i < C && diff > 0; i++) 
		{
            int cid = comm_index[i]; /*Iterate through sorted communities*/
            if (nodes_to_select_per_community[cid] < community_counts[cid])
			{
                nodes_to_select_per_community[cid]++;
                diff--;
            }
        }
    } 
	else if (diff < 0) 
	{ 
	/* If current_selected is more than top_k, remove from lowest ranked communities*/
        for (i = C - 1; i >= 0 && diff < 0; i--) {
            int cid = comm_index[i];
            if (nodes_to_select_per_community[cid] > 0) {
                nodes_to_select_per_community[cid]--;
                diff++;
            }
        }
    }
    else break;
    }while(diff!=top_k);

    printf("Total no. of community : %d\n",C);
    printf("\nTop-k = %d\n", top_k);
    
    printf("\nCommunity Ranking & Nodes Selected:\n");
    for (i = 0; i < C; i++)
	{
        int rank = i + 1;
        int cid = comm_index[i];
        if(nodes_to_select_per_community[cid]>0)
                printf("Community %d (Centrality=%.3f, Rank=%d): Select %d nodes out of %d\n",cid, comm_csr->clc[cid], rank, nodes_to_select_per_community[cid], community_counts[cid]);
    }
    
    int final_selected_count = 0;
    for(i = 0; i < C; i++) {
        final_selected_count += nodes_to_select_per_community[i];
    }
    printf("\nTotal selected: %d\n", final_selected_count);

    free(community_counts);
    free(comm_index);
     printf("\nExit rankCommunityNodes\n");
    return nodes_to_select_per_community;

   
}


/* Complete and corrected compositeSeedSel() implementation*/
void compositeSeedSel(Graph *graph, int s[], int t[], int nodes_to_select_per_community[], int v_count)
{
     int i,j;
    int *selected_per_community = (int *)calloc(graph->v_count, sizeof(int));
    if (!selected_per_community) {
        printf("Memory allocation failed for selection tracking.\n");
        return;
    }

    /* Create an array of node indices and their composite scores*/
    typedef struct {
        int node;
        float score;
    } NodeScore;

    NodeScore *node_scores = malloc(graph->v_count * sizeof(NodeScore));
    for (i = 0; i < graph->v_count; i++) {
        node_scores[i].node = i;
        node_scores[i].score = graph->composite_score[i];
    }

    /* Sort nodes in descending order of composite score*/
    for (i = 0; i < graph->v_count - 1; i++) {
        for (j = i + 1; j < graph->v_count; j++) {
            if (node_scores[i].score < node_scores[j].score) {
                NodeScore temp = node_scores[i];
                node_scores[i] = node_scores[j];
                node_scores[j] = temp;
            }
        }
    }

    /* Select top nodes based on nodes_to_select_per_community[]*/
    for (i = 0; i < graph->v_count; i++) {
        int v = node_scores[i].node;
        int comm = graph->comm[v];
        if (selected_per_community[comm] < nodes_to_select_per_community[comm]) {
            s[v] = 1; /* rumor seed*/
            t[v] = 0;
            selected_per_community[comm]++;
        }
    }
    /*
    printf("\nThe seed nodes are:\n");
    for(i=0;i<graph->v_count;i++)
    {
    	if(s[i]==1)
    	  printf("\n%d",i);
	}
    */
    free(node_scores);
    free(selected_per_community);
}
                   

void icm(int h_v[], int h_e[], int s[], int t[], int iter, int v_count, int e_count, int *h_continue, int rumor_flag[])			
{
    int i,p;
    /*printf("\nNodes selected in  iteration %d:\n",iter);*/
	for(i=0;i<v_count;i++)
	{
	
		if((s[i]==1) && (rumor_flag[i]!=1))
		{
			rumor_flag[i]=1;

			int end = (i==v_count-1)?2*e_count:h_v[i+1];

			for(p = h_v[i]; p < end; p++) 
			{
				int w = h_e[p];
				double scaled= (double)rand()/RAND_MAX;
         		double prob= 0.00 + scaled * (1.00 - 0.00);
				
      			if((prob<=0.05) && (s[w]==0))
				{
					s[w]=1;
					/*printf("%d\n",w);*/
					t[w]=iter;
					*h_continue = 1;

				}
			}

		}

		   
     }

}





int get_infect(int s[], int v_count)
{

	int i,count=0;
	for(i=0;i<v_count;i++)
	{

		if(s[i]==1)
		 count++;

	}	

	return count;
}

void rumor_antirumor(Graph *graph, int s[], int t[], int nodes_to_select_per_community[], char *ifile, int rumor_flag[]) {
    
    int i;
    int v_count = graph->v_count;
    int e_count = graph->e_count;
    int *h_v = graph->vptr;
    int *h_e = graph->eptr;
    
    srand(time(0));
    struct timeval tt1,tt2;

	/* Intial Seed Selection */

	
	gettimeofday(&tt1, 0);

    compositeSeedSel(graph, s, t, nodes_to_select_per_community, v_count);

    int *r = (int *) malloc(sizeof(int) * MAX);
    r[0] = get_infect(s, v_count);
   

      
    printf("\niter=0 --> %d",r[0]);
    int h_continue;
    int iter = 1;

    do {
        h_continue = 0;
        

        icm(h_v, h_e, s, t, iter, v_count, e_count, &h_continue, rumor_flag);
        
        if(h_continue >0)
        {
        	r[iter] = get_infect(s, v_count);
            printf("\niter=%d --> %d", iter,r[iter]);
            iter++;
        	
		}

        

    } while (h_continue);
    int total_influence=r[iter-1];  

    gettimeofday(&tt2, 0);

	double elapsed_time = (1000000.0*(tt2.tv_sec - tt1.tv_sec) + tt2.tv_usec - tt1.tv_usec)/1000000.0;
    /*calculating the computation time */

   

    FILE *fp;
    char oname[100];
    snprintf(oname, sizeof(oname), "result_%s", ifile);
    fp = fopen(oname, "w");
   

    for (i = 0; i < iter; i++) {
    	     
        fprintf(fp, "%d %d\n", i, r[i]);
    }

    fclose(fp);
    printf("\nTotal number of nodes influenced: %d : Out of : %d\n",total_influence,v_count);
    printf("\nComputation time in CPU = %lf secs\n",elapsed_time);
   
    free(r);
    
    
}

void mark_top_k_influencers(Graph* graph, int k) {
    int n = graph->v_count;
    int i,j;
    /* Create index array */
    int* indices = (int*)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) indices[i] = i;

    /* Sort indices by composite_score in descending order */
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (graph->composite_score[indices[j]] > graph->composite_score[indices[i]]) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }

    /* Mark top-k as seeds */
    for (i = 0; i < k && i < n; i++) {
        graph->seed[indices[i]] = 1;
    }

    free(indices);
}

float computeAPL(Graph* graph, int* seed) {
    int n = graph->v_count;
    long long total_dist = 0;
    long long connected_pairs = 0;
    int connected_components = 0;
    int i,j,src;
    int* global_visited = (int*)calloc(n, sizeof(int));

    for (src = 0; src < n; src++) {
        if (seed[src] || global_visited[src]) continue;

        /* Start BFS */
        int* visited = (int*)calloc(n, sizeof(int));
        int* dist = (int*)malloc(n * sizeof(int));
        int* queue = (int*)malloc(n * sizeof(int));
        int front = 0, rear = 0;

        for ( i = 0; i < n; i++) {
            visited[i] = 0;
            dist[i] = -1;
        }

        queue[rear++] = src;
        visited[src] = 1;
        dist[src] = 0;
        global_visited[src] = 1;

        while (front < rear) {
            int u = queue[front++];
            for (j = graph->vptr[u]; j < graph->vptr[u + 1]; j++) {
                int v = graph->eptr[j];
                if (!seed[v] && !visited[v]) {
                    visited[v] = 1;
                    dist[v] = dist[u] + 1;
                    global_visited[v] = 1;
                    queue[rear++] = v;
                }
            }
        }

        connected_components++;

        for (i = 0; i < n; i++) {
            if (!seed[i] && i != src && dist[i] != -1) {
                total_dist += dist[i];
                connected_pairs++;
            }
        }

        free(visited);
        free(dist);
        free(queue);
    }

    free(global_visited);

    if (connected_components > 1) {
        printf("Graph is disconnected after removing seed nodes.\n");
        return -1.0;
    }

    return connected_pairs > 0 ? (float)total_dist / connected_pairs : 0.0;
}

int LCC(Graph* graph, int* seed) {
    int n = graph->v_count;
    int* visited = (int*)calloc(n, sizeof(int));
    int max_size = 0;
    int src,i;
    for ( src = 0; src < n; src++) {
        if (seed[src] || visited[src]) continue;

        int* queue = (int*)malloc(n * sizeof(int));
        int front = 0, rear = 0, size = 0;

        queue[rear++] = src;
        visited[src] = 1;

        while (front < rear) {
            int u = queue[front++];
            size++;

            for ( i = graph->vptr[u]; i < graph->vptr[u + 1]; i++) {
                int v = graph->eptr[i];
                if (!visited[v] && !seed[v]) {
                    visited[v] = 1;
                    queue[rear++] = v;
                }
            }
        }

        if (size > max_size)
            max_size = size;

        free(queue);
    }

    free(visited);
    return max_size;
}


Graph* createGraph(char* file_name,char* f2){
	
	return CSR(file_name,f2);
	
}

int main(int argc,char *argv[]) 
{
	int i,j,c,b_count,top_k ;
    char *filename = argv[1]; /* Replace with your file name*/
    char *f2=argv[2];
    
    /* Create a graph from the file*/
    Graph *graph = createGraph(filename,f2);

    if (graph == NULL)
	{
        printf("Error creating graph.\n");
        return 1;
    }
     

    
    printf("Number of vertices: %d\n", graph->v_count);
    printf("Number of edges: %d\n", graph->e_count);
    
    c=2*graph->e_count;
    
    /*printf("Edge List:\n");
    
    for (i = 0; i < graph->v_count; i++)
	{
    	
        for (j = graph->vptr[i]; j<graph->vptr[i+1]; j++) 
		{
              
			  printf("%d -- %d\n", i, graph->eptr[j]);
			  
           
        }
        
        
    }
    
    
	
	printf("Community List:\n");
    for(i=0;i<graph->v_count;i++)
    {
    	printf("%d ->  %d\n",i,graph->comm[i]);
    	
	}
    */
	b_count=boundary(graph);
    printf("\nNo.of boundary nodes: %d\n",b_count);
    
    int v_count=graph->v_count;
    int e_count=graph->e_count;  
    int *h_v=(int *) malloc(sizeof(int)*v_count);
    int *h_e=(int *) malloc(sizeof(int)*2*e_count);
    for(i=0;i<v_count;i++)
      h_v[i]=graph->vptr[i];

    for(i=0;i<2*e_count;i++)
      h_e[i]=graph->eptr[i];


    int *s = calloc(v_count, sizeof(int)); /* 0: unvisited, 1: rumor, 2: anti-rumor*/
    int *t = calloc(v_count, sizeof(int)); /*iteration each node got infected*/
    int *rumor_flag = calloc(v_count, sizeof(int)); 
   
    
    boundary_score(graph);
    closenessCentrality(graph);
    composite_score(graph);
    /*median_score(graph);*/
    
    Graph *comm_csr = communityGraph(graph);

    if (comm_csr == NULL)
	{
        printf("Error creating graph.\n");
        return 1;
    }
	
    comm_closenessCentrality(comm_csr);
    int percent;
    printf("Enter the percentage of seed nodes: \n");
    scanf("%d", &percent);
    top_k = (percent * graph->v_count) / 100;	/* You can set this as desired or take as input*/
    printf("Top k selected nodes = %d\n",top_k);
    mark_top_k_influencers(graph, top_k);
    
    int *nodes_to_select_per_community=rankCommunityNodes(graph, comm_csr, top_k);
    for(i=0;i<v_count;i++)
    {
        
	    s[i]=0;
	    t[i]=0;
        rumor_flag[i]=0;
	    
    }
  
  
   
  
    printf("\n\n");

   
    rumor_antirumor(graph,s,t,nodes_to_select_per_community,argv[1],rumor_flag);
     
    
    float apl = computeAPL(graph, graph->seed);
    if(apl!= -1.0)
    printf("APL after removing top %d%% influencers: %f\n", percent, apl);
    int lcc_size = LCC(graph, graph->seed);
    printf("Largest Connected Component size (ignoring seed nodes): %d\n", lcc_size);

    free(graph->border_score);
    free(graph->composite_score);
    free(graph->clc);
    free(graph->comm);
    free(graph->vptr);
    free(graph->eptr);
    free(graph->boundary);
    free(graph);
    
    free(comm_csr->clc);
    free(comm_csr->vptr);
    free(comm_csr->eptr);
    free(comm_csr);
    
    free(nodes_to_select_per_community);
    free(rumor_flag);
   
    free(h_v);
    free(h_e);
    free(s);
    free(t);

    return 0;
}

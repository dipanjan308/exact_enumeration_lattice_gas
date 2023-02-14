/*This code enumerates all configurations for the system of hard dimers and 2 X 2 squares on square lattice.-------written by Dipanjan Mandal, dipkar.308@gmail.com*/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define L 4
#define N (L*L)
#define PS 3//number of particle types (hd, vd and square)
#define MAX 1000000//maximum dimer states

int lat[N],ln[N],rn[N],tn[N],bn[N],con_s[N/2];
int stored_states[MAX][N/2],stored_states2[MAX][N/2];
char outfile[100];

//Given x and y it returns site index
int find_site(int x, int y)
{
	x=(x+L) % L;
	y=(y+L) % L;
	return x+y*L;
}
//Gives neighbour site index
void neighbour_index()
{
	int i,j,s,n,k;
	for(i=0;i<L;i++)
	{
		for(j=0;j<L;j++)
		{
			s=find_site(i,j);	
			n=find_site(i+1,j);	rn[s]=n;
			n=find_site(i-1,j);	ln[s]=n;
			n=find_site(i,j+1);	tn[s]=n;
			n=find_site(i,j-1);	bn[s]=n;
		}
	}
}
void initialize_lat()
{
	int i;
	for(i=0;i<N;i++)
	{
		lat[i]=0;
	}
}
void initialize_stored_states()
{
	int i,j;
	for(j=0;j<N/2;j++)
	{
		for(i=0;i<MAX;i++)
		{
			stored_states[i][j]=0;
		}
	}
}
void initialize_stored_states2()
{
	int i,j;
	for(j=0;j<N/2;j++)
	{
		for(i=0;i<MAX;i++)
		{
			stored_states2[i][j]=0;
		}
	}
}
//deposit horizontal dimer
int deposit_hd(int site)
{
	int s;
	if(lat[site]==0 && lat[rn[site]]==0)
	{
		lat[site]=2;
		lat[rn[site]]=1;
		s=1;
	}
	else
	{
		s=0;
	}
	return s;
}
//deposit vertical dimer
int deposit_vd(int site)
{
	int s;
	if(lat[site]==0 && lat[tn[site]]==0)
	{
		lat[site]=4;
		lat[tn[site]]=3;
		s=1;
	}
	else
	{
		s=0;
	}
	return s;
}
//deposit square
int deposit_sq(int site)
{
	int s;
	if(lat[site]==0 && lat[tn[site]]==0 && lat[rn[site]]==0 && lat[rn[tn[site]]]==0)
	{
		lat[site]=6;
		lat[tn[site]]=5;
		lat[rn[site]]=5;
		lat[rn[tn[site]]]=5;
		s=1;
	}
	else
	{
		s=0;
	}
	return s;
}
//one dimer states
int prepare_single_particle_states(int con)
{
	int i,s;
	if(con%PS==1)
	{
		s=deposit_hd((con-1)/PS);
	}
	else if(con%PS==2)
	{
		s=deposit_vd((con-1)/PS);
	}
	else if(con%PS==0)
	{
		s=deposit_sq((con-1)/PS);
	}
	return s;
}
void go_to_next_state(int node)
{
	con_s[node]++;
}
void print_lattice()
{
	int i,j;
	for(j=L-1;j>=0;j--)
	{
		for(i=0;i<L;i++)
		{
			printf("%d\t",lat[i+j*L]);
		}
		printf("\n");
	}
}
//store current state to given position
void store_current_state(int pos)
{
	int i;
	for(i=0;i<N/2;i++)
	{
		stored_states[pos][i]=con_s[i];
	}
}
void store_current_state2(int pos)
{
	int i;
	for(i=0;i<N/2;i++)
	{
		stored_states2[pos][i]=con_s[i];
	}
}
//restore previous node states
void restore_previous_node_state(int node, int pos)
{
	int i,s;
	initialize_lat();
	for(i=0;i<node;i++)
	{
		s=prepare_single_particle_states(stored_states[pos][i]);
		if(s==0)
		{
			printf("ERROR!\n");
			exit(0);
		}
	}
	for(i=0;i<node;i++)
	{
		con_s[i]=stored_states[pos][i];
	}
}
void move_stored_states(int pos)
{
	int i,j;
	initialize_stored_states();
	for(j=0;j<pos;j++)
	{
		for(i=0;i<N/2;i++)
		{
			stored_states[j][i]=stored_states2[j][i];
		}
	}
	initialize_stored_states2();
}
int main()
{
	int i,j,node;
	int count,max_con_node;
	neighbour_index();
	initialize_stored_states();
	initialize_stored_states2();
	initialize_lat();
	for(i=0;i<N/2;i++)
	{
		con_s[i]=0;
	}
	con_s[0]=1;
	node=0;
	count=0;
	//store single particle states (node=0)
	while(con_s[node]<=PS*N)
	{
		if(prepare_single_particle_states(con_s[node])==1)
		{
			count++;
			store_current_state(count-1);			
			go_to_next_state(node);
			initialize_lat();
		}
	}
	max_con_node=count;

	node++;
	printf("dimers+squares=%d\t%d\n",node,count);
	//N/2 is the maximum number of particles in the system
	while(node<N/2)
	{
		count=0;
		for(i=0;i<max_con_node;i++)	
		{
			restore_previous_node_state(node,i);
			if(con_s[node-1]<PS*N)
			{
				con_s[node]=con_s[node-1]+1;
				while(con_s[node]<=PS*N)
				{
					if(prepare_single_particle_states(con_s[node])==1)
					{
						count++;
						store_current_state2(count-1);
						//print_lattice();
						//printf("\n");
						go_to_next_state(node);
						restore_previous_node_state(node,i);
					}
					else
					{
						go_to_next_state(node);
					}
				}
			}
		}
		move_stored_states(count);
		max_con_node=count;
		node++;
		printf("dimers+squares=%d\t%d\n",node,count);
	}
	return(0);
}

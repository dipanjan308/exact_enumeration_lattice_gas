/*This code enumerates all configurations for the system of 1NN model on square lattice using depth-first algorithm.-------written by Dipanjan Mandal, dipkar.308@gmail.com*/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define L 4
#define N (L*L)
#define PS 1//number of particle types
#define MAX (N/2)//maximum number of particles
#define MS (N*PS)//highest number of single particle state

int lat[N],ln[N],rn[N],tn[N],bn[N],con_s[MS];
int flag=1;//flag for successful move_particle and put_particle
long states[MAX+1];

//Given x and y it returns site index
int find_site(int x, int y)
{
	x=(x+L)%L;
	y=(y+L)%L;
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
//deposit 1NN particle
int deposit_1NN(int site)
{
	int s;
	if(lat[site]==0)
	{
		lat[site]++;
		lat[rn[site]]--;
		lat[ln[site]]--;
		lat[tn[site]]--;
		lat[bn[site]]--;
		s=1;
	}
	else
	{
		s=0;
	}
	return s;
}
//delete 1NN particle
void delete_1NN(int site)
{
	lat[site]--;
	lat[rn[site]]++;
	lat[ln[site]]++;
	lat[tn[site]]++;
	lat[bn[site]]++;
}
//single dimer states
int prepare_single_particle_state(int con)
{
	int s;
	s=deposit_1NN(con-1);
	return s;
}
//delete particle from node
void delete_particle(int node)
{
	int site;
	site=(con_s[node]-1);
	if(lat[site]==1)
	{
		delete_1NN(site);
	}
	else
	{
		printf("ERROR!\n");
		exit(0);
	}	
}
//put particle in a given node
void put_particle(int node)
{
	int con,s;
	con=con_s[node-1]+1;
	while(con<=MS)
	{
		flag=prepare_single_particle_state(con);
		if(flag==0)
		{
			con++;
		}
		else
		{
			con_s[node]=con;
			break;
		}
	}
	if(con>MS)
	{
		flag=0;
	}
}
void put_particle_after_move(int node)
{
	int con,s;
	con=con_s[node]+1;
	while(con<=MS)
	{
		flag=prepare_single_particle_state(con);
		if(flag==0)
		{
			con++;
		}
		else
		{
			con_s[node]=con;
			break;
		}
	}
	if(con>MS)
	{
		flag=0;
	}
}
//move particle from node state
void move_particle(int node)
{
	int site;
	delete_particle(node);
	put_particle_after_move(node);
	if(flag==0)
	{
		site=(con_s[node]-1);
		deposit_1NN(site);
	}
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
int count_particle()
{
	int i,count=0;
	for(i=0;i<N;i++)
	{
		if(lat[i]==1)
		{
			count++;
		}
	}
	return count;
}
int main()
{
	char outfile[100];
	int i,node,next;
	long sm=0;
	//next=0(put), next=1(move), next=2(delete)
	neighbour_index();
	initialize_lat();
	for(i=0;i<MS;i++)
	{
		con_s[i]=0;
	}
	for(i=0;i<=MAX;i++)
	{
		states[i]=0;
	}
	con_s[0]=1;
	node=0;
	flag=prepare_single_particle_state(con_s[node]);
	states[1]++;
	next=0;
	while(count_particle()!=0)
	{
		if(next==0)
		{
			node++;
			if(node>=MAX)
			{
				flag=0;
				next=1;
				node--;
			}
			else
			{
				put_particle(node);
				if(flag==1)
				{
					next=0;
				}
				else
				{
					next=1;
					node--;
				}
			}
		}
		else if(next==1)
		{
			move_particle(node);
			if(flag==1)
			{
				next=0;
			}
			else
			{
				next=2;
			}
		}
		else if(next==2)
		{
			delete_particle(node);
			node--;
			flag=0;
			next=1;
		}
		if(flag==1)
		{
			//printf("1NN number=%d\n",node+1);
			//print_lattice();
			states[node+1]++;
		}
	}

	FILE *fp;
	sprintf(outfile,"DoS_1NN_square_L%d",L);
	fp=fopen(outfile,"w");
        fprintf(fp,"#1NN\tconfigurations\n");
        fclose(fp);

	states[0]=1;
	fp=fopen(outfile,"a");
	for(i=0;i<=MAX;i++)
	{
		sm+=states[i];
		fprintf(fp,"%d\t%ld\n",i,states[i]);
	}
	fprintf(fp,"Total configurations=%ld.\n",sm);
	fclose(fp);
	return(0);
}

#include <stdio.h>
#include <stdlib.h>

#define MAX_JOBS 100
#define MAX_SERVERS 20

typedef struct {
    int id;
    int cpu;
    int memory;
    int storage;
} Job;

typedef struct {
    int id;
    int cpu_available;
    int memory_available;
    int storage_available;
    int total_load; 
} Server;

Job jobs[MAX_JOBS];
Server servers[MAX_SERVERS];
int job_assignment[MAX_JOBS]; 

int compare_job_demand(const void *a, const void *b) {
    Job *jobA = (Job *)a;
    Job *jobB = (Job *)b;
    int demandA = jobA->cpu + jobA->memory + jobA->storage;
    int demandB = jobB->cpu + jobB->memory + jobB->storage;
    return demandB - demandA; 
}

int compare_server_load(const void *a, const void *b) {
    Server *sA = (Server *)a;
    Server *sB = (Server *)b;
    return sA->total_load - sB->total_load; 
}

int fits(Job job, Server server) {
    return job.cpu <= server.cpu_available &&
           job.memory <= server.memory_available &&
           job.storage <= server.storage_available;
}

void greedy_allocation(int num_jobs, int num_servers) {
    for (int i = 0; i < num_jobs; i++) {
        job_assignment[i] = -1;
        for (int j = 0; j < num_servers; j++) {
            if (fits(jobs[i], servers[j])) {
                servers[j].cpu_available -= jobs[i].cpu;
                servers[j].memory_available -= jobs[i].memory;
                servers[j].storage_available -= jobs[i].storage;
                job_assignment[i] = servers[j].id;
                break;
            }
        }
    }
}

void ffd_allocation(int num_jobs, int num_servers) {
    qsort(jobs, num_jobs, sizeof(Job), compare_job_demand);
    for (int i = 0; i < num_jobs; i++) {
        job_assignment[i] = -1;
        for (int j = 0; j < num_servers; j++) {
            if (fits(jobs[i], servers[j])) {
                servers[j].cpu_available -= jobs[i].cpu;
                servers[j].memory_available -= jobs[i].memory;
                servers[j].storage_available -= jobs[i].storage;
                job_assignment[i] = servers[j].id;
                break;
            }
        }
    }
}

void min_heap_allocation(int num_jobs, int num_servers) {
    for (int i = 0; i < num_servers; i++) {
        servers[i].total_load = 0;
    }

    for (int i = 0; i < num_jobs; i++) {
        job_assignment[i] = -1;
        qsort(servers, num_servers, sizeof(Server), compare_server_load);
        for (int j = 0; j < num_servers; j++) {
            if (fits(jobs[i], servers[j])) {
                servers[j].cpu_available -= jobs[i].cpu;
                servers[j].memory_available -= jobs[i].memory;
                servers[j].storage_available -= jobs[i].storage;
                servers[j].total_load += jobs[i].cpu + jobs[i].memory + jobs[i].storage;
                job_assignment[i] = servers[j].id;
                break;
            }
        }
    }
}

void print_results(int num_jobs) {
    printf("[\n");
    for (int i = 0; i < num_jobs; i++) {
        printf("  {\"job_id\": %d, \"assigned_server\": %d}%s\n", jobs[i].id, job_assignment[i], (i < num_jobs - 1) ? "," : "");
    }
    printf("]\n");
}

int main() {
    int num_jobs, num_servers;
    int algorithm_choice;

    scanf("%d %d %d", &num_jobs, &num_servers, &algorithm_choice);
    for (int i = 0; i < num_jobs; i++) {
        scanf("%d %d %d %d", &jobs[i].id, &jobs[i].cpu, &jobs[i].memory, &jobs[i].storage);
    }
    for (int i = 0; i < num_servers; i++) {
        scanf("%d %d %d %d", &servers[i].id, &servers[i].cpu_available, &servers[i].memory_available, &servers[i].storage_available);
    }

    switch (algorithm_choice) {
        case 1:
            greedy_allocation(num_jobs, num_servers);
            break;
        case 2:
            ffd_allocation(num_jobs, num_servers);
            break;
        case 3:
            min_heap_allocation(num_jobs, num_servers);
            break;
        default:
            printf("Invalid algorithm selection\n");
            return 1;
    }

    print_results(num_jobs);
    return 0;
}
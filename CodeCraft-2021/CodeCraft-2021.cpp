#include <iostream> 
#include <string> 
#include <map> 
#include <stdlib.h> 
#include <cmath>
#include <vector>
#include <float.h> 
using namespace std; 
 
// #define DEBUG_POINT 
// #define TESTINFO 
//#define FILEINPUT 
//#define MI
#define SLOVED 1 
#define UNSLOVED 0 
#define MAX_SERVER_NUM 6000 
#define ADD_SERVER_NUM 1000 
#define BUY_CPU_MEM_HIGH  1.8
#define BUY_CPU_MEM_LOW 0.6

long long TOTALCOST = 0,SERVERCOST=0,DAYCOST=0;

struct request 
{ 
    // 1��ʾ������ 0��ʾ����δ���  
    int sloved; 
    // 1Ϊadd  2Ϊdel   
    int request; 
    //��������ƶ�Ӧ�����������  
    int vm_type; 
    //�������Id  
    int vm_id; 
    int server_type; 
    //��ʾ������ŵķ�����Id  
    int server_id; 
    //��ʾ�ŵĽڵ�  
    int node; 
};  
struct server_info 
{ 
    string name; //����  
    int count; //�������������������  
    int cpu_core; // cpu����  
    int memory; // �ڴ��С  
    int machine_cost; // Ӳ���ɱ�  
    int energy_cost; // ��Դ����   
    double cpu_memory; // ÿ��CPU����ռ�õ��ڴ�  
}; 
struct server_instance 
{ 
    int type_id; // ������Id(������) 
    double  A_cpu_access;  // ������A��CPU�ܽ��ܵĺ���  
    double B_cpu_access; //������B��CPU�ܽ��ܵĺ���  
    double A_memory_access; //������A���ڴ��ܽ��ܵĴ�С   
    double B_memory_access;//������B���ڴ��ܽ��ܵĴ�С 
    double A_cpu_usage;
	double A_memory_usage;
	double B_cpu_usage;
	double B_memory_usage;
}; 
struct vm_info 
{ 
    int cpu_core; //�����CPU����  
    int memory; // ������ڴ���  
    int is_double_node; // ������Ƿ�˫�ڵ�  
    double cpu_memory; // ÿ��CPU����ռ�õ��ڴ�  
}; 
struct vm_instance 
{ 
    int type_id;  //��������Id  
    int server_id;  // ��������ڷ�����Id  
    int node;  //������Ƿ�˫�ڵ����  0Ϊ˫�� 1ΪA  2ΪB 
}; 
struct server_vm
{
	vector<int> A_instance;
	vector<int> B_instance;
	vector<int> double_instance; 
}; 
struct migration_info{
	int target_serverId;
	int mi_vm_id;
	int target_node;
};

int max_server_num; //����������  
int server_type_num;   
int server_instance_num; 
int vm_type_num; 
int vm_instance_num; 
server_info *server_type_list;            // ����������Ϣ  
server_instance *server_instance_list;   // �÷������ܹ���Ŷ��ٵ���Ϣ 
//int migration_info_sum;
vm_info *vm_type_list; //���������Ϣ  
//��ŷ��������� ����  
map<string, int> server_type_index; 
// ��� ��������� ��Ӧ����  
map<string, int> vm_type_index; 
// id  �������ŵ���Ϣ  
map<int, vm_instance> vm_instance_map;
//������Id �ڵ��Ϸֲ�������� 
map<int, server_vm> server_vm_map;  

int total_day;  
double *sort_cpu_memory;    //����� ��������ÿ��CPU���е��ڴ���  
int *sort_server_type_id;  //����� ÿ��CPU�����ڴ��С��С�ķ�����Id  
int mi_num = 0;
map<int,migration_info> migration_info_map;
int *total_server_id;
double *total_server_usage;

#ifdef TESTINFO
void print_usage();
#endif 
 
void init_c() 
{ 
    //������������   MAX_SERVER_NUM 5000 
    max_server_num = MAX_SERVER_NUM; 
    // ��������������  
    server_type_num = 0; 
    // ���õķ���������  
    server_instance_num = 0; 
    //���������������  
    vm_type_num = 0; 
    // ���õ����������  
    vm_instance_num = 0; 
    // ָ��������ܴ�Ŷ��ٵ���Ϣ��ָ��  
    server_instance_list = new server_instance[max_server_num]; 
    //Ǩ�Ƽ�¼����
	//int migration_info_sum = 0; 
	total_server_usage = new double[max_server_num];
	total_server_id = new int[max_server_num];
} 
 
void init_server_type_list() 
{ 
    //���� ���Բɹ��ķ�������������  
    cin >> server_type_num; 
    //���õķ�������Ϣ  
    server_type_list = new server_info[server_type_num]; 
    //����ķ�����cpu/memory  
    sort_cpu_memory = new double[server_type_num]; 
    //�����ķ��������� 
    sort_server_type_id = new int[server_type_num]; 
     
    char tmp_char = 0; 
    int tmp_cpu = 0; 
    int tmp_memory = 0; 
    int tmp_machine_cost = 0; 
    int tmp_energy_cost = 0; 
    double tmp_cpu_memory = 0; 
    string tmp_name; 
    for (int i = 0; i < server_type_num; i++) 
    { 
        cin >> tmp_char; 
        cin >> tmp_name; 
        cin >> tmp_cpu; 
        cin >> tmp_char; 
        cin >> tmp_memory; 
        cin >> tmp_char; 
        cin >> tmp_machine_cost; 
        cin >> tmp_char; 
        cin >> tmp_energy_cost; 
        cin >> tmp_char; 
        tmp_name.pop_back(); 
        tmp_cpu_memory = ((double)tmp_cpu) / ((double)tmp_memory); 
        server_type_index[tmp_name] = i; 
        server_type_list[i].name = tmp_name; 
        server_type_list[i].count = 0; 
        server_type_list[i].cpu_core = tmp_cpu; 
        server_type_list[i].memory = tmp_memory; 
        server_type_list[i].machine_cost = tmp_machine_cost; 
        server_type_list[i].energy_cost = tmp_energy_cost; 
        server_type_list[i].cpu_memory = tmp_cpu_memory; 
        sort_server_type_id[i] = i; 
        sort_cpu_memory[i] = tmp_cpu_memory; 
    } 
} 
 
void init_vm_type_list() 
{       
    cin >> vm_type_num; 
    vm_type_list = new vm_info[vm_type_num]; 
    char tmp_char = 0; 
    int tmp_cpu = 0; 
    int tmp_memory = 0; 
    int tmp_double_node; 
    double tmp_cpu_memory = 0; 
    string tmp_name; 
    for (int i = 0; i < vm_type_num; i++) 
    { 
        cin >> tmp_char; 
        cin >> tmp_name; 
        cin >> tmp_cpu; 
        cin >> tmp_char; 
        cin >> tmp_memory; 
        cin >> tmp_char; 
        cin >> tmp_double_node; 
        cin >> tmp_char; 
        tmp_name.pop_back(); 
        tmp_cpu_memory = ((double)tmp_cpu) / ((double)tmp_memory); 
        vm_type_index[tmp_name] = i; 
        vm_type_list[i].cpu_core = tmp_cpu; 
        vm_type_list[i].memory = tmp_memory; 
        vm_type_list[i].is_double_node = tmp_double_node; 
        vm_type_list[i].cpu_memory = tmp_cpu_memory; 
    } 
} 


void refresh(int server_instance_id) {
	server_instance_list[server_instance_id].A_cpu_usage = (double)(server_type_list[server_instance_list[server_instance_id].type_id].cpu_core / 2 - server_instance_list[server_instance_id].A_cpu_access) / (double)(server_type_list[server_instance_list[server_instance_id].type_id].cpu_core / 2);
	server_instance_list[server_instance_id].B_cpu_usage = (double)(server_type_list[server_instance_list[server_instance_id].type_id].cpu_core / 2 - server_instance_list[server_instance_id].B_cpu_access) / (double)(server_type_list[server_instance_list[server_instance_id].type_id].cpu_core / 2);
	server_instance_list[server_instance_id].A_memory_usage = (double)(server_type_list[server_instance_list[server_instance_id].type_id].memory / 2 - server_instance_list[server_instance_id].A_memory_access) / (double)(server_type_list[server_instance_list[server_instance_id].type_id].memory / 2);
	server_instance_list[server_instance_id].B_memory_usage = (double)(server_type_list[server_instance_list[server_instance_id].type_id].memory / 2 - server_instance_list[server_instance_id].B_memory_access) / (double)(server_type_list[server_instance_list[server_instance_id].type_id].memory / 2);
}

//�鲢����  sort_cpu_memory, sort_server_type_id, server_type_num  ����ǽ�ÿ��cpuռ���ڴ��ٵķ���������ǰ��  
void mergeSort(double *arr, int *arr_index, int len) 
{     
    //����ɹ��ķ���������С�ڵ���1 ����  
    if (len <= 1) 
    { 
        return; 
    } 
    int leftlen = len / 2; 
    int rightlen = len - leftlen; 
    double *left = arr; 
    int *left_index = arr_index; 
    double *right = arr + leftlen; 
    int *right_index = arr_index + leftlen; 
    mergeSort(left, left_index, leftlen); 
    mergeSort(right, right_index, rightlen); 
    double *tmp = new double[len]; 
    int *tmp_index = new int[len]; 
    int i = 0; 
    int j = leftlen; 
    int k = 0; 
    while (i < leftlen && j < len) 
    { 
        if (arr[i] <= arr[j]) 
        { 
            tmp[k] = arr[i]; 
            tmp_index[k] = arr_index[i]; 
            i++; 
            k++; 
        } 
        else 
        { 
            tmp[k] = arr[j]; 
            tmp_index[k] = arr_index[j]; 
            j++; 
            k++; 
        } 
    } 
    while (i < leftlen) 
    { 
        tmp[k] = arr[i]; 
        tmp_index[k] = arr_index[i]; 
        i++; 
        k++; 
    } 
    while (j < len) 
    { 
        tmp[k] = arr[j]; 
        tmp_index[k] = arr_index[j]; 
        j++; 
        k++; 
    } 
    for (k = 0; k < len; k++) 
    { 
        arr[k] = tmp[k]; 
        arr_index[k] = tmp_index[k]; 
    } 
    if (tmp != NULL) 
    { 
        delete[] tmp; 
        tmp = NULL; 
    } 
    if (tmp_index != NULL) 
    { 
        delete[] tmp_index; 
        tmp_index = NULL; 
    } 
} 


#ifdef MI
void print_usage() {
	for(int i = 0; i < server_instance_num; i++) {
		if (server_instance_list[i].A_cpu_usage>0 || server_instance_list[i].B_cpu_usage>0)
  {
   DAYCOST += server_type_list[server_instance_list[i].type_id].energy_cost;
   TOTALCOST += server_type_list[server_instance_list[i].type_id].energy_cost;
  }
		refresh(i);
		//cout << server_instance_list[i].type_id << " "; 
		//cout << i << "|| ";
		//cout << "A_cpu: " << server_instance_list[i].A_cpu_usage * 100 << "%" << "  ";
		///cout << "A_mem: " << server_instance_list[i].A_memory_usage * 100 << "%" << "  ";
		//cout << "B_cpu: " << server_instance_list[i].B_cpu_usage * 100 << "%" << "  ";
		//cout << "B_mem: " << server_instance_list[i].B_memory_usage * 100 << "%" <<endl;
	}
}
#endif
 
//����������������� 
void check_limit(int num)  
{ 
	
    if (num >= max_server_num) 
    { 
        int old = max_server_num; 
        //����������������� +1000 
		max_server_num += ADD_SERVER_NUM; 
        server_instance *tmp_list = new server_instance[max_server_num]; 
        double *tmp_total_usage = new double[max_server_num];
		int *tmp_total_id = new int[max_server_num];		
        for (int i = 0; i < old; i++) 
        { 
            tmp_list[i] = server_instance_list[i]; 
            tmp_total_usage[i] = total_server_usage[i];
            tmp_total_id[i] = total_server_id[i];
        } 
        delete[] server_instance_list;
		delete[] total_server_usage;
		delete[] total_server_id;
		total_server_usage = new double[max_server_num];
		total_server_id = new int[max_server_num];
        server_instance_list = new server_instance[max_server_num]; 
        for (int i = 0; i < old; i++) 
        { 	
        	total_server_usage[i] = tmp_total_usage[i];
            total_server_id[i] = tmp_total_id[i];
            server_instance_list[i] = tmp_list[i]; 
        } 
        delete[] tmp_list;
		delete[] tmp_total_usage;
		delete[] tmp_total_id; 
    } 
} 

void delete_vm(int vm_id, request *r) 
{ 
    vm_instance v = vm_instance_map[vm_id]; 
    int server_id = v.server_id; 
    int server_node = v.node; 
    int vm_type = v.type_id; 
    vm_info v_i = vm_type_list[vm_type]; 
    int cpu = v_i.cpu_core; 
    int memory = v_i.memory; 
    if (server_node == 1) 
    { 
        server_instance_list[server_id].A_cpu_access += cpu; 
        server_instance_list[server_id].A_memory_access += memory;
        for(vector<int>::iterator it = server_vm_map[server_id].A_instance.begin();it!=server_vm_map[server_id].A_instance.end();){
        	if(*it == vm_id){
        		it = server_vm_map[server_id].A_instance.erase(it);
			}else it++;
		}
    } 
    else if (server_node == 2) 
    { 
        server_instance_list[server_id].B_cpu_access += cpu; 
        server_instance_list[server_id].B_memory_access += memory; 
        for(vector<int>::iterator it = server_vm_map[server_id].B_instance.begin();it!=server_vm_map[server_id].B_instance.end();){
        	if(*it == vm_id){
        		it = server_vm_map[server_id].B_instance.erase(it);
			}
			else it++;
		}
    } 
    else 
    { 
        int node_cpu = cpu / 2; 
        int node_memory = memory / 2; 
        server_instance_list[server_id].A_cpu_access += node_cpu; 
        server_instance_list[server_id].A_memory_access += node_memory; 
        server_instance_list[server_id].B_cpu_access += node_cpu; 
        server_instance_list[server_id].B_memory_access += node_memory; 
        for(vector<int>::iterator it = server_vm_map[server_id].double_instance.begin();it!=server_vm_map[server_id].double_instance.end();){
        	if(*it == vm_id){
        		it = server_vm_map[server_id].double_instance.erase(it);
			}else it++;
		}
    } 
    vm_instance_map.erase(vm_id); 
    vm_instance_num--;
    r->sloved = SLOVED; 
} 

void del_migration_vm(int vm_id) 
{ 	
    vm_instance v = vm_instance_map[vm_id]; 
    int server_id = v.server_id; 
    int server_node = v.node; 
    int vm_type = v.type_id; 
    vm_info v_i = vm_type_list[vm_type]; 
    int cpu = v_i.cpu_core; 
    int memory = v_i.memory; 
    //�����������ڷ�����A��
    if (server_node == 1) 
    { 
        server_instance_list[server_id].A_cpu_access += cpu; 
        server_instance_list[server_id].A_memory_access += memory;
        refresh(server_id); 
        for(vector<int>::iterator it = server_vm_map[server_id].A_instance.begin();it!=server_vm_map[server_id].A_instance.end();){
        	if(*it == vm_id){
        		it = server_vm_map[server_id].A_instance.erase(it);
			}else it++;
		}
    } 
    else if (server_node == 2) 
    { 
        server_instance_list[server_id].B_cpu_access += cpu; 
        server_instance_list[server_id].B_memory_access += memory; 
        refresh(server_id); 
        for(vector<int>::iterator it = server_vm_map[server_id].B_instance.begin();it!=server_vm_map[server_id].B_instance.end();){
        	if(*it == vm_id){
        		it = server_vm_map[server_id].B_instance.erase(it);
			}
			else it++;
		}
    } 
    else 
    { 
        int node_cpu = cpu / 2; 
        int node_memory = memory / 2; 
        server_instance_list[server_id].A_cpu_access += node_cpu; 
        server_instance_list[server_id].A_memory_access += node_memory; 
        server_instance_list[server_id].B_cpu_access += node_cpu; 
        server_instance_list[server_id].B_memory_access += node_memory; 
        refresh(server_id); 
        for(vector<int>::iterator it = server_vm_map[server_id].double_instance.begin();it!=server_vm_map[server_id].double_instance.end();){
        	if(*it == vm_id){
        		it = server_vm_map[server_id].double_instance.erase(it);
			}else it++;
		}
    } 
    vm_instance_map.erase(vm_id); 
} 
 
 
int migration_check(int vm_type_id, int vm_id, int start_sort_index)
{ 
	int server_instance_id = -1; // ���������е���� 
	int node = 0;
	double least_log_remain = 100;
	double least_remain = 100;
	vm_info v = vm_type_list[vm_type_id];
	for (int i = start_sort_index; i < server_instance_num; i++)
	{
		if (i == start_sort_index-1)
		{
			continue;
		}
		server_instance s = server_instance_list[total_server_id[i]];
		if(s.A_cpu_access + s.B_cpu_access == server_type_list[s.type_id].cpu_core && s.A_memory_access + s.B_memory_access == server_type_list[s.type_id].memory){
			continue;
		} 
		if (v.is_double_node == 0)
		{
			if (s.A_cpu_access >= v.cpu_core && s.A_memory_access >= v.memory)
			{
				double remain_cpu_mem = ( (double)(s.A_cpu_access - v.cpu_core) + (double)(s.A_memory_access - v.memory) ) / ((double)(server_type_list[s.type_id].cpu_core / 2)+(double)(server_type_list[s.type_id].memory / 2));
			//	double remain_log_cpu_mem = abs((double(s.A_cpu_access - v.cpu_core)/double(s.A_memory_access - v.memory)));			
				if (remain_cpu_mem<least_remain) {
			//	least_log_remain =remain_log_cpu_mem;
					least_remain = remain_cpu_mem;
					server_instance_id = total_server_id[i];
					node = 1;
			}
		}
		if (s.B_cpu_access >= v.cpu_core && s.B_memory_access >= v.memory)
		{
				double remain_cpu_mem = ( (double)(s.B_cpu_access - v.cpu_core) + (double)(s.B_memory_access - v.memory) ) / ((double)(server_type_list[s.type_id].cpu_core / 2)+(double)(server_type_list[s.type_id].memory / 2));
			//	double remain_log_cpu_mem = abs((double(s.B_cpu_access - v.cpu_core)/double(s.B_memory_access - v.memory)));

				if (remain_cpu_mem<least_remain) {
			//		least_log_remain =remain_log_cpu_mem;
					least_remain = remain_cpu_mem;
					server_instance_id = total_server_id[i];
					node = 2;
				}
		}
		}
		else
		{
			int node_cpu = v.cpu_core / 2;
			int node_memory = v.memory / 2;
			if (s.A_cpu_access >= node_cpu && s.A_memory_access >= node_memory && s.B_cpu_access >= node_cpu && s.B_memory_access >= node_memory)
			{
				double remain_cpu_mem = ( (double)(s.B_cpu_access - v.cpu_core/ 2) + (double)(s.B_memory_access - v.memory/ 2) + (double)(s.A_cpu_access - v.cpu_core/ 2) + (double)(s.A_memory_access - v.memory/ 2)) / ((double)(server_type_list[s.type_id].cpu_core)+(double)(server_type_list[s.type_id].memory));
			//	double remain_log_cpu_mem = abs(( (double(s.B_cpu_access - v.cpu_core) + double(s.A_cpu_access - v.cpu_core))/ (double(s.B_memory_access - v.memory) + double(s.A_memory_access - v.memory))));
				if ( remain_cpu_mem<least_remain) {
			//		least_log_remain =remain_log_cpu_mem;
					least_remain = remain_cpu_mem;
					server_instance_id = total_server_id[i];
					node = 0;
				}
			}
		}
	}
	if (server_instance_id != -1)
	{
		if (node == 1)
		{	
			if (vm_instance_map[vm_id].server_id == server_instance_id && vm_instance_map[vm_id].node == 1){
				return 0;
			}
			else{
				//ɾ��ԭ��vm_id ���ڵ���Ϣ 
				del_migration_vm(vm_id);
				vm_instance_map[vm_id].type_id = vm_type_id;
				vm_instance_map[vm_id].server_id = server_instance_id;
				vm_instance_map[vm_id].node = 1;
				server_instance_list[server_instance_id].A_cpu_access -= v.cpu_core;
				server_instance_list[server_instance_id].A_memory_access -= v.memory;
				server_vm_map[server_instance_id].A_instance.push_back(vm_id);
				// ˢ�������Ѿ����з��������ڴ�ʹ���� 
				refresh(server_instance_id);
				return 1;
			}
		}
		else if (node == 2)
		{	
			if (vm_instance_map[vm_id].server_id == server_instance_id && vm_instance_map[vm_id].node == 2 ){
				return 0;
			}
			else{
				del_migration_vm(vm_id);
				vm_instance_map[vm_id].type_id = vm_type_id;
				vm_instance_map[vm_id].server_id = server_instance_id;
				vm_instance_map[vm_id].node = 2;
				server_instance_list[server_instance_id].B_cpu_access -= v.cpu_core;
				server_instance_list[server_instance_id].B_memory_access -= v.memory;
				server_vm_map[server_instance_id].B_instance.push_back(vm_id);
				refresh(server_instance_id);
				return 1;
			}
		}
		else
		{	
			if (vm_instance_map[vm_id].server_id == server_instance_id && vm_instance_map[vm_id].node == 0 ){
				return 0;
			}
			else{
				int node_cpu = v.cpu_core / 2;
				int node_memory = v.memory / 2;
				del_migration_vm(vm_id);
				vm_instance_map[vm_id].type_id = vm_type_id;
				vm_instance_map[vm_id].server_id = server_instance_id;
				vm_instance_map[vm_id].node = 0;
				server_instance_list[server_instance_id].A_cpu_access -= node_cpu;
				server_instance_list[server_instance_id].A_memory_access -= node_memory;
				server_instance_list[server_instance_id].B_cpu_access -= node_cpu;
				server_instance_list[server_instance_id].B_memory_access -= node_memory;
				server_vm_map[server_instance_id].double_instance.push_back(vm_id);
				refresh(server_instance_id);
				return 1;
			} 
		}
	}
	return 0;
}

//�������Ӧ�������š��������Id��������Ϣ�б�  
int check_and_add(int vm_type_id, int vm_id, request *r) {
	int server_instance_id = -1;
	int node = 0;
	vm_info v = vm_type_list[vm_type_id];
	double least_remain =100;
	double least_log_remain = 100;
	for (int i = 0; i < server_instance_num; i++) {
		server_instance s = server_instance_list[i];
		if (v.is_double_node == 0)
		{
				if (s.A_cpu_access >= v.cpu_core && s.A_memory_access >= v.memory)
				{
					double remain_cpu_mem = ( (double)(s.A_cpu_access - v.cpu_core) + (double)(s.A_memory_access - v.memory) ) / ((double)(server_type_list[s.type_id].cpu_core / 2)+(double)(server_type_list[s.type_id].memory / 2));
				//	double remain_log_cpu_mem = abs((double(s.A_cpu_access - v.cpu_core)/double(s.A_memory_access - v.memory)));
					if (remain_cpu_mem<least_remain) {
				//		least_log_remain =remain_log_cpu_mem;
						least_remain = remain_cpu_mem;
						server_instance_id = i;
						node = 1;
					}
				}
			if (s.B_cpu_access >= v.cpu_core && s.B_memory_access >= v.memory)
			{
				double remain_cpu_mem = ( (double)(s.B_cpu_access - v.cpu_core) + (double)(s.B_memory_access - v.memory) ) / ((double)(server_type_list[s.type_id].cpu_core / 2)+(double)(server_type_list[s.type_id].memory / 2));
			//	double remain_log_cpu_mem = abs((double(s.B_cpu_access - v.cpu_core)/double(s.B_memory_access - v.memory)));
				if (remain_cpu_mem<least_remain) {
			//		least_log_remain =remain_log_cpu_mem;
					least_remain = remain_cpu_mem;
					server_instance_id = i;
					node = 2;
				}
			}
		}
		else
		{
			int node_cpu = v.cpu_core / 2;
			int node_memory = v.memory / 2;
			if (s.A_cpu_access >= node_cpu && s.A_memory_access >= node_memory && s.B_cpu_access >= node_cpu && s.B_memory_access >= node_memory)
			{
				double remain_cpu_mem = ( (double)(s.B_cpu_access - v.cpu_core/ 2) + (double)(s.B_memory_access - v.memory/ 2) + (double)(s.A_cpu_access - v.cpu_core/ 2) + (double)(s.A_memory_access - v.memory/ 2)) / ((double)(server_type_list[s.type_id].cpu_core)+(double)(server_type_list[s.type_id].memory));
		//		double remain_log_cpu_mem = abs(( (double(s.B_cpu_access - v.cpu_core) + double(s.A_cpu_access - v.cpu_core))/ (double(s.B_memory_access - v.memory) + double(s.A_memory_access - v.memory))));
				if (remain_cpu_mem<least_remain) {
			//		least_log_remain =remain_log_cpu_mem;
					least_remain = remain_cpu_mem;
					server_instance_id = i;
					node = 0;
				}
			}
		}
	}
	if (server_instance_id != -1) {
		if (node == 1)
		{
			vm_instance_map[vm_id].type_id = vm_type_id;
			vm_instance_map[vm_id].server_id = server_instance_id;
			vm_instance_map[vm_id].node = 1;
			server_instance_list[server_instance_id].A_cpu_access -= v.cpu_core;
			server_instance_list[server_instance_id].A_memory_access -= v.memory;
			server_vm_map[server_instance_id].A_instance.push_back(vm_id);
			r->sloved = SLOVED;
			r->server_id = server_instance_id;
			r->node = 1;
			vm_instance_num++;
			return 1;
		}
		else if (node == 2)
		{
			vm_instance_map[vm_id].type_id = vm_type_id;
			vm_instance_map[vm_id].server_id = server_instance_id;
			vm_instance_map[vm_id].node = 2;
			server_instance_list[server_instance_id].B_cpu_access -= v.cpu_core;
			server_instance_list[server_instance_id].B_memory_access -= v.memory;
			server_vm_map[server_instance_id].B_instance.push_back(vm_id);
			r->sloved = SLOVED;
			r->server_id = server_instance_id;
			r->node = 2;
			vm_instance_num++;
			return 1;
		}
		else
		{
			int node_cpu = v.cpu_core / 2;
			int node_memory = v.memory / 2;
			vm_instance_map[vm_id].type_id = vm_type_id;
			vm_instance_map[vm_id].server_id = server_instance_id;
			vm_instance_map[vm_id].node = 0;
			server_instance_list[server_instance_id].A_cpu_access -= node_cpu;
			server_instance_list[server_instance_id].A_memory_access -= node_memory;
			server_instance_list[server_instance_id].B_cpu_access -= node_cpu;
			server_instance_list[server_instance_id].B_memory_access -= node_memory;
			server_vm_map[server_instance_id].double_instance.push_back(vm_id);
			r->sloved = SLOVED;
			r->server_id = server_instance_id;
			r->node = 0;
			vm_instance_num++;
			return 1;
		}
	}
	r->sloved = UNSLOVED;
	return 0;
}

int find_fit_server(double vm_cpu_memory, int vm_cpu, int vm_memory, int day_remain)
{
	int server_type_id = 0;
	double least_fit_param = DBL_MAX;
	double least_average_cost = DBL_MAX;
	// double average_cost = (server_type_list[server_type_id].machine_cost + (day_remain * server_type_list[server_type_id].energy_cost))/(server_type_list[server_type_id].cpu_core+server_type_list[server_type_id].memory);
	for (int i=0;i<server_type_num;i++)
	{	
		// server_type_list[i].cpu_memory<BUY_CPU_MEM_HIGH &&server_type_list[i].cpu_memory>BUY_CPU_MEM_LOW
	//	double fit_param = abs(log(server_type_list[i].cpu_memory) - log(vm_cpu_memory));
		//double average_cost = (server_type_list[i].machine_cost + (day_remain * server_type_list[i].energy_cost))/(server_type_list[i].cpu_core+server_type_list[i].memory);//����CPU�ڴ�
		 double average_cost = (server_type_list[i].machine_cost + (day_remain * server_type_list[i].energy_cost));//������CPU�ڴ�
		if(average_cost<=least_average_cost && server_type_list[i].cpu_core >= vm_cpu && server_type_list[i].memory >= vm_memory &&  server_type_list[i].cpu_memory >BUY_CPU_MEM_LOW && server_type_list[i].cpu_memory < BUY_CPU_MEM_HIGH)
		{
			server_type_id = i;
			least_average_cost = average_cost;
		//	least_fit_param = fit_param;
		}
	}
	return server_type_id;
}

//��������� 
void buy_server(int type_id) 
{ 
    check_limit(server_instance_num);
	// ʵ����һ�������� 
    server_instance_list[server_instance_num].type_id = type_id;
	// ����������cpu �ڴ� ����2 ��ΪA�� B�� 
    int node_cpu = server_type_list[type_id].cpu_core / 2; 
    int node_memory = server_type_list[type_id].memory / 2; 
    server_instance_list[server_instance_num].A_cpu_access = node_cpu; 
    server_instance_list[server_instance_num].A_memory_access = node_memory; 
    server_instance_list[server_instance_num].B_cpu_access = node_cpu; 
    server_instance_list[server_instance_num].B_memory_access = node_memory; 
    server_instance_num++; 
    //�����еķ���������+1 
    server_type_list[type_id].count++; 
    SERVERCOST +=server_type_list[type_id].machine_cost;
	TOTALCOST += server_type_list[type_id].machine_cost;
} 
 
//cur_server_instance(ʵ������-1), daily_request_list[i].vm_type, daily_request_list[i].vm_id, &daily_request_list[i]
int add_vm(int server_instance_id, int vm_type_id, int vm_id, request *r) 
{  
    server_instance s = server_instance_list[server_instance_id];
	//��Ҫ��ӵ������ʵ�� 
    vm_info v = vm_type_list[vm_type_id]; 
    
    if (v.is_double_node == 0) 
    { 	
    	//���㲿�� 
        int select = 0; 
//        if (s.A_cpu_access >= v.cpu_core && s.A_memory_access >= v.memory && s.B_cpu_access >= v.cpu_core && s.B_memory_access >= v.memory) 
//        { 
//            if (s.A_cpu_access + s.A_memory_access >= s.B_cpu_access + s.B_memory_access) 
//            { 
//                select = 1; 
//            } 
//            else 
//            { 
//                select = 2; 
//            } 
//        } 
        if (s.A_cpu_access >= v.cpu_core && s.A_memory_access >= v.memory) 
        { 
            select = 1; 
        } 
        if (s.B_cpu_access >= v.cpu_core && s.B_memory_access >= v.memory) 
        { 
            select = 2; 
        } 
        if (select == 1) 
        { 
            vm_instance_map[vm_id].type_id = vm_type_id; 
            vm_instance_map[vm_id].server_id = server_instance_id; 
            vm_instance_map[vm_id].node = 1; 
            server_instance_list[server_instance_id].A_cpu_access -= v.cpu_core; 
            server_instance_list[server_instance_id].A_memory_access -= v.memory;
            server_vm_map[server_instance_id].A_instance.push_back(vm_id);
            r->sloved = SLOVED; 
            r->server_id = server_instance_id; 
            r->node = 1; 
            vm_instance_num++;
            return 1; 
        } 
        else if (select == 2) 
        { 
            vm_instance_map[vm_id].type_id = vm_type_id; 
            vm_instance_map[vm_id].server_id = server_instance_id; 
            vm_instance_map[vm_id].node = 2; 
            server_instance_list[server_instance_id].B_cpu_access -= v.cpu_core; 
            server_instance_list[server_instance_id].B_memory_access -= v.memory;
			server_vm_map[server_instance_id].B_instance.push_back(vm_id); 
            r->sloved = SLOVED; 
            r->server_id = server_instance_id; 
            r->node = 2; 
            vm_instance_num++;
            return 1; 
        } 
    } 
    else 
    { 
        int node_cpu = v.cpu_core / 2; 
        int node_memory = v.memory / 2; 
        if (s.A_cpu_access >= node_cpu && s.A_memory_access >= node_memory && s.B_cpu_access >= node_cpu && s.B_memory_access >= node_memory) 
        { 
            vm_instance_map[vm_id].type_id = vm_type_id; 
            vm_instance_map[vm_id].server_id = server_instance_id; 
            vm_instance_map[vm_id].node = 0; 
            server_instance_list[server_instance_id].A_cpu_access -= node_cpu; 
            server_instance_list[server_instance_id].A_memory_access -= node_memory; 
            server_instance_list[server_instance_id].B_cpu_access -= node_cpu; 
            server_instance_list[server_instance_id].B_memory_access -= node_memory; 
            server_vm_map[server_instance_id].double_instance.push_back(vm_id);
            r->sloved = SLOVED; 
            r->server_id = server_instance_id; 
            r->node = 0; 
            vm_instance_num++;
            return 1; 
        } 
    } 
    r->sloved = UNSLOVED; 
    return 0; 
} 

//Ǩ�ƺ��� 
int migration_vm(){
    int mi_sum = 0; //��¼Ǩ�ƴ���
    int tmp_vm_id = 0; // �������Ӧ��Id ��Id
    int vm_cpu = 0;  //  �������cpu������
    int tmp_vm_type_id = 0;  //  �������Ӧ����� ��
    int vm_memory = 0; // �������memory
    double cpu_memory = 0; // �������cpu/memory
    int flag = 0;
    int stop = 0 ; // ��¼�Ƿ񳬹�ǧ��֮��  
    int limit_times =  (vm_instance_num / double(1000) )* 5;
    //�����˾��˳� 
    if(mi_sum + 1 > limit_times){
    	return 0;
	}
	for(int i = 0;i < server_instance_num; i++){
		refresh(i);
		//����ʹ������������ŷ������ 
		total_server_id[i] = i;
		//��ʹ����Ϊ�ĸ�ָ����ӳ���4 
		total_server_usage[i] = (server_instance_list[i].A_cpu_usage+server_instance_list[i].A_memory_usage + server_instance_list[i].B_cpu_usage + server_instance_list[i].B_memory_usage) / 4.0;
	}
	mergeSort(total_server_usage,total_server_id,server_instance_num); 
	for(int i = 0;i < server_instance_num;i++){
		refresh(total_server_id[i]);
		double i_server_usage = (server_instance_list[total_server_id[i]].A_cpu_usage + server_instance_list[total_server_id[i]].A_memory_usage+server_instance_list[total_server_id[i]].B_cpu_usage + server_instance_list[total_server_id[i]].B_memory_usage) / 4 ;
		int *record_vmid = new int[100];
		int record_num = 0;
		if(stop == 1){
			break;
		}
		if(i_server_usage == 0){
			continue;
		}
		if(i_server_usage > 0.88){
			continue;
		}	
		if(i_server_usage > 0){
			for(int a = 0; a < server_vm_map[total_server_id[i]].A_instance.size(); a++){
				//����������A�ڵ��vm_id ������ʱ�ı���
				tmp_vm_id = server_vm_map[total_server_id[i]].A_instance[a];
				//��¼�������Ӧ�����
				tmp_vm_type_id = vm_instance_map[tmp_vm_id].type_id;
				// �Ƴ���������ƽ��
				server_instance this_server = server_instance_list[i];
				vm_info v = vm_type_list[tmp_vm_type_id];
				double this_remain_log_cpu_mem = abs(log(double(this_server.A_cpu_access + v.cpu_core)/double(this_server.A_memory_access + v.memory)));
				double this_ori_log_cpu_mem = abs(log(double(this_server.A_cpu_access)/double(this_server.A_memory_access)));
				if(this_ori_log_cpu_mem < this_remain_log_cpu_mem)
				{
					continue;
				}		
				int flag = migration_check(tmp_vm_type_id, tmp_vm_id, i + 1);
				if(flag == 1){
					record_vmid[record_num] = tmp_vm_id;
					record_num++;
					migration_info_map[mi_sum].mi_vm_id =  tmp_vm_id;
					migration_info_map[mi_sum].target_serverId = vm_instance_map[tmp_vm_id].server_id;
					migration_info_map[mi_sum].target_node = vm_instance_map[tmp_vm_id].node;
					mi_sum++;
					if(mi_sum + 1 > limit_times){
						stop = 1;
    					break;
					}
				}
			}
			if(stop == 1){
				break;
			}
			else{
				for(int a = 0; a < server_vm_map[total_server_id[i]].B_instance.size(); a++){
					tmp_vm_id = server_vm_map[total_server_id[i]].B_instance[a];
					tmp_vm_type_id = vm_instance_map[tmp_vm_id].type_id;
					server_instance this_server = server_instance_list[i];
					vm_info v = vm_type_list[tmp_vm_type_id];
					double this_remain_log_cpu_mem = abs(log(double(this_server.B_cpu_access + v.cpu_core)/double(this_server.B_memory_access + v.memory)));
					double this_ori_log_cpu_mem = abs(log(double(this_server.B_cpu_access)/double(this_server.B_memory_access)));
					if(this_ori_log_cpu_mem < this_remain_log_cpu_mem)
					{
						continue;
					}
					int r_flag = 1;
					for(int r = 0; r < record_num; r++) {
						if(tmp_vm_id == record_vmid[r]){
							r_flag = 0;
							break;
						}
					}
					if(r_flag == 1){
							int flag = migration_check(tmp_vm_type_id, tmp_vm_id, i + 1);
							if(flag == 1){
								migration_info_map[mi_sum].mi_vm_id =  tmp_vm_id;
								migration_info_map[mi_sum].target_serverId = vm_instance_map[tmp_vm_id].server_id;
								migration_info_map[mi_sum].target_node = vm_instance_map[tmp_vm_id].node;
								mi_sum++;
								if(mi_sum + 1 > limit_times){
									stop = 1;
			    					break;
								}
							}
					}
				}
			}
            if(stop == 1){
				break;
			}
			else{
				for(int a = 0; a < server_vm_map[total_server_id[i]].double_instance.size(); a++){
					tmp_vm_id = server_vm_map[total_server_id[i]].double_instance[a];
					tmp_vm_type_id = vm_instance_map[tmp_vm_id].type_id;
					server_instance this_server = server_instance_list[i];
					vm_info v = vm_type_list[tmp_vm_type_id];
					double this_remain_log_cpu_mem = abs(log( (double(this_server.B_cpu_access + v.cpu_core/2)+(double(this_server.A_cpu_access + v.cpu_core/2)))/(double(this_server.B_memory_access + v.memory/2)+double(this_server.A_memory_access + v.memory/2))));
					double this_ori_log_cpu_mem = abs(log((double(this_server.B_cpu_access)+(double(this_server.A_cpu_access))/(double(this_server.B_memory_access)+double(this_server.A_memory_access)))));
					if(this_ori_log_cpu_mem < this_remain_log_cpu_mem)
					{
						continue;
					}
					int flag = migration_check(tmp_vm_type_id, tmp_vm_id, i + 1);
					if(flag == 1){
						migration_info_map[mi_sum].mi_vm_id =  tmp_vm_id;
						migration_info_map[mi_sum].target_serverId = vm_instance_map[tmp_vm_id].server_id;
						migration_info_map[mi_sum].target_node = vm_instance_map[tmp_vm_id].node;
						mi_sum++;
						if(mi_sum + 1 > limit_times){
							stop = 1;
	    					break;
						}
					}
				}
			}
		}
		if (record_vmid != NULL) 
        { 
            delete[] record_vmid; 
            record_vmid = NULL; 
        } 
	}
	return mi_sum;	
}

////�ֶδ���request
void deal_cut_requests(int begin,int end,int *sort_request_id,double *sort_vm_cpu_memory,request *daily_request_list,int &buying_count,int &tmp_server_instance_num,int &newserver_vm_num,int &newserver_req_num,map<int, int> &newserver_req,int runday,vector<int> &buying_list,map<int, int> &buying_num,map<int, int> &newserver_vm_list)
{
	// ��ʱ�ķֶ������ 
	int *tmp_sort_request_id;
	double *tmp_sort_vm_cpu_memory;
	tmp_sort_request_id = new int[end - begin]; 
	tmp_sort_vm_cpu_memory = new double[end - begin];  
	for(int i = 0; i < end - begin ; i++){
		tmp_sort_request_id[i] = sort_request_id[i+begin];
		tmp_sort_vm_cpu_memory[i] = sort_vm_cpu_memory[i+begin];
#ifdef DEBUG_POINT
		cout << "δ����ǰid  " << tmp_sort_request_id[i] << "   : "  << tmp_sort_vm_cpu_memory[i]  << endl;
#endif
	}
	// ����ʱ���������  
	mergeSort(tmp_sort_vm_cpu_memory,tmp_sort_request_id,end - begin); 
#ifdef DEBUG_POINT	
	for(int i = 0; i < end - begin ;i++){
		cout <<  "����ǰid  "  << i ; 
		cout << "�����id  " << tmp_sort_request_id[i] << "   : "  << tmp_sort_vm_cpu_memory[i]  << endl;
	}
#endif
	// ������ķֶ����� 
	//for(int i =0; i < end - begin ;i++)
	for(int i =end - begin -1 ; i >= 0 ;i--)
	{
		if(check_and_add(daily_request_list[tmp_sort_request_id[i]].vm_type,daily_request_list[tmp_sort_request_id[i]].vm_id,&daily_request_list[tmp_sort_request_id[i]]) == 0){
			// ���������
			int find_vm_cpu =  vm_type_list[daily_request_list[tmp_sort_request_id[i]].vm_type].cpu_core*2;
			int find_vm_memory = vm_type_list[daily_request_list[tmp_sort_request_id[i]].vm_type].memory*2;
//			int find_vm_cpu =  vm_type_list[daily_request_list[tmp_sort_request_id[i]].vm_type].cpu_core;
//			int find_vm_memory = vm_type_list[daily_request_list[tmp_sort_request_id[i]].vm_type].memory;
//			if(vm_type_list[daily_request_list[tmp_sort_request_id[i]].vm_type].is_double_node == 0){
//				find_vm_cpu *= 2;
//				find_vm_memory *= 2;
//			}
			int server_type_id = find_fit_server(vm_type_list[daily_request_list[tmp_sort_request_id[i]].vm_type].cpu_memory,
					find_vm_cpu, find_vm_memory,total_day-runday);
			buy_server(server_type_id);
			buying_list.push_back(server_type_id);
			buying_count++;
			// // ���û�����������������Ϣ
			if(buying_num.count(server_type_id) == 0){
				buying_num[server_type_id] = 0;
			}
			buying_num[server_type_id] += 1;
			int add_result = add_vm(server_instance_num-1,daily_request_list[tmp_sort_request_id[i]].vm_type,daily_request_list[tmp_sort_request_id[i]].vm_id,&daily_request_list[tmp_sort_request_id[i]]);
		}
		// �洢�����·������������������
		if(vm_instance_map[daily_request_list[tmp_sort_request_id[i]].vm_id].server_id>=tmp_server_instance_num){
			newserver_vm_list[newserver_vm_num++] = daily_request_list[tmp_sort_request_id[i]].vm_id;
			newserver_req[newserver_req_num++] = tmp_sort_request_id[i];
		}
	}
	if (tmp_sort_request_id != NULL) 
	{ 
		delete[] tmp_sort_request_id; 
		tmp_sort_request_id = NULL; 
	} 
	if (tmp_sort_vm_cpu_memory != NULL) 
	{ 
		delete[] tmp_sort_vm_cpu_memory; 
		tmp_sort_vm_cpu_memory = NULL; 
	}
} 
 
void daily_requests(int daily_request_num,int runday)
{
#ifdef DEBUG_POINT
	cout << "daily_request_num: " << daily_request_num << endl;
#endif
	if (daily_request_num == 0)
	{ // debug03: �������Ϊ0����Ȼ�����ѭ�������������
		cout << "(purchase,0)" << endl;
		cout << "(migration, 0)" << endl;
		return;
	}
    int migration_sum = migration_vm();
	request *daily_request_list;
	int *sort_request_id;
	double *sort_vm_cpu_memory; 
	map<int, int> buying_num;
	daily_request_list = new request[daily_request_num];
	sort_request_id = new int[daily_request_num]; 
    sort_vm_cpu_memory = new double[daily_request_num];  
	vector<int> buying_list;
	char tmp_char;
	int tmp_vm_id = 0;
	int tmp_type_id = 0;
	string tmp_request;
	string tmp_name;
	int buying_count = 0;
	// δ���������ǰ����������
	int tmp_server_instance_num = server_instance_num;
	// �����·������������
	map<int, int> newserver_vm_list;
	// �����·����������������
	int newserver_vm_num = 0;
	// �����·�����������
	map<int, int> newserver_req;
	// �����·��������������� 
	int newserver_req_num = 0;
	// ��¼ÿ��del������ 
	int record_del = 0;
	//  ��¼ÿ��ڼ���del��λ��
	map<int, int> record_del_position; 
	
	// ����ÿ������
	for (int i=0;i<daily_request_num;i++)
	{	
		// ��ȡ��Ϣ
	 	cin >> tmp_char; 
	 	cin >> tmp_request;
	 	tmp_request.pop_back();
	 	if (tmp_request == "add")
	 	{	
	 		// �ֶ������id  
	 		sort_request_id[i] =i;
			// request Ϊ1 ����add  Ϊ2 ����del	
	 		daily_request_list[i].request = 1;
	 		cin >> tmp_name;
	 		tmp_name.pop_back();
	 		tmp_type_id = vm_type_index[tmp_name];
	 		daily_request_list[i].vm_type = tmp_type_id;
	 		sort_vm_cpu_memory[i] = (vm_type_list[tmp_type_id].cpu_core*1 + vm_type_list[tmp_type_id].memory * 0.6);
	 	}
	 	else
	 	{
	 		daily_request_list[i].request = 2;
	 		// ��¼del��λ����Ϣ 
			record_del_position[record_del++] = i ;			
		}
	 	cin >> tmp_vm_id;
	 	cin >> tmp_char;
	 	daily_request_list[i].vm_id = tmp_vm_id;
	 }
#ifdef DEBUG_POINT
	cout << server_instance_num << ":" <<tmp_server_instance_num <<endl;
#endif
	 // request�ֶ�����
	 // �ֶε���ʼ ��ʼΪ0  ����ÿ�μ�  record_del_position[a] + 1  
	 int seg_begin = 0;
	 // ����del
	 for(int a = 0;a < record_del; a++)
	 {	
	 	if(seg_begin != record_del_position[a]){ 
			deal_cut_requests(seg_begin,record_del_position[a],
				sort_request_id,sort_vm_cpu_memory,daily_request_list,buying_count,tmp_server_instance_num,
				newserver_vm_num,newserver_req_num,newserver_req,runday,buying_list,buying_num,newserver_vm_list);
		} 
	 	delete_vm(daily_request_list[record_del_position[a]].vm_id, &daily_request_list[record_del_position[a]]);
	 	seg_begin = record_del_position[a] + 1;
	 }
	 if (seg_begin < daily_request_num)
	 {
	 	deal_cut_requests(seg_begin,daily_request_num,
			sort_request_id,sort_vm_cpu_memory,daily_request_list,buying_count,tmp_server_instance_num,
			newserver_vm_num,newserver_req_num,newserver_req,runday,buying_list,buying_num,newserver_vm_list);
	 }
	// �Թ����������������
	int y = 0;
	// �����������Ӧ
	map<int, int> sort_server_fit;
	map<int, int>::iterator iter; 
	for (iter = buying_num.begin(); iter != buying_num.end(); iter++) 
    { 
		int i=0;
        for (int tmp = 0;i<iter->second;i++)
		{
			while (iter->first !=buying_list[tmp]){
				tmp+=1;
			}
		    sort_server_fit[y+i] = tmp;
			tmp += 1;
		} 
		y += i;
    } 
#ifdef DEBUG_POINT
	// ��������˳�� �� ������������˳�� 
	for (int i=0;i<buying_count;i++){
		cout << i << ":" << sort_server_fit[i] << endl;
	}
#endif
#ifdef DEBUG_POINT
	 cout << "����ǰ������˳��" <<endl;
	 for (int i = 0; i < server_instance_num; i++){
	 	cout << i << ":" << server_type_list[server_instance_list[i].type_id].name << endl;
	 }
#endif
	// �滻server_instance_list��server_vm_map
	server_instance *temp_server_instance_list = new server_instance[server_instance_num-tmp_server_instance_num];
	for (int i = 0; i < server_instance_num-tmp_server_instance_num; i++)
	{
		temp_server_instance_list[i] = server_instance_list[i+tmp_server_instance_num];
	}
	map<int, server_vm> temp_server_vm_map = server_vm_map;

	for (int i = 0; i < server_instance_num-tmp_server_instance_num; i++){
		server_instance_list[tmp_server_instance_num+i] = temp_server_instance_list[sort_server_fit[i]];
		server_vm_map[tmp_server_instance_num+i] = temp_server_vm_map[tmp_server_instance_num+sort_server_fit[i]];
	}
#ifdef DEBUG_POINT
	 cout << "�����������˳��" <<endl;
	 for (int i = 0; i < server_instance_num; i++){
	 	cout << i << ":" << server_type_list[server_instance_list[i].type_id].name << endl;
	 }
#endif
	delete[] temp_server_instance_list;
	temp_server_vm_map.clear();

	// ���������Ӧ
	map<int, int> sort_req_fit;
	y=0; 
	for (iter = buying_num.begin(); iter != buying_num.end(); iter++) 
    { 
		int i=0;
        for (int tmp = 0;i<iter->second;i++){
			while (iter->first !=buying_list[tmp]){
				tmp+=1;
			}
			sort_req_fit[tmp] = y+i;
			tmp += 1;
		} 
		y += i;
    } 
	// �滻vm_instance_map.server_id
	for (int i=0;i<newserver_vm_num;i++){
		vm_instance_map[newserver_vm_list[i]].server_id = sort_req_fit[vm_instance_map[newserver_vm_list[i]].server_id-tmp_server_instance_num]+tmp_server_instance_num;
	}

	// �滻r->server_id
	for (int i=0;i<newserver_req_num;i++){
		daily_request_list[newserver_req[i]].server_id = sort_req_fit[daily_request_list[newserver_req[i]].server_id-tmp_server_instance_num]+tmp_server_instance_num;
	}
	// ��ӡ�����Ϣ
	cout << "(purchase, " << buying_num.size() << ")" << endl; 
    for (iter = buying_num.begin(); iter != buying_num.end(); iter++) 
    { 
       cout << "(" << server_type_list[iter->first].name << ", " << iter->second << ")" << endl; 
    } 
    if(runday == 0){
    	cout << "(migration, 0)" << endl; 
	}
	else{
		
		mi_num += migration_sum ;
		cout << "(migration," << migration_sum << ")" << endl;
		for(int j = 0; j < migration_sum; j++){
			if(migration_info_map[j].target_node == 1){
				cout << "(" << migration_info_map[j].mi_vm_id << "," << migration_info_map[j].target_serverId << ",A)" << endl;
			}
			else if(migration_info_map[j].target_node == 2){
				cout << "(" << migration_info_map[j].mi_vm_id << "," << migration_info_map[j].target_serverId << ",B)" << endl;
			}
			else{
				cout << "(" << migration_info_map[j].mi_vm_id << "," << migration_info_map[j].target_serverId << ")" << endl;
			}
		} 
	}
	for (int i = 0; i < daily_request_num; i++)
	{
		if (daily_request_list[i].request == 1)
		{
			if(daily_request_list[i].sloved == UNSLOVED) {
				cout << "UNSOLVED\n";
			}
			if (daily_request_list[i].node == 1)
			{
				cout << "(" << daily_request_list[i].server_id << ", A)" << endl;
				if(vm_type_list[daily_request_list[i].vm_type].is_double_node == 1) {
					cout << "error\n";
				}
			}
			else if (daily_request_list[i].node == 2)
			{
				cout << "(" << daily_request_list[i].server_id << ", B)" << endl;
				if(vm_type_list[daily_request_list[i].vm_type].is_double_node == 1) {
					cout << "error\n";
				}
			}
			else
			{
				cout << "(" << daily_request_list[i].server_id << ")" << endl;
				if(vm_type_list[daily_request_list[i].vm_type].is_double_node == 0) {
					cout << "error\n";
				}
			}
		}
	}
	// �������
	if (daily_request_list != NULL)
	{
		delete[] daily_request_list;
		daily_request_list = NULL;
	}
	if (sort_request_id != NULL)
	{
		delete[] sort_request_id;
		sort_request_id = NULL;
	}
	if (sort_vm_cpu_memory != NULL) 
    { 
        delete[] sort_vm_cpu_memory; 
        sort_vm_cpu_memory = NULL; 
    } 
	migration_info_map.clear();
	newserver_vm_list.clear();
	buying_num.clear();
	buying_list.clear();
	record_del_position.clear();
} 



void all_requests() 
{ 
    //�������������  
    cin >> total_day; 
    for (int i = 0; i < total_day; i++) 
    { 
        int daily_request_num = 0; 
        //��������������  
        cin >> daily_request_num; 
#ifdef MI 
        cout << "--day" << i << endl; 
#endif     
        // �����������  
        daily_requests(daily_request_num,i);
        for(int i = 0; i < server_instance_num; i++) {
		refresh(i);
	    }
#ifdef MI
		print_usage();
#endif 
		//cout << "--endday" << i << endl;
    } 
} 

 
void delete_cluster() 
{ 
    if (server_type_list != NULL) 
    { 
        delete[] server_type_list; 
        server_type_list = NULL; 
    } 
    if (server_instance_list != NULL) 
    { 
        delete[] server_instance_list; 
        server_instance_list = NULL; 
    } 
    if (vm_type_list != NULL) 
    { 
        delete[] vm_type_list; 
        vm_type_list = NULL; 
    } 
    if (sort_cpu_memory != NULL) 
    { 
        delete[] sort_cpu_memory; 
        sort_cpu_memory = NULL; 
    } 
    if (sort_server_type_id != NULL) 
    { 
        delete[] sort_server_type_id; 
        sort_server_type_id = NULL; 
    }
    if (total_server_usage != NULL) {
		delete[] total_server_usage;
		total_server_usage = NULL;
	}
	if (total_server_id != NULL) {
		delete[] total_server_id;
		total_server_id = NULL;
	}
} 


int main() 
{ 
#ifdef FILEINPUT 
    freopen("training-2.txt", "r", stdin); 
#endif 
    init_c(); 
    init_server_type_list(); 
    init_vm_type_list(); 
    mergeSort(sort_cpu_memory, sort_server_type_id, server_type_num);
#ifdef DEBUG_POINT 
    //����������������  
    cout << "server_type_num: " << server_type_num << endl; 
    // ���õķ���������  
    cout << "server_instance_num: " << server_instance_num << endl; 
    //���������������  
    cout << "vm_type_num: " << vm_type_num << endl; 
    // ���õ����������  
    cout << "vm_instance_num: " << vm_instance_num << endl; 
#endif 
    all_requests(); 
#ifdef MI
	cout << "TOTALCOST:" << TOTALCOST << ", SERVERCOST:" << SERVERCOST << ", DAYCOST:" << DAYCOST << endl;
	cout << "Ǩ�ƴ�����" << mi_num << endl;
#endif
    delete_cluster(); 
    fflush(stdout); 
    return 0; 
} 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{ 
    int index;
    int no_sections;       
    char name[30];         
    float wear[10];        
    float future_wear[10]; 
    int cost;              
    struct node *next;
    struct node *prev;
} Node;

typedef struct graph
{ 
    int nr_nodes;
    Node **neighbours;
} Graph;

Graph *init_graph(int nodes)
{                                              
    Graph *g = (Graph *)malloc(sizeof(Graph)); 
    if (g == NULL)
        return NULL;
    g->nr_nodes = nodes;
    g->neighbours = (Node **)malloc(nodes * sizeof(Node *)); 
    if (g->neighbours == NULL)
        return NULL;
    int i = 0;
    for (i = 0; i < nodes; i++)
        g->neighbours[i] = NULL;
    return g;
}

Node *add_node(Node *head, int index, char *name, int n, float *wear)
{                                                  
    Node *new_node = (Node *)malloc(sizeof(Node)); 
    if (new_node == NULL)
        return NULL;
    new_node->index = index;
    new_node->no_sections = n;
    strcpy(new_node->name, name);
    int i = 0;
    for (i = 0; i < n; i++)
    {
        new_node->wear[i] = wear[i];
        new_node->future_wear[i] = wear[i];
    }

    new_node->next = head;
    new_node->prev = NULL;

    if (head != NULL)
    { 
        head->prev = new_node;
    }

    return new_node;
}

void reverse_array(float *wear, int n, float *result)
{ 
    int i = 0;
    for (i = 0; i < n; i++)
    {
        result[i] = wear[n - 1 - i];
    }
}

void insert_edge(Graph *g, int source_index, int dest_index, char *name_source, char *name_dest, int n, float *wear)
{ 
    float reverse_wear[20];
    reverse_array(wear, n, reverse_wear);
    g->neighbours[source_index] = add_node(g->neighbours[source_index], dest_index, name_dest, n, wear);         
    g->neighbours[dest_index] = add_node(g->neighbours[dest_index], source_index, name_source, n, reverse_wear); 
}

Node *add_node_2(Node *head, int index, char *name, int distance) 
{
    Node *new_node = (Node *)malloc(sizeof(Node)); 
    if (new_node == NULL)
        return NULL;
    new_node->index = index;
    new_node->cost = distance;
    strcpy(new_node->name, name);

    new_node->next = head;
    new_node->prev = NULL;

    if (head != NULL)
    { 
        head->prev = new_node;
    }

    return new_node;
}

void insert_edge_2(Graph *g, int source_index, int dest_index, char *name_source, char *name_dest, int distance) 
{
    g->neighbours[source_index] = add_node_2(g->neighbours[source_index], dest_index, name_dest, distance);
    g->neighbours[dest_index] = add_node_2(g->neighbours[dest_index], source_index, name_source, distance);
}

void print_pairs(Graph *g, char cities[20][30], int num_pairs, int *index_1, int *index_2, FILE *f, int wear_grade)
{ 
    int i = 0, j = 0, sum = 0, prod = 0;
    for (i = 0; i < num_pairs; i++)
    {
        sum = 0;
        prod = 0;
        fprintf(f, "%s ", cities[index_1[i]]);                              
        fprintf(f, "%s ", cities[index_2[i]]);                              
        Node *temp = g->neighbours[index_1[i]];                             
        while (temp != NULL && strcmp(temp->name, cities[index_2[i]]) != 0) 
            temp = temp->next;
        fprintf(f, "%d ", temp->no_sections);
        for (j = 0; j < temp->no_sections; j++)
            fprintf(f, "%.2f ", temp->future_wear[j]);
        fprintf(f, "\n");
    }
    for (i = 0; i < num_pairs; i++)
    { 
        sum = 0;
        prod = 0;
        Node *temp = g->neighbours[index_1[i]]; 
        while (temp != NULL && strcmp(temp->name, cities[index_2[i]]) != 0)
            temp = temp->next;
        for (j = 0; j < temp->no_sections; j++)
        {
            sum = sum + temp->future_wear[j]; 
        }
        prod = sum / temp->no_sections; 
        if (prod < wear_grade)          
            fprintf(f, "%d ", i + 1);
    }
}

float max(float a, float b)
{ 
    return (a > b) ? a : b;
}

float max_det(Node *node, int index, char cities[20][30])
{ 
    float maxi = 0;
    while (node != NULL)
    {
        if (strcmp(node->name, cities[index]) == 0) 
            node = node->next;
        else
        {
            maxi = max(maxi, node->wear[0]); 
            node = node->next;
        }
    }
    return maxi;
}

void degradation_calc(Graph *g, int index_1, int index_2, char cities[20][30])
{ 
    int k = 0;
    float leftNeighborWear = 0, rightNeighborWear = 0, maxNeighborWear = 0; 

    int num_1 = 0, num_2 = 0;
    Node *temp_1 = g->neighbours[index_1]; 
    Node *temp_2 = g->neighbours[index_2]; 
    while (temp_1 != NULL)
    {
        temp_1 = temp_1->next;
        num_1++; 
    }
    while (temp_2 != NULL)
    {
        temp_2 = temp_2->next;
        num_2++; 
    }

    Node *node_1 = g->neighbours[index_1];
    Node *node_2 = g->neighbours[index_2];

    if (num_1 != 1)
    {
        while (node_1 != NULL && strcmp(node_1->name, cities[index_2]) != 0)
            node_1 = node_1->next; 
    }
    if (num_2 != 1)
    {
        while (node_2 != NULL && strcmp(node_2->name, cities[index_1]) != 0)
            node_2 = node_2->next; 
    }

    for (k = 0; k < node_1->no_sections; k++)
    {
        leftNeighborWear = 0;
        rightNeighborWear = 0;
        maxNeighborWear = 0;
        if (node_1->wear[k] != 0)
        { 
            if (node_1->wear[k] * 2 > 100.00)
                node_1->future_wear[k] = 100.00;
            else
                node_1->future_wear[k] = node_1->wear[k] * 2;
        }
        else
        {
            if (node_1->wear[k] == 0)
            { 
                if (k != 0 && k != (node_1->no_sections - 1))
                { 
                    leftNeighborWear = node_1->wear[k - 1];
                    rightNeighborWear = node_1->wear[k + 1];
                    maxNeighborWear = max(leftNeighborWear, rightNeighborWear);
                    node_1->future_wear[k] = maxNeighborWear / 2;
                }
                if (k == 0 && node_1->no_sections == 1)
                { 
                    if (num_1 != 1)
                    { 
                        Node *leftNeighbor = g->neighbours[index_1];
                        if (leftNeighbor != NULL)
                            leftNeighborWear = max_det(leftNeighbor, index_2, cities);
                        rightNeighborWear = node_1->wear[k + 1];
                        maxNeighborWear = max(leftNeighborWear, rightNeighborWear);
                    }
                    else
                    { 
                        Node *leftNeighbor = g->neighbours[index_1];
                        if (leftNeighbor != NULL)
                            leftNeighborWear = leftNeighbor->wear[0];
                        rightNeighborWear = node_1->wear[k + 1];
                        maxNeighborWear = max(leftNeighborWear, rightNeighborWear);
                    }
                    node_1->future_wear[k] = maxNeighborWear / 2; 
                }
                if (k == 0)
                { 
                    if (num_1 != 1)
                    { 
                        Node *leftNeighbor = g->neighbours[index_1];
                        if (leftNeighbor != NULL)
                            leftNeighborWear = max_det(leftNeighbor, index_2, cities);
                        rightNeighborWear = node_1->wear[k + 1];
                        maxNeighborWear = max(leftNeighborWear, rightNeighborWear);
                    }
                    else
                    { 
                        Node *leftNeighbor = g->neighbours[index_1];
                        if (leftNeighbor != NULL)
                            leftNeighborWear = leftNeighbor->wear[0];
                        rightNeighborWear = node_1->wear[k + 1];
                        maxNeighborWear = max(leftNeighborWear, rightNeighborWear);
                    }
                    node_1->future_wear[k] = maxNeighborWear / 2; 
                }
                if (k == (node_1->no_sections - 1))
                { 
                    if (num_2 != 1)
                    { 
                        Node *rightNeighbor = g->neighbours[index_2];
                        if (rightNeighbor != NULL)
                            rightNeighborWear = max_det(rightNeighbor, index_1, cities);
                        leftNeighborWear = node_1->wear[k - 1];
                        maxNeighborWear = max(leftNeighborWear, rightNeighborWear);
                    }
                    else
                    { 
                        Node *rightNeighbor = g->neighbours[index_2];
                        if (rightNeighbor != NULL)
                            rightNeighborWear = rightNeighbor->wear[0];
                        leftNeighborWear = node_1->wear[k - 1];
                        maxNeighborWear = max(leftNeighborWear, rightNeighborWear);
                        node_1->future_wear[k] = maxNeighborWear / 2;
                    }
                    node_1->future_wear[k] = maxNeighborWear / 2; 
                }
            }
        }
    }
    
    float result[20];
    reverse_array(node_1->future_wear, node_1->no_sections, result);
    int i = 0;
    for (i = 0; i < node_2->no_sections; i++)
        node_2->future_wear[i] = result[i]; 
}

int find_or_add_city(char cities[][30], int *num_cities, char *city)
{ 
    int i = 0;
    for (i = 0; i < *num_cities; i++)
    {
        if (strcmp(cities[i], city) == 0)
        { 
            return i;
        }
    }
    strcpy(cities[*num_cities], city);
    (*num_cities)++;
    return *num_cities - 1; 
}

int print_routes_to_keep(Graph *g, int *dist, int index_1, int index_2, char cities[20][30], int max_routes)
{ 
    Node *node_1 = g->neighbours[index_1];
    Node *node_2 = g->neighbours[index_2];

    while (node_1 != NULL && strcmp(node_1->name, cities[index_2]) != 0) 
        node_1 = node_1->next;
    while (node_2 != NULL && strcmp(node_2->name, cities[index_1]) != 0) 
        node_2 = node_2->next;

    int diff = 0, max_routes_count = 0;
    if (dist[index_1] > dist[index_2]) 
        diff = dist[index_1] - dist[index_2];
    else
        diff = dist[index_2] - dist[index_1];

    if (diff == node_1->cost)
    { 
        if (max_routes_count < max_routes)
            max_routes_count++;
    }
    return max_routes_count;
}

int routes_to_keep(Graph *g, int *dist, int index_1, int index_2, char cities[20][30], FILE *f)
{ 
    Node *node_1 = g->neighbours[index_1];
    Node *node_2 = g->neighbours[index_2];

    while (node_1 != NULL && strcmp(node_1->name, cities[index_2]) != 0) 
        node_1 = node_1->next;
    while (node_2 != NULL && strcmp(node_2->name, cities[index_1]) != 0) 
        node_2 = node_2->next;

    int diff = 0, max_routes_count = 0;
    if (dist[index_1] > dist[index_2]) 
        diff = dist[index_1] - dist[index_2];
    else
        diff = dist[index_2] - dist[index_1];

    if (diff == node_1->cost)
    {
        max_routes_count++;
        fprintf(f, "%s %s\n", node_2->name, node_1->name);
        return max_routes_count;
    }
    return max_routes_count;
}

void dijkstra(Graph *g, int start_node, int *index_1, int *index_2, char cities[20][30], int max_routes, int num_pairs, FILE *f)
{ 
    int distances[100], i = 0;
    for (i = 0; i < g->nr_nodes; i++)
        distances[i] = 99999;  
    distances[start_node] = 0; 

    int visited[100] = {0}; 
    visited[start_node] = 1;

    Node *current_node = g->neighbours[start_node]; 
    while (current_node != NULL)
    {
        distances[current_node->index] = current_node->cost;
        current_node = current_node->next;
    }

    while (1)
    { 
        int min_distance = 99999;
        int min_index = -1, i = 0;
        for (i = 0; i < g->nr_nodes; i++)
        {
            if (visited[i] == 0 && distances[i] < min_distance)
            {
                min_distance = distances[i];
                min_index = i;
            }
        }
        if (min_index == -1) 
            break;

        visited[min_index] = 1;

        current_node = g->neighbours[min_index];
        while (current_node != NULL)
        { 
            if (distances[min_index] + current_node->cost < distances[current_node->index])
                distances[current_node->index] = distances[min_index] + current_node->cost;
            current_node = current_node->next;
        }
    }
    int count = 0;
    for (i = 0; i < num_pairs; i++)
    { 
        if (count < max_routes)
            count = count + print_routes_to_keep(g, distances, index_1[i], index_2[i], cities, max_routes);
    }
    fprintf(f, "%d\n", count); 
    int count_1 = 0;
    for (i = 0; i < num_pairs; i++)
    { 
        if (count_1 < max_routes)
        {
            count_1 = count_1 + routes_to_keep(g, distances, index_1[i], index_2[i], cities, f);
        }
    }
}

void free_graph(Graph *g)
{ 
    if (g == NULL)
    {
        return;
    }
    int i = 0;
    for (i = 0; i < g->nr_nodes; i++)
    {
        Node *current = g->neighbours[i];
        while (current != NULL)
        {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(g->neighbours);
    free(g);
}

int main(int argc, char *argv[])
{
    Graph *g = init_graph(100);    
    if (strcmp(argv[1], "1") == 0) 
    {                              
        FILE *file1 = fopen("tema3.in", "r");
        if (file1 == NULL)
            return 0;
        FILE *file2 = fopen("tema3.out", "w");
        if (file2 == NULL)
            return 0;

        int no_routes = 0, no_years = 0, max_wear_grade = 0, i = 0;
        fscanf(file1, "%d\n", &no_routes);      
        fscanf(file1, "%d\n", &no_years);       
        fscanf(file1, "%d\n", &max_wear_grade); 

        int no_sections = 0;
        char city_1[30];
        char city_2[30];
        float wear_val[20];

        char cities[20][30];
        int num_cities = 0, j = 0, index_1[20], index_2[20], num_pairs = 0;

        for (i = 0; i < no_routes; i++)
        {

            fscanf(file1, "%s ", city_1); 
            fscanf(file1, "%s ", city_2); 
            num_pairs++;                  

            if (num_cities == 0)
            {
                strcpy(cities[num_cities], city_1);
                num_cities++;
                strcpy(cities[num_cities], city_2);
                num_cities++;
            }

            index_1[num_pairs - 1] = find_or_add_city(cities, &num_cities, city_1); 
            index_2[num_pairs - 1] = find_or_add_city(cities, &num_cities, city_2);

            fscanf(file1, "%d ", &no_sections); 
            for (j = 0; j < no_sections; j++)
                fscanf(file1, "%f ", &wear_val[j]); 

            insert_edge(g, index_1[num_pairs - 1], index_2[num_pairs - 1], city_1, city_2, no_sections, wear_val); 
        }
        for (i = 0; i < no_years; i++)
        {
            for (j = 0; j < num_pairs; j++)
                degradation_calc(g, index_1[j], index_2[j], cities); 
            for (j = 0; j < num_pairs; j++)
            {
                Node *temp_1 = g->neighbours[index_1[j]]; 
                Node *temp_2 = g->neighbours[index_2[j]]; 
                while (temp_1 != NULL && strcmp(temp_1->name, cities[index_2[j]]) != 0)
                    temp_1 = temp_1->next;                                                      
                memcpy(temp_1->wear, temp_1->future_wear, sizeof(float) * temp_1->no_sections); 
                while (temp_2 != NULL && strcmp(temp_2->name, cities[index_1[j]]) != 0)
                    temp_2 = temp_2->next;                                                      
                memcpy(temp_2->wear, temp_2->future_wear, sizeof(float) * temp_2->no_sections); 
            }
        }

        print_pairs(g, cities, num_pairs, index_1, index_2, file2, max_wear_grade); 

        
        fclose(file1);
        fclose(file2);

        free_graph(g); 
    }
    else
    {
        if (strcmp(argv[1], "2") == 0) 
        {                              
            FILE *file1 = fopen("tema3.in", "r");
            if (file1 == NULL)
                return 0;
            FILE *file2 = fopen("tema3.out", "w");
            if (file2 == NULL)
                return 0;

            char cities[20][30], start_city[20];
            int max_routes = 0, no_routes = 0, num_cities = 0;
            fscanf(file1, "%s\n", start_city); 
            strcpy(cities[num_cities], start_city);
            num_cities++;                       
            fscanf(file1, "%d\n", &max_routes); 
            fscanf(file1, "%d\n", &no_routes);  

            char city_1[20], city_2[20];
            int i = 0, distance = 0, index_1[20], index_2[20], num_pairs = 0;
            for (i = 0; i < no_routes; i++)
            {
                fscanf(file1, "%s ", city_1); 
                fscanf(file1, "%s ", city_2); 
                num_pairs++;

                index_1[num_pairs - 1] = find_or_add_city(cities, &num_cities, city_1); 
                index_2[num_pairs - 1] = find_or_add_city(cities, &num_cities, city_2);

                fscanf(file1, "%d", &distance); 

                insert_edge_2(g, index_1[num_pairs - 1], index_2[num_pairs - 1], city_1, city_2, distance); 
            }

            dijkstra(g, find_or_add_city(cities, &num_cities, start_city), index_1, index_2, cities, max_routes, num_pairs, file2); 

            
            fclose(file1);
            fclose(file2);

            free_graph(g); 
        }
    }
    return 0;
}
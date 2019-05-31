#include <iostream>
#include "src/ragglomerative.h"

int main(int argc, char const *argv[]){
    
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input file> <num clusters> "
            "<linkage type>\n", argv[0]);
        exit(1);
    } else {
        item_t *items = NULL;
        int num_items = process_input(&items, argv[1]);
        set_linkage(argv[3][0]);
        if (num_items) {
            cluster_t *cluster = agglomerate(num_items, items);
            free(items);

            if (cluster) {
                /*fprintf(stdout, "CLUSTER HIERARCHY\n"
                        "--------------------\n");
                print_cluster(cluster);*/

                int k = atoi(argv[2]);
                fprintf(stdout, "%d CLUSTERS\n"
                        "--------------------\n", k);
                get_k_clusters(cluster, k);
                free_cluster(cluster);
            }
        }
    }
    return 0;
    return 0;
}

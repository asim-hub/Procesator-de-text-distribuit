#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <unistd.h>
#include <pthread.h>
#define NUM_THREADS 4
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

struct Result {
   char *str_res;
   int ok;
} a[600000]; 

pthread_mutex_t mutex;
pthread_mutex_t mutex_var;
pthread_barrier_t barrier;

int size;
char* name;
int ok1;
int ok2;
int ok3;
int ok4;
char* buffer;
int pos1[10000] = {0,-1};
int pos2[10000] = {0,-1};
int pos3[10000] = {0,-1};
int pos4[10000] = {0,-1};
int p1 = 1;
int p2 = 1;
int p3 = 1;
int p4 = 1;
int *pos_in_text;
MPI_Status status;
int counter;
char* bff_Res_horror;
char* bff_Res_comedy;
char* bff_Res_fantasy;
char* bff_Res_Science_fiction;
int idx1 = 0;
int idx2 = 0;
int idx3 = 0;
int idx4 = 0;
char *alph2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
char *alph1 = "BCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";
char *alph_low = "abcdefghijklmnopqrstuvwxyz";
int pt1 = 1;
int pt2 = 1;
int pt3 = 1;
int pt4 = 1;
int stop = 0;

int v[600000] = {0};

void *f(void *arg)
{
    long id = *(long*) arg;

    // horror
    if(id == 0) {

        //deschid fisierul
        FILE *file = fopen(name, "r");
        if (file == NULL) {
		    printf("Eroare la deschiderea fisierului de intrare!\n");
		    exit(1);
	    }

        char *sir; 
        sir = malloc (2048 * 2048 * 500 * sizeof (char));  
        const size_t line_size = 2048;
        char* line = malloc(line_size);
        int contor = 0;
        ok1 = 0;
        char *type_str = malloc(20*sizeof(char));
        
        strcpy(type_str, "horror\n");

        //calculez cate Send fac
        while (fgets(line, line_size, file) != NULL){
            if(strcmp(line,type_str) == 0){
               contor++;
            }
        }

        fclose(file);

        //transmit numarul de Recv care trebuie facut
        MPI_Send(&contor, 1, MPI_INT, id + 1, pt1, MPI_COMM_WORLD);

        //deschid din nou folderul pentru citire date din el
        file = fopen(name, "r");
        if (file == NULL) {
		    printf("Eroare la deschiderea fisierului de intrare!\n");
		    exit(1);
	    }

        bff_Res_horror = malloc(2048 * 2048 * 500 * sizeof(char));
        int ajut1 = -1;
        while (fgets(line, line_size, file) != NULL) {
            
            if(strcmp(line,type_str) == 0){
                ok1 = 1;
                a[pt1].str_res = malloc(2048 * 2048 * 500 * sizeof(char));
                a[pt1].ok = id + 1;
                ajut1 = pt1;
            } else {
                //a[pt1].str_res = NULL;
                //a[pt1].ok = 0;
            }

            pt1++;

            if(ok1 == 1 && strcmp(line,"\n") != 0 && strcmp(line,type_str) != 0){
                strcat(sir, line);
                //calculez pozitii newline
                pos1[p1] = pos1[p1-1] + strlen(line);
                p1++;
            }

            if(strcmp(line,"\n") == 0  && ok1 == 1){
                ok1 = 0;
                size = strlen(sir);
                //trimit mesaj
                MPI_Send(&size, 1, MPI_INT, id + 1, pt1, MPI_COMM_WORLD);
                MPI_Send(sir, size, MPI_CHAR, id + 1, pt1, MPI_COMM_WORLD);


                //trimit pos
                MPI_Send(&p1, 1, MPI_INT, id + 1, pt1, MPI_COMM_WORLD);
                MPI_Send(pos1, p1, MPI_INT, id + 1, pt1, MPI_COMM_WORLD);

                memset(sir,0,strlen(sir));

                //resetez info text send
                memset(pos1, 0, p1);
                p1 = 1;

                //primesc date prelucrate
                MPI_Recv(&idx1, 1, MPI_INT, id + 1, pt1, MPI_COMM_WORLD, &status);
                MPI_Recv(a[ajut1].str_res, idx1, MPI_CHAR, id + 1, pt1, MPI_COMM_WORLD, &status);

                //resetare
                memset(bff_Res_horror, 0, idx1);
            }
        }

        //cazul in care e ultimul
        if(strlen(sir) > 0){
            //trimit mesaj
            size = strlen(sir);
            MPI_Send(&size, 1, MPI_INT, id + 1, pt1, MPI_COMM_WORLD);
            MPI_Send(sir, size, MPI_CHAR, id + 1, pt1, MPI_COMM_WORLD);
            
            //elimin pos ultim newline
            //p--;
            //pos[p]= -1;

            //trimit pos
            MPI_Send(&p1, 1, MPI_INT, id + 1, pt1, MPI_COMM_WORLD);
            MPI_Send(pos1, p1, MPI_INT, id + 1, pt1, MPI_COMM_WORLD);

            //printf("%s", sir);
            memset(sir,0,strlen(sir));

            //primesc date prelucrate
            MPI_Recv(&idx1, 1, MPI_INT, id + 1, pt1, MPI_COMM_WORLD, &status);
            MPI_Recv(a[ajut1].str_res, idx1, MPI_CHAR, id + 1, pt1, MPI_COMM_WORLD, &status);

            
            //resetare
            memset(bff_Res_horror, 0, idx1);
        }
        stop++;
    }

    //comedy
    if(id == 1){
        //deschid fisierul
        FILE *file = fopen(name, "r");
        if (file == NULL) {
		    printf("Eroare la deschiderea fisierului de intrare!\n");
		    exit(1);
	    }

        char *sir; 
        sir = malloc (2048 * 2048 * 500 * sizeof (char));  
        const size_t line_size = 2048;
        char* line = malloc(line_size);
        int contor = 0;
        ok2 = 0;
        char *type_str = malloc(20*sizeof(char));
        
        strcpy(type_str, "comedy\n");
        
        //calculez cate Send fac
        while (fgets(line, line_size, file) != NULL){
            if(strcmp(line,type_str) == 0){
               contor++;
            }
        }

        fclose(file);

        //transmit numarul de Recv care trebuie facut
        MPI_Send(&contor, 1, MPI_INT, id + 1, pt2, MPI_COMM_WORLD);

        //deschid din nou folderul pentru citire date din el
        file = fopen(name, "r");
        if (file == NULL) {
		    printf("Eroare la deschiderea fisierului de intrare!\n");
		    exit(1);
	    }

        bff_Res_comedy = malloc(2048 * 2048 * 500 * sizeof(char));
        int ajut2 = -1;
        while (fgets(line, line_size, file) != NULL) {
            if(strcmp(line,type_str) == 0){
                ok2 = 1;
                a[pt2].str_res = malloc(2048 * 2048 * 500 * sizeof(char));
                a[pt2].ok = id + 1;
                ajut2 = pt2;
            }else{
                //a[pt2].str_res = NULL;
                //a[pt2].ok = 0;
            }

            pt2++;

            if(ok2 == 1 && strcmp(line,"\n") != 0 && strcmp(line,type_str) != 0){
                strcat(sir, line);
                //calculez pozitii newline
                pos2[p2] = pos2[p2-1] + strlen(line);
                p2++;
            }
            
            if(strcmp(line,"\n") == 0  && ok2 == 1){
                ok2 = 0;
                //elimin newline
                //sir[strlen(sir)-1]='\0';
                size = strlen(sir);
                //trimit mesaj
                MPI_Send(&size, 1, MPI_INT, id + 1, pt2, MPI_COMM_WORLD);
                MPI_Send(sir, size, MPI_CHAR, id + 1, pt2, MPI_COMM_WORLD);
               
                //trimit pos
                MPI_Send(&p2, 1, MPI_INT, id + 1, pt2, MPI_COMM_WORLD);
                MPI_Send(pos2, p2, MPI_INT, id + 1, pt2, MPI_COMM_WORLD);
                
                //printf("%s", sir);
                memset(sir,0,strlen(sir));
                //printf("\n");

                //resetez info text send
                memset(pos2, 0, p2);
                p2 = 1;
                //primesc date prelucrate
                MPI_Recv(&idx2, 1, MPI_INT, id + 1, pt2, MPI_COMM_WORLD, &status);
                MPI_Recv(a[ajut2].str_res , idx2, MPI_CHAR, id + 1, pt2, MPI_COMM_WORLD, &status);
                

                //resetare
                memset(bff_Res_comedy, 0, idx2);
            }
        }

        //cazul in care e ultimul
        if(strlen(sir) > 0){
            //trimit mesaj
            size = strlen(sir);
            MPI_Send(&size, 1, MPI_INT, id + 1, pt2, MPI_COMM_WORLD);
            MPI_Send(sir, size, MPI_CHAR, id + 1, pt2, MPI_COMM_WORLD);
            

            //trimit pos
            MPI_Send(&p2, 1, MPI_INT, id + 1, pt2, MPI_COMM_WORLD);
            MPI_Send(pos2, p2, MPI_INT, id + 1, pt2, MPI_COMM_WORLD);

            //printf("%s", sir);
            memset(sir,0,strlen(sir));
            
            //primesc date prelucrate
            MPI_Recv(&idx2, 1, MPI_INT, id + 1, pt2, MPI_COMM_WORLD, &status);
            MPI_Recv(a[ajut2].str_res , idx2, MPI_CHAR, id + 1, pt2, MPI_COMM_WORLD, &status);

            
            //resetare
            memset(bff_Res_comedy, 0, idx2);
        }
        stop++;
    }
    //fantasy
    if(id == 2){
        //deschid fisierul
        FILE *file = fopen(name, "r");
        if (file == NULL) {
		    printf("Eroare la deschiderea fisierului de intrare!\n");
		    exit(1);
	    }

        char *sir; 
        sir = malloc (2048 * 2048 * 500 * sizeof (char));  
        const size_t line_size = 2048;
        char* line = malloc(line_size);
        int contor = 0;
        ok3 = 0;
        char *type_str = malloc(20*sizeof(char));
        
        strcpy(type_str, "fantasy\n");
        
        //calculez cate Send fac
        while (fgets(line, line_size, file) != NULL){
            if(strcmp(line,type_str) == 0){
               contor++;
            }
        }

        fclose(file);

        //transmit numarul de Recv care trebuie facut
        MPI_Send(&contor, 1, MPI_INT, id + 1, pt3, MPI_COMM_WORLD);

        //deschid din nou folderul pentru citire date din el
        file = fopen(name, "r");
        if (file == NULL) {
		    printf("Eroare la deschiderea fisierului de intrare!\n");
		    exit(1);
	    }

        bff_Res_fantasy = malloc(2048 * 2048 * 500 * sizeof(char));
        int ajut3 = -1;
        while (fgets(line, line_size, file) != NULL) {
            if(strcmp(line,type_str) == 0){
                ok3 = 1;
                a[pt3].str_res = malloc(2048 * 2048 * 500 * sizeof(char));
                a[pt3].ok = id + 1;
                ajut3 = pt3;
            }else{
                //a[pt3].str_res = NULL;
                //a[pt3].ok = 0;
            }
            
            pt3++;
            if(ok3 == 1 && strcmp(line,"\n") != 0 && strcmp(line,type_str) != 0){
                strcat(sir, line);
                //calculez pozitii newline
                pos3[p3] = pos3[p3-1] + strlen(line);
                p3++;
            }
            
            if(strcmp(line,"\n") == 0  && ok3 == 1){
                ok3 = 0;
                //elimin newline
                //sir[strlen(sir)-1]='\0';
                size = strlen(sir);
                //trimit mesaj
                MPI_Send(&size, 1, MPI_INT, id + 1, pt3, MPI_COMM_WORLD);
                MPI_Send(sir, size, MPI_CHAR, id + 1, pt3, MPI_COMM_WORLD);
 
               
                //trimit pos
                MPI_Send(&p3, 1, MPI_INT, id + 1, pt3, MPI_COMM_WORLD);
                MPI_Send(pos3, p3, MPI_INT, id + 1, pt3, MPI_COMM_WORLD);
                
                //printf("%s", sir);
                memset(sir,0,strlen(sir));
                //printf("\n");

                //resetez info text send
                memset(pos3, 0, p3);
                p3 = 1;
                //primesc date prelucrate
                MPI_Recv(&idx3, 1, MPI_INT, id + 1, pt3, MPI_COMM_WORLD, &status);
                MPI_Recv(a[ajut3].str_res, idx3, MPI_CHAR, id + 1, pt3, MPI_COMM_WORLD, &status);


                //resetare
                memset(bff_Res_fantasy, 0, idx3);
            }
        }

        //cazul in care e ultimul
        if(strlen(sir) > 0){
            //trimit mesaj
            size = strlen(sir);
            MPI_Send(&size, 1, MPI_INT, id + 1, pt3, MPI_COMM_WORLD);
            MPI_Send(sir, size, MPI_CHAR, id + 1, pt3, MPI_COMM_WORLD);
            
            //elimin pos ultim newline
            //p--;
            //pos[p]= -1;

            //trimit pos
            MPI_Send(&p3, 1, MPI_INT, id + 1, pt3, MPI_COMM_WORLD);
            MPI_Send(pos3, p3, MPI_INT, id + 1, pt3, MPI_COMM_WORLD);

            //printf("%s", sir);
            memset(sir,0,strlen(sir));
            
            //primesc date prelucrate
            MPI_Recv(&idx3, 1, MPI_INT, id + 1, pt3, MPI_COMM_WORLD, &status);
            MPI_Recv(a[ajut3].str_res, idx3, MPI_CHAR, id + 1, pt3, MPI_COMM_WORLD, &status);
            
            //resetare
            memset(bff_Res_fantasy, 0, idx3);
        }
        stop++;
    }

    //science-fiction
    if(id == 3){
        //deschid fisierul
        FILE *file = fopen(name, "r");
        if (file == NULL) {
		    printf("Eroare la deschiderea fisierului de intrare!\n");
		    exit(1);
	    }

        char *sir; 
        sir = malloc (2048 * 2048 * 500 * sizeof (char));  
        const size_t line_size = 2048;
        char* line = malloc(line_size);
        int contor = 0;
        ok4 = 0;
        char *type_str = malloc(20*sizeof(char));
        
        strcpy(type_str, "science-fiction\n");
        
        //calculez cate Send fac
        while (fgets(line, line_size, file) != NULL){
            if(strcmp(line,type_str) == 0){
               contor++;
            }
        }

        fclose(file);

        //transmit numarul de Recv care trebuie facut
        MPI_Send(&contor, 1, MPI_INT, id + 1, pt4, MPI_COMM_WORLD);

        //deschid din nou folderul pentru citire date din el
        file = fopen(name, "r");
        if (file == NULL) {
		    printf("Eroare la deschiderea fisierului de intrare!\n");
		    exit(1);
	    }

        bff_Res_Science_fiction = malloc(2048 * 2048 * 500 * sizeof(char));
        int ajut4 = -1;
        while (fgets(line, line_size, file) != NULL) {
            if(strcmp(line,type_str) == 0){
                ok4 = 1;
                a[pt4].str_res = malloc(2048 * 2048 * 500 * sizeof(char));
                a[pt4].ok = id + 1;
                ajut4 = pt4;
            }else{
                //a[pt4].str_res = NULL;
                //a[pt4].ok = 0;
            }

            pt4++;

            if(ok4 == 1 && strcmp(line,"\n") != 0 && strcmp(line,type_str) != 0){
                strcat(sir, line);
                //calculez pozitii newline
                pos4[p4] = pos4[p4-1] + strlen(line);
                p4++;
            }
            
            if(strcmp(line,"\n") == 0  && ok4 == 1){
                ok4 = 0;
                //elimin newline
                //sir[strlen(sir)-1]='\0';
                size = strlen(sir);
                //trimit mesaj
                MPI_Send(&size, 1, MPI_INT, id + 1, pt4, MPI_COMM_WORLD);
                MPI_Send(sir, size, MPI_CHAR, id + 1, pt4, MPI_COMM_WORLD);

               
                //trimit pos
                MPI_Send(&p4, 1, MPI_INT, id + 1, pt4, MPI_COMM_WORLD);
                MPI_Send(pos4, p4, MPI_INT, id + 1, pt4, MPI_COMM_WORLD);
                
                //printf("%s", sir);
                memset(sir,0,strlen(sir));
                //printf("\n");

                //resetez info text send
                memset(pos4, 0, p4);
                p4 = 1;
                //primesc date prelucrate
                MPI_Recv(&idx4, 1, MPI_INT, id + 1, pt4, MPI_COMM_WORLD, &status);
                MPI_Recv(a[ajut4].str_res, idx4, MPI_CHAR, id + 1, pt4, MPI_COMM_WORLD, &status);
                

                //resetare
                memset(bff_Res_Science_fiction, 0, idx4);
            }
        }

        //cazul in care e ultimul
        if(strlen(sir) > 0){
            //trimit mesaj
            size = strlen(sir);
            MPI_Send(&size, 1, MPI_INT, id + 1, pt4, MPI_COMM_WORLD);
            MPI_Send(sir, size, MPI_CHAR, id + 1, pt4, MPI_COMM_WORLD);
            

            //trimit pos
            MPI_Send(&p4, 1, MPI_INT, id + 1, pt4, MPI_COMM_WORLD);
            MPI_Send(pos4, p4, MPI_INT, id + 1, pt4, MPI_COMM_WORLD);

            //printf("%s", sir);
            memset(sir,0,strlen(sir));
            
            //primesc date prelucrate
            MPI_Recv(&idx4, 1, MPI_INT, id + 1, pt4, MPI_COMM_WORLD, &status);
            MPI_Recv(a[ajut4].str_res, idx4, MPI_CHAR, id + 1, pt4, MPI_COMM_WORLD, &status);
            
            
            //resetare
            memset(bff_Res_Science_fiction, 0, idx4);
        }
        stop++;
    }

    //bariera
    pthread_barrier_wait(&barrier);

    if(id == 1){
        char *new_name = malloc(11);
        
        //scriu in fisier
        strcpy(new_name, name);
        int len = strlen(name);
        new_name[len-1] = 't';
        new_name[len-2] = 'u';
        new_name[len-3] = 'o';

        FILE *outf = fopen(new_name, "w");

        if (outf == NULL) {
		    printf("Eroare la deschiderea fisierului de intrare!\n");
		    exit(1);
	    }

        for(int f = 1; f < MAX(MAX(pt1,pt2), MAX(pt3,pt4)); f++){
            if(a[f].ok == 1 ){
                fputs("horror\n", outf);
                fputs(a[f].str_res, outf);
                fputs("\n", outf);
            } 
            if(a[f].ok == 2 ){
                fputs("comedy\n", outf);
                fputs(a[f].str_res, outf);
                fputs("\n", outf);
            } 
            if(a[f].ok == 3 ){
                fputs("fantasy\n", outf);
                fputs(a[f].str_res, outf);
                fputs("\n", outf);
            } 
            if(a[f].ok == 4 ){
                fputs("science-fiction\n", outf);
                fputs(a[f].str_res, outf);
                fputs("\n", outf);
            }        
        }
        fclose (outf);
    }

}

void master()
{
    pthread_t threads[NUM_THREADS];
    int r;
    long id;
    void *stat;
    long arguments[NUM_THREADS];

    for (id = 0; id < NUM_THREADS; id++) {
        // arg = 0 (horror), 1 (comedy), 2 (fantasy), 3 (science-fiction).
        arguments[id] = id;
            r = pthread_create(&threads[id], NULL, f, (void *) &arguments[id]);
        
        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    }

    for (id = 0; id < NUM_THREADS; id++) {
            r = pthread_join(threads[id], &stat);
 
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
    }

}


void *g1(void *arg)
{
    
    long id = *(long*) arg;
    
    //receptionez datele MPI_Recv
    MPI_Recv(&size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(buffer, size, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    //primesc lungime pos
    MPI_Recv(&p1, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    pos_in_text = malloc(p1 * sizeof(int));
    //primesc vec pos
    MPI_Recv(pos_in_text, p1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    pthread_exit(NULL);
}

void *g2(void *arg)
{
    
    long id = *(long*) arg;
    
    //receptionez datele MPI_Recv
    MPI_Recv(&size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(buffer, size, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    //primesc lungime pos
    
    MPI_Recv(&p2, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    pos_in_text = malloc(p2 * sizeof(int));
    //printf("\n%d\n", p2);
    
    //primesc vec pos
    MPI_Recv(pos_in_text, p2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);


    pthread_exit(NULL);
}

void *g3(void *arg)
{
    
    long id = *(long*) arg;
    
    //receptionez datele MPI_Recv
    MPI_Recv(&size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(buffer, size, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    //primesc lungime pos
    
    MPI_Recv(&p3, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    pos_in_text = malloc(p3 * sizeof(int));
    //printf("\n%d\n", p2);
    
    //primesc vec pos
    MPI_Recv(pos_in_text, p3, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    pthread_exit(NULL);
}

void *g4(void *arg)
{
    
    long id = *(long*) arg;
    
    //receptionez datele MPI_Recv
    MPI_Recv(&size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(buffer, size, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    //primesc lungime pos
    
    MPI_Recv(&p4, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    pos_in_text = malloc(p4 * sizeof(int));
    //printf("\n%d\n", p4);
    
    //primesc vec pos
    MPI_Recv(pos_in_text, p4, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    pthread_exit(NULL);
}

void *f4(void *arg)
{
    long id = *(long*) arg;
    int nr_line[20] = {-1};
    int k = 0;
    
etc4:
    pthread_mutex_lock(&mutex);
    
    for(int i = 0; i < p4 && k <= 20; i++){
        if(pos_in_text[i] != -1){
            nr_line[k] = pos_in_text[i];
            if(k != 20){
                pos_in_text[i] = -1;
            }
            k++;
        }
    }

    pthread_mutex_unlock(&mutex);

    if(k != 0){
        int c = 0;
        for(int i = nr_line[0]; i <= nr_line[k-1]; i++){
            
            if(buffer[i] == ' ' || buffer[i] == '\n'){
                c++;
            }
            if(c == 7){
                c = 0;
                //merg inapoi si inversez cuvantul
                int aux1 = i-1, aux2 = i-1;
                char litera;
                while(buffer[aux1] != ' '){
                    aux1--;
                }
                aux1++;
                while(aux1 <= aux2){
                    litera = buffer[aux1];
                    buffer[aux1] = buffer[aux2];
                    buffer[aux2] = litera;
                    aux1++;
                    aux2--;
                }
            }
            
            if(buffer[i] == '\n'){
                c = 0;
            }

        }
    }

    //bariera??

    for(int i = 0; i < p4; i++){
        if(pos_in_text[i] != -1){
            i = p4;

            //resetez
            for(int k = 0; k < 20; k++){
                nr_line[k] = -1;
            }
            k = 0;
            goto etc4;
        }
    }
    pthread_exit(NULL);
}

void *f3(void *arg)
{
    long id = *(long*) arg;
    int nr_line[20] = {-1};
    int k = 0;

etc3:
    pthread_mutex_lock(&mutex);
    
    for(int i = 0; i < p3 && k <= 20; i++){
        if(pos_in_text[i] != -1){
            nr_line[k] = pos_in_text[i];
            if(k != 20){
                pos_in_text[i] = -1;
            }
            k++;
        }
    }

    pthread_mutex_unlock(&mutex);

    if(k != 0){
        for(int i = nr_line[0] + 1; i <= nr_line[k-1]; i++){
            
            if(strchr(alph_low, buffer[nr_line[0]]) != NULL){
                buffer[nr_line[0]] -= 32;
            }

            if((buffer[i-1] == ' ' || buffer[i-1] == '\n') && strchr(alph_low, buffer[i]) != NULL){
                buffer[i] -= 32;
            }

        }
    }

    //bariera??

    for(int i = 0; i < p3; i++){
        if(pos_in_text[i] != -1){
            i = p3;

            //resetez
            for(int k = 0; k < 20; k++){
                nr_line[k] = -1;
            }
            k = 0;
            goto etc3;
        }
    }
    pthread_exit(NULL);
}

void *f2(void *arg)
{
    long id = *(long*) arg;
    int nr_line[20] = {-1};
    int k = 0;

etc2:
    pthread_mutex_lock(&mutex);
    
    for(int i = 0; i < p2 && k <= 20; i++){
        if(pos_in_text[i] != -1){
            nr_line[k] = pos_in_text[i];
            if(k != 20){
                pos_in_text[i] = -1;
            }
            k++;
        }
    }

    pthread_mutex_unlock(&mutex);

    if(k != 0){
        //printf("\n%ld\n",id);
        //prelucrez
        int par = 0;
        for(int i = nr_line[0]; i <= nr_line[k-1]; i++){
            
            if(buffer[i] == ' ' || buffer[i] == '\n'){
                par = 0;
            }
            
            if(strchr(alph2, buffer[i]) != NULL){
                par++;
               if(par%2 == 0 && buffer[i] <= 122 && buffer[i] >= 97){
                    buffer[i] -= 32;
                }

            }
            if(strchr(alph2, buffer[i]) == NULL && buffer[i] != ' ' && buffer[i] != '\n'){
                par++;
            }
        }
    }

    //bariera??

    for(int i = 0; i < p2; i++){
        if(pos_in_text[i] != -1){
            i = p2;

            //resetez
            for(int k = 0; k < 20; k++){
                nr_line[k] = -1;
            }
            k = 0;
            goto etc2;
        }
    }
    pthread_exit(NULL);
}

void *f1(void *arg)
{

    long id = *(long*) arg;
    int nr_line[20] = {-1};
    int k = 0;

etc:

    pthread_mutex_lock(&mutex);
    for(int i = 0; i < p1 && k <= 20; i++){
        if(pos_in_text[i] != -1){
            nr_line[k] = pos_in_text[i];
            if(k != 20){
                pos_in_text[i] = -1;
            }
            k++;
        }
    }
 
    if(k != 0){
        //printf("\n%ld\n",id);
        //prelucrez
        for(int i = nr_line[0]; i <= nr_line[k-1]; i++){
            //printf("%c ", buffer[i]);
            if(strchr(alph1, buffer[i]) != NULL){
                bff_Res_horror[idx1] = buffer[i];
                idx1++;
                bff_Res_horror[idx1] = buffer[i];
                idx1++;
                //ma asigur ca duplicata e litera mica
                if(buffer[i] <= 90 && buffer[i] >= 65){
                    bff_Res_horror[idx1 - 1] += 32; 
                }
            } else {
                bff_Res_horror[idx1] = buffer[i];
                idx1++;
            }
        }
    }

    pthread_mutex_unlock(&mutex);

    //bariera??

    for(int i = 0; i < p1; i++){
        if(pos_in_text[i] != -1){
            i = p1;

            //resetez
            for(int k = 0; k < 20; k++){
                nr_line[k] = -1;
            }
            k = 0;

            goto etc;
        }
    }

    pthread_exit(NULL);
}

void worker1(){
   
    long cores = sysconf(_SC_NPROCESSORS_CONF);
    pthread_t threads[cores];
    int r;
    long id;
    void *stat;
    long arguments[cores];
    pthread_mutex_init(&mutex,NULL);
    buffer =  malloc (2048 * 2048 * 500 * sizeof (char));

    //receptionez nr de senduri
    MPI_Recv(&counter, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    bff_Res_horror = malloc(2048 * 2048 * 500 * sizeof(char));

    while(counter != 0){
        
        counter--;
        id = 0;
        arguments[id] = id;
        
        r = pthread_create(&threads[id], NULL, g1, (void *) &arguments[id]);

        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    
        r = pthread_join(threads[id], &stat);
 
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }

        //thread-uri care prelucreaza buffer
        for (id = 1; id < cores; id++) {
            arguments[id] = id;
            r = pthread_create(&threads[id], NULL, f1, (void *) &arguments[id]);
 
        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    }

    for (id = 1; id < cores; id++) {
        r = pthread_join(threads[id], &stat);
 
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
    }
    /*for (int l = 0; l < idx-2; l++){
        printf("%c", bff_Res_horror[l]);
    }*/

    idx1 -= 2;
    bff_Res_horror[idx1] = '\0';
    //trimit text prelucrat
    MPI_Send(&idx1, 1, MPI_INT, 0, status.MPI_TAG, MPI_COMM_WORLD);
    MPI_Send(bff_Res_horror, idx1, MPI_CHAR, 0, status.MPI_TAG, MPI_COMM_WORLD);

    //reactualizare
    memset(bff_Res_horror, '\0', idx1);
    idx1 = 0;
    //free(bff_Res_horror);
    }
    //deblocare zona protejata/ distrug mutex
	pthread_mutex_destroy(&mutex); 
}

void worker2(){
    long cores = sysconf(_SC_NPROCESSORS_CONF);
    pthread_t threads[cores];
    int r;
    long id;
    void *stat;
    long arguments[cores];
    pthread_mutex_init(&mutex,NULL);
    buffer =  malloc (2048 * 2048 * 500 * sizeof (char));

    //receptionez nr de senduri
    MPI_Recv(&counter, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    
    while(counter != 0){
        
        counter--;
        id = 0;
        arguments[id] = id;
        
        r = pthread_create(&threads[id], NULL, g2, (void *) &arguments[id]);

        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    
        r = pthread_join(threads[id], &stat);
        
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
        
        //thread-uri care prelucreaza buffer
        for (id = 1; id < cores; id++) {
            arguments[id] = id;
            r = pthread_create(&threads[id], NULL, f2, (void *) &arguments[id]);
 
        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    }
    
    for (id = 1; id < cores; id++) {
        r = pthread_join(threads[id], &stat);
 
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
    }
    //trimit text prelucrat
    
    MPI_Send(&size, 1, MPI_INT, 0, status.MPI_TAG, MPI_COMM_WORLD);
    MPI_Send(buffer, size, MPI_CHAR, 0, status.MPI_TAG, MPI_COMM_WORLD);

    //reactualizare
    memset(buffer, '\0', size);
    size = 0;
    
    }

    //deblocare zona protejata/ distrug mutex
	pthread_mutex_destroy(&mutex);
}

void worker3(){
    long cores = sysconf(_SC_NPROCESSORS_CONF);
    pthread_t threads[cores];
    int r;
    long id;
    void *stat;
    long arguments[cores];
    pthread_mutex_init(&mutex,NULL);
    buffer =  malloc (2048 * 2048 * 500 * sizeof (char));

    //receptionez nr de senduri
    MPI_Recv(&counter, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    
    while(counter != 0){
        
        counter--;
        id = 0;
        arguments[id] = id;
        
        r = pthread_create(&threads[id], NULL, g3, (void *) &arguments[id]);

        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    
        r = pthread_join(threads[id], &stat);
        
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
        
        //thread-uri care prelucreaza buffer
        for (id = 1; id < cores; id++) {
            arguments[id] = id;
            r = pthread_create(&threads[id], NULL, f3, (void *) &arguments[id]);
 
        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    }
    
    for (id = 1; id < cores; id++) {
        r = pthread_join(threads[id], &stat);
 
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
    }
    //trimit text prelucrat
    
    MPI_Send(&size, 1, MPI_INT, 0, status.MPI_TAG, MPI_COMM_WORLD);
    MPI_Send(buffer, size, MPI_CHAR, 0, status.MPI_TAG, MPI_COMM_WORLD);

    //reactualizare
    memset(buffer, '\0', size);
    size = 0;
    
    }

    //deblocare zona protejata/ distrug mutex
	pthread_mutex_destroy(&mutex);
};

void worker4(){
    long cores = sysconf(_SC_NPROCESSORS_CONF);
    pthread_t threads[cores];
    int r;
    long id;
    void *stat;
    long arguments[cores];
    pthread_mutex_init(&mutex,NULL);
    buffer =  malloc (2048 * 2048 * 500 * sizeof (char));

    //receptionez nr de senduri
    MPI_Recv(&counter, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    
    while(counter != 0){
        
        counter--;
        id = 0;
        arguments[id] = id;
        
        r = pthread_create(&threads[id], NULL, g4, (void *) &arguments[id]);

        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    
        r = pthread_join(threads[id], &stat);
        
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
        
        //thread-uri care prelucreaza buffer
        for (id = 1; id < cores; id++) {
            arguments[id] = id;
            r = pthread_create(&threads[id], NULL, f4, (void *) &arguments[id]);
 
        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    }
    
    for (id = 1; id < cores; id++) {
        r = pthread_join(threads[id], &stat);
 
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
    }
    //trimit text prelucrat
    
    MPI_Send(&size, 1, MPI_INT, 0, status.MPI_TAG, MPI_COMM_WORLD);
    MPI_Send(buffer, size, MPI_CHAR, 0, status.MPI_TAG, MPI_COMM_WORLD);
    //printf("%s\n", buffer);

    //reactualizare
    memset(buffer, '\0', size);
    size = 0;
    
    }

    //deblocare zona protejata/ distrug mutex
	pthread_mutex_destroy(&mutex);
};

int main(int argc, char ** argv) { 

    name = malloc(strlen(argv[1]) * sizeof(char));
    strcpy(name, argv[1]);

    int provided, myrank, numtasks;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); 
   
    //printf("myrank = %d\n", myrank);
    if (myrank == 0) {
        //initializez mutex-ul inainte sa creez firele
	    pthread_mutex_init(&mutex_var,NULL);
        //initializez bariera
	    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
        master();
        //deblocare zona protejata/ distrug mutex
	    pthread_mutex_destroy(&mutex_var);
        //distrug bariera
	    pthread_barrier_destroy(&barrier);
    } else if (myrank == 1){
        worker1();
        } else if (myrank == 2){
            worker2();
            } else if (myrank == 3){
                worker3();
                } else if (myrank == 4){
                    worker4();
                    }
    MPI_Finalize();   
    
    return 0; 
    }


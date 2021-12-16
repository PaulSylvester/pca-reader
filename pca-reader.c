#include "include/pca-reader.h"
#define _GNU_SOURCE


/**
 * holds records from the pcas file
 */
typedef struct process_conection_assignments
{
    char *filename_path;
    char *records;
    int no_records;
} process_conection_assignments;

static process_conection_assignments g_pcas = {0};
static process_conection_assignment g_pca = {0};

void free_pca()
{
    g_pca.pca_id=0;
    g_pca.oflag=0;
    if(g_pca.requested_file)
        free(g_pca.requested_file);
    g_pca.requested_file=NULL;

    if(g_pca.socket_path)
        free(g_pca.socket_path);
    g_pca.socket_path=NULL;

    if(g_pca.client_process_filepath)
        free(g_pca.client_process_filepath);
    g_pca.client_process_filepath=NULL;
}

static void free_pcas()
{
    g_pcas.no_records=0;
    if(g_pcas.records)
        free(g_pcas.records);
    g_pcas.records = NULL;

    if(g_pcas.filename_path)
        free(g_pcas.filename_path);
    g_pcas.filename_path = NULL;
}


/**
 * @brief allocates memory if available and concatenates src with des otherwise will terminate process
 * @param des - gets des appended to it
 * @param src - provides the data to append to src
 * @return des - result of strcat
 */
static char* malloc_strcat(char *des, char *src){

    if(src)
    {
        if(!(des = malloc( strlen(src) + 1) ))
        {
            puts ("Failed to alloacate memory!");
            exit(EXIT_FAILURE);
        }else
            strcat(des,src);
    }
    return des;
}

static int set_pcas_filename_path()
{
    g_pcas.filename_path = malloc_strcat(g_pcas.filename_path, DEFAULT_PCAS_FILEPATH);
    return open(g_pcas.filename_path,O_RDONLY);
}

/**
 * @brief fetches records from the pcas (process connection assignments) file and stores them in the g_pcas struct
 * @return
 */
static int set_pcas_from_file(){

    struct stat st;

    int fd = set_pcas_filename_path();

    if(fd != -1)
        fstat(fd, &st);
    else {
        printf("The pcas file:%s could not be opened\n",g_pcas.filename_path);
        return -1;
    }

    off_t size = st.st_size;

    if(size > 0){
        g_pcas.records = (char *) malloc(size + 1);
        if(g_pcas.records){
            char buf[sizeof (char) * 2];
            ssize_t bytes_read;
            while ((bytes_read = read(fd, buf, sizeof (char))) > 0)
            {
                strcat(g_pcas.records, buf);
                if(*buf=='\n')
                    g_pcas.no_records += 1;
            }
        }
    } else {
        printf("No records found in the process connection assignment file:%s\n",g_pcas.filename_path);
        return -1;
    }
    close(fd);
    return 0;
}

const process_conection_assignment * get_pca_from_file(int pca_id)
{
    if(set_pcas_from_file() == -1)
        goto FREE_PCAS;

    char *rows = NULL, *field = NULL;
    int foundid = 0;

    g_pca.pca_id = pca_id;
    rows = strtok(g_pcas.records,"\n"); //split rows by using newline as delimiter

    for (int i = 1; i < g_pcas.no_records; i++ ) {

        if(rows && atoi( rows ) == pca_id ) //rows should start with an integer id number atoi will deal with that
        {
            foundid = pca_id;
            break;
        }
        rows = strtok( NULL, "\n" );//point to next row
    }

    if(!foundid)
    {
        printf("Unable to find pca_id:%d in process connection assignment file:%s\n", pca_id, g_pcas.filename_path);
        goto FREE_PCAS;
    }

    //The order of calling strtok is important - must match the order of the fields in the pcas file
    field = strtok(rows,",");//eat id field as this value has been set already.
    g_pca.requested_file = malloc_strcat( g_pca.requested_file, strtok( NULL, "," ) );
    field = strtok( NULL, "," );
    if(field && !isdigit( (int)*field ) )
    {
        printf("There was an issue getting the open flag field from file:%s\nPlease check the order of the fields and that they all exist.\n", g_pcas.filename_path);
        goto FREE_PCAS;
    }
    g_pca.oflag = atoi(field);
    g_pca.socket_path = malloc_strcat( g_pca.socket_path, strtok( NULL, "," ) );
    g_pca.client_process_filepath = malloc_strcat( g_pca.client_process_filepath, strtok( NULL, "," ) );

    if(!field)
    {
        printf("There was an issue getting all the field from file:%s\nPlease check the order of the field and that they all exist.\n", g_pcas.filename_path);
        goto FREE_PCAS;
    }

    return &g_pca;

    FREE_PCAS:
    free_pcas();
    return NULL;
}

void __attribute__ ((constructor)) initLibrary(void) {
 //
 // Function that is called when the library is loaded
 //
    printf("The pca_reader library has initialised\n");
}

void __attribute__ ((destructor)) cleanUpLibrary(void) {
 //
 // Function that is called when the library is »closed«.
 //
    free_pca();
    free_pcas();
    printf("The pca_reader library exited\n");
}



#ifndef FD_READER_PRECOMP
#define FD_READER_PRECOMP

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
//TODO: cmake setting?
#define DEFAULT_PCAS_FILEPATH "/home/paulsylvester/Documents/pcas"
/**
 * holds a single record from the pcas file
 */
typedef struct process_conection_assignment {
    int pca_id;
    char *requested_file;
    int oflag;
    char *socket_path;
    char *client_process_filepath;
} process_conection_assignment;

/**
 * @brief Sets the record that a fd_sender instance will use to comunicate with an instance of a fd_reciever client.
 *        (Both instances would use the same pca_id to commuicate with each other over unix socket stream version).
 * @param pca_id - should be a unque id in the pcas (process connection assignments) file.
 * @return
 */
const process_conection_assignment * get_pca_from_file(int pca_id);
/**
 * @brief Free memory allocated for a pca, sets pca_id and oflag to 0
 */
void free_pca();
#ifdef __cplusplus
}
#endif

#endif //FD_READER_PRECOMP

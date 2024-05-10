#include "API.h"
#include <microhttpd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "rotas.h"


struct postStatus {
    bool status;
    char *buff;
};

// Funciona
static enum MHD_Result ahc_echo(void * cls,
                                struct MHD_Connection * connection,
                                const char * url,
                                const char * method,
                                const char * version,
                                const char * upload_data,
                                size_t * upload_data_size,
                                void ** ptr) {
    //const char * page = cls;
    struct MHD_Response * response;
    int ret;

    char * page1 = NULL;

    struct postStatus *post = NULL;
    post = (struct postStatus*)*ptr;

    if(post == NULL) {
        post = malloc(sizeof(struct postStatus));
        post->status = false;
        *ptr = post;
    }

    if(!post->status) {
        post->status = true;
        return MHD_YES;
    } else {
        if(*upload_data_size != 0) {
            post->buff = malloc(*upload_data_size);
            strncpy(post->buff, upload_data, *upload_data_size);
            *upload_data_size = 0;
            return MHD_YES;
        } else {

            page1 = criaRotas(url,post->buff);
            free(post->buff);
        }
    }

    if(post != NULL) {
        free(post);
    }

    response = MHD_create_response_from_buffer (strlen(page1),
                                                (void*) page1,
                                                MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection,
                             MHD_HTTP_OK,
                             response);
    MHD_destroy_response(response);
    return ret;
}

void servidor(){
    struct MHD_Daemon * d;

    d = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
                         9000,
                         NULL,
                         NULL,
                         &ahc_echo,
                         NULL,
                         MHD_OPTION_END);


    if (d == NULL)
        return;
    getchar();

    MHD_stop_daemon(d);
}
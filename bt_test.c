#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

typedef struct {
    char addr[19];
    char name[248];
} bt_info;

int main(int argc, char **argv) {
    inquiry_info *ii = NULL;
    bt_info *info;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    int choice = -1;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    char *line, *linep;
    size_t lenmax = 100, inlen = lenmax;
    int c;

    printf("Detecting Device and binding socket...\n");
    dev_id = hci_get_route(NULL);
    sock = hci_open_dev(dev_id); 

    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    printf("Scanning for devices...\n");
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);

    printf("%d devices found!\n", num_rsp);

    info = (bt_info*)malloc(num_rsp * sizeof(bt_info));
    if (num_rsp < 0) {
        perror("hci_inquiry");
    }

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0) {
            strcpy(name, "[unknown]");
        }
        strcpy((info+i)->name, name);
        strcpy((info+i)->addr, addr);
    }

    for (i = 0; i < num_rsp; i++) {
        fprintf(stdout, "%d %s %s\n", i, (info + i)->addr, (info + i)->name);
    }

    while(choice < 0 || choice > (num_rsp - 1)) {
        line = (char *)malloc(100);
        linep = line;
        lenmax = 100;
        inlen = lenmax;

        for(;;) {
            c = fgetc(stdin);
            if (c == EOF) {
                break;
            }

            if (--inlen == 0) {
                inlen = lenmax;
                char *linen = realloc(linep, lenmax *= 2);

                if(linen == NULL) {
                    free(linep);
                    perror("fail");
                    exit(1);
                }
                line = linen + (line - linep);
                linep = linen;
            }

            if ((*line++ = c) == '\n') {
                break;
            }
        }
        *line = '\0';
        choice = atoi(linep);
        free(linep);
    }



    free(ii);
    free(info);
    close(sock);

    return 0;
}

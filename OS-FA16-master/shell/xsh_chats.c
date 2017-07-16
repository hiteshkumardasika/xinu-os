/* xsh_udp_request.c  -- xsh_upd_request.c*/

#include <xinu.h>

shellcmd xsh_client(int nargs, char *args[]) {

    int port_no, ret_val,temp;
    char buffer[512];
    char message[512];
    uint32 server_ip;
    int local_port = 42000;
    int conn_obj;        // conn_obj of UDP table
    int msg_len, free_status;        // length of message, status of future
    int future_flag = 0;
    future *f_exclusive;
    int value, status;

    if (nargs == 4 && strcmp(args[3], "-f") == 0) {
        future_flag = 1;
    }

    if (nargs > 4 || (nargs > 3 && strcmp(args[3], "-f") != 0)) {
        printf("Too many arguments for udp_request command.\n");
        printf("Use --help to know about usage.\n");
        return -1;
    }

    if (nargs == 3 && strcmp(args[2], "-f") == 0) {
        printf("\ntoo few arguments for future mode.\n");
        return -1;
    }

    if ((nargs == 2) && strncmp(args[1], "--help", 7) == 0) {

        printf("Use: %s  Find length of string\n\n");
        printf("Description:\n");
        printf("\tCommunicate with server using UDP (User packet Protocol).\n");
        printf("Options:\n");
        printf("\tserver_ip:\tIP address of the server\n");
        printf("\tport_no:\tPort number of the server\n");
        printf("\t-f Use future mode \n");
        printf("\t--help\t display this help and exit\n");
        return -1;
    }

    if (nargs < 3) {
        printf("Too few arguments for udp_request command.\n");
        printf("Use --help to know about usage.\n");
        return -1;
    }

    if (dot2ip(args[1], &server_ip) == SYSERR) {
        printf("%s: invalid IP address argument\r\n",
               args[0]);
        return 1;
    }

    port_no = atoi(args[2]);

    conn_obj = udp_register(server_ip, port_no, local_port);        // register a conn_obj in udp table
    if (conn_obj == SYSERR) {
        printf(" Error while calling udp register %d ", local_port);
        return -1;
    }

    if (!future_flag) {
        while (1) {
            printf("\n>");
            fgets(message, 512, CONSOLE);
            msg_len = strlen(message);
            message[msg_len - 1] = '\0';
            if (strcmp(message, "exit") == 0)
                break;

            ret_val = udp_send(conn_obj, message, msg_len);
            if (ret_val == SYSERR) {
                printf(" Error in udp sending packet ");
                return -1;

            }

            ret_val = udp_recv(conn_obj, buffer, sizeof(buffer), 10000);
            if (ret_val == SYSERR) {
                printf("Error in receiving packet");
                return -1;

            }

            if (ret_val == TIMEOUT) {
                printf("Timeout in receiving packet");
                return -1;
            }

            printf("Length of %s is  %s \n", message, buffer);
            memset(buffer, '\0', sizeof(buffer));
            memset(message, '\0', sizeof(message));
        }
        udp_release(conn_obj);
    }
    else
    {
        f_exclusive = future_alloc(FUTURE_EXCLUSIVE);

        if (f_exclusive != NULL )
        {
            resume(create(future_netw_cons, 1024, 20, "fcons1", 1, f_exclusive));

            while(1)
            {
                while(f_exclusive->state==FUTURE_VALID)
                    printf("");

                printf("\n>");
                fgets(message,512,CONSOLE);
                msg_len = strlen(message);
                message[msg_len-1]='\0';

                if(strcmp(message,"exit") == 0)
                    break;
                ret_val = udp_send(conn_obj, message, msg_len);
                if(ret_val == SYSERR)
                {
                    printf(" Error in sending packet ");
                    return -1;
                }

                ret_val = udp_recv(conn_obj, buffer, sizeof(buffer),10000);
                if(ret_val == SYSERR)
                {
                    printf("Error in receiving packet");
                    return -1;
                }
                if(ret_val == TIMEOUT)
                {
                    printf("Timeout in receiving packet");
                    return -1;
                }
                value = atoi(buffer);
                printf("\nProducer produced %d",value);
                status = future_set(f_exclusive, &value);
                if (status < 1)
                {
                    printf("future_set failed\n");
                    return -1;
                }
                if(value==-1)
                    break;
                memset(buffer,'\0',sizeof(buffer));
                memset(message,'\0',sizeof(message));

            }
        }
        udp_release(conn_obj);
        if(!(future_free(f_exclusive)))
            return SYSERR;
    }
    return 0;

}
